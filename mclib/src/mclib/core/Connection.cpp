#include <mclib/core/Connection.h>

#include <mclib/common/DataBuffer.h>
#include <mclib/core/Compression.h>
#include <mclib/core/Encryption.h>
#include <mclib/network/Network.h>
#include <mclib/network/TCPSocket.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/protocol/packets/PacketFactory.h>
#include <mclib/util/Utility.h>

#include <future>
#include <memory>
#include <thread>

namespace mc {
namespace core {

Connection::Connection(protocol::packets::PacketDispatcher* dispatcher,
                       protocol::Version version)
    : protocol::packets::PacketHandler(dispatcher),
      m_Encrypter(std::make_unique<EncryptionStrategyNone>()),
      m_Compressor(std::make_unique<CompressionNone>()),
      m_Socket(std::make_unique<network::TCPSocket>()),
      m_Yggdrasil(std::make_unique<util::Yggdrasil>()),
      m_Protocol(protocol::Protocol::GetProtocol(version)),
      m_SentSettings(false),
      m_Dimension(1) {
    dispatcher->RegisterHandler(protocol::State::Login,
                                protocol::login::Disconnect, this);
    dispatcher->RegisterHandler(protocol::State::Login,
                                protocol::login::EncryptionRequest, this);
    dispatcher->RegisterHandler(protocol::State::Login,
                                protocol::login::LoginSuccess, this);
    dispatcher->RegisterHandler(protocol::State::Login,
                                protocol::login::SetCompression, this);

    dispatcher->RegisterHandler(protocol::State::Status,
                                protocol::status::Response, this);

    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::KeepAlive, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::PlayerPositionAndLook, this);
    dispatcher->RegisterHandler(protocol::State::Play, protocol::play::JoinGame,
                                this);
    dispatcher->RegisterHandler(protocol::State::Play, protocol::play::Respawn,
                                this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::UpdateHealth, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::Disconnect, this);
}

Connection::~Connection() { GetDispatcher()->UnregisterHandler(this); }

network::Socket::Status Connection::GetSocketState() const {
    return m_Socket->GetStatus();
}

void Connection::HandlePacket(protocol::packets::in::JoinGamePacket* packet) {
    m_Dimension = packet->GetDimension();
}

void Connection::HandlePacket(protocol::packets::in::RespawnPacket* packet) {
    m_Dimension = packet->GetDimension();
}

void Connection::HandlePacket(protocol::packets::in::KeepAlivePacket* packet) {
    protocol::packets::out::KeepAlivePacket response(packet->GetAliveId());
    SendPacket(&response);
}

void Connection::HandlePacket(
    protocol::packets::in::PlayerPositionAndLookPacket* packet) {
    using namespace protocol::packets;

    // Used to verify position
    out::TeleportConfirmPacket confirmation(packet->GetTeleportId());
    SendPacket(&confirmation);

    out::PlayerPositionAndLookPacket response(
        packet->GetPosition(), packet->GetYaw(), packet->GetPitch(), true);

    SendPacket(&response);
}

void Connection::HandlePacket(
    protocol::packets::in::UpdateHealthPacket* packet) {
    using namespace protocol::packets;

    if (packet->GetHealth() <= 0) {
        out::ClientStatusPacket status(
            out::ClientStatusPacket::Action::PerformRespawn);
        SendPacket(&status);
    }
}

void Connection::HandlePacket(protocol::packets::in::DisconnectPacket* packet) {
    m_Socket->Disconnect();

    NotifyListeners(&ConnectionListener::OnSocketStateChange,
                    m_Socket->GetStatus());

    if (m_ProtocolState != protocol::State::Play)
        NotifyListeners(&ConnectionListener::OnLogin, false);
}

void Connection::AuthenticateClient(const std::wstring& serverId,
                                    const std::string& sharedSecret,
                                    const std::string& pubkey) {
    bool success = true;
    std::string error = "";

    if (!m_Yggdrasil->IsAuthenticated()) {
        try {
            if (!m_Yggdrasil->Authenticate(m_Username, m_Password)) {
                error = "Failed to authenticate";
                success = false;
            }
        } catch (const util::YggdrasilException& e) {
            error = e.what();
            success = false;
        }
    }

    try {
        if (!m_Yggdrasil->JoinServer(serverId, sharedSecret, pubkey)) {
            success = false;
            error = "Failed to join server through Yggdrasil.";
        }
    } catch (const util::YggdrasilException& e) {
        success = false;
        error = e.what();
    }

    m_Password.clear();
    NotifyListeners(&ConnectionListener::OnAuthentication, success, error);
}

void Connection::HandlePacket(
    protocol::packets::in::EncryptionRequestPacket* packet) {
    std::string pubkey = packet->GetPublicKey();
    std::string verify = packet->GetVerifyToken();

    auto aesEncrypter = std::make_unique<EncryptionStrategyAES>(pubkey, verify);
    protocol::packets::out::EncryptionResponsePacket* encResp =
        aesEncrypter->GenerateResponsePacket();

    AuthenticateClient(packet->GetServerId().GetUTF16(),
                       aesEncrypter->GetSharedSecret(), pubkey);

    SendPacket(encResp);

    m_Encrypter = std::move(aesEncrypter);
}

void Connection::SendSettingsPacket() {
    protocol::packets::out::ClientSettingsPacket clientSettings(
        m_ClientSettings.GetLocale(), m_ClientSettings.GetViewDistance(),
        m_ClientSettings.GetChatMode(), m_ClientSettings.GetChatColors(),
        m_ClientSettings.GetSkinParts(), m_ClientSettings.GetMainHand());

    SendPacket(&clientSettings);

    m_SentSettings = true;
}

void Connection::HandlePacket(
    protocol::packets::in::LoginSuccessPacket* packet) {
    m_ProtocolState = protocol::State::Play;

    NotifyListeners(&ConnectionListener::OnLogin, true);
}

void Connection::HandlePacket(
    protocol::packets::in::SetCompressionPacket* packet) {
    m_Compressor = std::make_unique<CompressionZ>(packet->GetMaxPacketSize());
}

bool Connection::Connect(const std::string& server, u16 port) {
    bool result = false;

    m_Socket = std::make_unique<network::TCPSocket>();
    m_Yggdrasil = std::unique_ptr<util::Yggdrasil>(new util::Yggdrasil());
    m_ProtocolState = protocol::State::Handshake;

    m_Compressor = std::make_unique<CompressionNone>();
    m_Encrypter = std::make_unique<EncryptionStrategyNone>();

    m_Server = server;
    m_Port = port;

    if (isdigit(m_Server.at(0))) {
        network::IPAddress addr(m_Server);

        result = m_Socket->Connect(addr, m_Port);
    } else {
        auto addrs = network::Dns::Resolve(m_Server);
        if (addrs.size() == 0) return false;

        for (auto addr : addrs) {
            if (m_Socket->Connect(addr, m_Port)) {
                result = true;
                break;
            }
        }
    }

    m_Socket->SetBlocking(false);

    if (result)
        NotifyListeners(&ConnectionListener::OnSocketStateChange,
                        m_Socket->GetStatus());

    return result;
}

void Connection::Disconnect() {
    m_Socket->Disconnect();
    NotifyListeners(&ConnectionListener::OnSocketStateChange,
                    m_Socket->GetStatus());
}

protocol::packets::Packet* Connection::CreatePacket(DataBuffer& buffer) {
    std::size_t readOffset = buffer.GetReadOffset();
    VarInt length;

    try {
        buffer >> length;
    } catch (const std::out_of_range&) {
        // This will happen when the buffer only contains part of the VarInt,
        // so only part of the packet was received so far.
        // The buffer read offset isn't advanced when the exception is thrown,
        // so no need to set it back to what it was.
        return nullptr;
    }

    if (length.GetInt() == 0 || buffer.GetRemaining() < (u32)length.GetInt()) {
        // Reset the read offset back to what it was because the full packet
        // hasn't been received yet.
        buffer.SetReadOffset(readOffset);
        return nullptr;
    }

    DataBuffer decompressed = m_Compressor->Decompress(buffer, length.GetInt());

    return protocol::packets::PacketFactory::CreatePacket(
        m_Protocol, m_ProtocolState, decompressed, length.GetInt(), this);
}

void Connection::CreatePacket() {
    while (true) {
        DataBuffer buffer;

        m_Socket->Receive(buffer, 4096);

        if (buffer.IsEmpty()) {
            if (m_Socket->GetStatus() != network::Socket::Connected) {
                NotifyListeners(&ConnectionListener::OnSocketStateChange,
                                m_Socket->GetStatus());
            }
            return;
        }

        m_HandleBuffer << m_Encrypter->Decrypt(buffer);

        do {
            try {
                protocol::packets::Packet* packet =
                    CreatePacket(m_HandleBuffer);

                if (packet) {
                    // Only send the settings after the server has accepted the
                    // new protocol state.
                    if (!m_SentSettings &&
                        packet->GetProtocolState() == protocol::State::Play) {
                        SendSettingsPacket();
                    }

                    this->GetDispatcher()->Dispatch(packet);
                    protocol::packets::PacketFactory::FreePacket(packet);
                } else {
                    break;
                }
            } catch (const protocol::UnfinishedProtocolException&) {
                // Ignore for now
            }
        } while (!m_HandleBuffer.IsFinished() && m_HandleBuffer.GetSize() > 0);

        if (m_HandleBuffer.IsFinished())
            m_HandleBuffer = DataBuffer();
        else if (m_HandleBuffer.GetReadOffset() != 0)
            m_HandleBuffer =
                DataBuffer(m_HandleBuffer, m_HandleBuffer.GetReadOffset());

        if (m_Socket->GetStatus() != network::Socket::Connected) {
            NotifyListeners(&ConnectionListener::OnSocketStateChange,
                            m_Socket->GetStatus());
        }
    }
}

bool Connection::Login(const std::string& username,
                       const std::string& password) {
    static const std::string fml("\0FML\0", 5);

    if (m_Socket->GetStatus() != network::Socket::Status::Connected)
        return false;

    protocol::packets::out::HandshakePacket handshake(
        static_cast<s32>(m_Protocol.GetVersion()), m_Server + fml, m_Port,
        protocol::State::Login);

    SendPacket(&handshake);
    m_ProtocolState = protocol::State::Login;

    m_Email = username;

    if (username.find("@") != std::string::npos) {
        m_Yggdrasil->Authenticate(username, password);

        m_Username = m_Yggdrasil->GetPlayerName();
    } else {
        m_Username = username;
    }

    m_Password = password;

    protocol::packets::out::LoginStartPacket loginStart(m_Username);
    SendPacket(&loginStart);

    return true;
}

bool Connection::Login(const std::string& username, AuthToken token) {
    static const std::string fml("\0FML\0", 5);

    if (m_Socket->GetStatus() != network::Socket::Status::Connected)
        return false;

    if (!token.IsValid()) {
        if (!token.Validate(username)) return false;
    }

    m_Yggdrasil = std::move(token.GetYggdrasil());

    protocol::packets::out::HandshakePacket handshake(
        static_cast<s32>(m_Protocol.GetVersion()), m_Server + fml, m_Port,
        protocol::State::Login);

    SendPacket(&handshake);
    m_ProtocolState = protocol::State::Login;

    m_Username = username;

    protocol::packets::out::LoginStartPacket loginStart(m_Username);
    SendPacket(&loginStart);

    return true;
}

void Connection::HandlePacket(
    protocol::packets::in::status::ResponsePacket* packet) {
    std::string response = mc::to_string(packet->GetResponse());

    json data;

    try {
        data = json::parse(response);
    } catch (json::parse_error&) {
    }

    NotifyListeners(&ConnectionListener::OnPingResponse, data);
}

void Connection::Ping() {
    std::string fml("\0FML\0", 5);

    protocol::packets::out::HandshakePacket handshake(
        static_cast<s32>(m_Protocol.GetVersion()), m_Server + fml, m_Port,
        protocol::State::Status);
    SendPacket(&handshake);

    m_ProtocolState = protocol::State::Status;

    protocol::packets::out::status::RequestPacket request;
    SendPacket(&request);
}

}  // namespace core
}  // namespace mc
