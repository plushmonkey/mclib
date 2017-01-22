#include "Connection.h"

#include "Encryption.h"
#include "Compression.h"
#include "Packets/PacketDispatcher.h"
#include "Packets/PacketFactory.h"
#include "Network/Network.h"
#include "Network/TCPSocket.h"

#include <future>
#include <thread>
#include <memory>

namespace Minecraft {

Connection::Connection(Minecraft::Packets::PacketDispatcher* dispatcher)
        : Minecraft::Packets::PacketHandler(dispatcher),
        m_Encrypter(new Minecraft::EncryptionStrategyNone()),
        m_Compressor(new Minecraft::CompressionNone()),
        m_Socket(new Network::TCPSocket()),
        m_Yggdrasil(new Yggdrasil())
{
    using namespace Minecraft;

    dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::Disconnect, this);
    dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::EncryptionRequest, this);
    dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::LoginSuccess, this);
    dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::SetCompression, this);

    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::KeepAlive, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerPositionAndLook, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Disconnect, this);

    m_Socket->SetBlocking(false);
}

Connection::~Connection() {
    GetDispatcher()->UnregisterHandler(this);
}

void Connection::HandlePacket(Minecraft::Packets::Inbound::KeepAlivePacket* packet) {
    Minecraft::Packets::Outbound::KeepAlivePacket response(packet->GetAliveId());
    SendPacket(&response);
}

void Connection::HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet) {
    using namespace Minecraft::Packets;

    // Used to verify position
    Outbound::PlayerPositionAndLookPacket response(packet->GetPosition(),
        packet->GetYaw(), packet->GetPitch(), true);

    SendPacket(&response);

    Outbound::ClientStatusPacket status(Outbound::ClientStatusPacket::Action::PerformRespawn);
    SendPacket(&status);
}

void Connection::HandlePacket(Minecraft::Packets::Inbound::DisconnectPacket* packet) {
    m_Socket->Disconnect();

    NotifyListeners(&ConnectionListener::OnSocketStateChange, m_Socket->GetStatus());

    if (m_ProtocolState != Protocol::State::Play)
        NotifyListeners(&ConnectionListener::OnLogin, false);
}

void Connection::AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey) {
    bool success = true;
    std::string error = "";

    if (!m_Yggdrasil->IsAuthenticated()) {
        try {
            if (!m_Yggdrasil->Authenticate(m_Username, m_Password)) {
                error = "Failed to authenticate";
                success = false;
            }
        } catch (const Minecraft::YggdrasilException& e) {
            error = e.what();
            success = false;
        }
    }

    try {
        if (!m_Yggdrasil->JoinServer(serverId, sharedSecret, pubkey)) {
            success = false;
            error = "Failed to join server through Yggdrasil.";
        }
    } catch (const Minecraft::YggdrasilException& e) {
        success = false;
        error = e.what();
    }

    m_Password.clear();
    NotifyListeners(&ConnectionListener::OnAuthentication, success, error);
}

void Connection::HandlePacket(Minecraft::Packets::Inbound::EncryptionRequestPacket* packet) {
    std::string pubkey = packet->GetPublicKey();
    std::string verify = packet->GetVerifyToken();

    Minecraft::EncryptionStrategyAES* aesEncrypter = new Minecraft::EncryptionStrategyAES(pubkey, verify);
    Minecraft::Packets::Outbound::EncryptionResponsePacket* encResp = aesEncrypter->GenerateResponsePacket();

    AuthenticateClient(packet->GetServerId().GetUTF16(), aesEncrypter->GetSharedSecret(), pubkey);

    SendPacket(encResp);

    delete m_Encrypter;
    m_Encrypter = aesEncrypter;
}

void Connection::HandlePacket(Minecraft::Packets::Inbound::LoginSuccessPacket* packet) {
    m_ProtocolState = Minecraft::Protocol::State::Play;

    NotifyListeners(&ConnectionListener::OnLogin, true);

    using namespace Packets::Outbound;

    // I guess the server doesn't switch protocol state immediately
    std::async(std::launch::async, [&]{
        std::this_thread::sleep_for(std::chrono::seconds(5));

        u8 skinFlags = 0;
        skinFlags |= (int)ClientSettingsPacket::SkinPartFlags::Cape;
        skinFlags |= (int)ClientSettingsPacket::SkinPartFlags::Jacket;
        skinFlags |= (int)ClientSettingsPacket::SkinPartFlags::LeftSleeve;
        skinFlags |= (int)ClientSettingsPacket::SkinPartFlags::RightSleeve;
        skinFlags |= (int)ClientSettingsPacket::SkinPartFlags::LeftPants;
        skinFlags |= (int)ClientSettingsPacket::SkinPartFlags::RightPants;
        skinFlags |= (int)ClientSettingsPacket::SkinPartFlags::Hat;

        ClientSettingsPacket clientSettings(L"en-GB", (u8)32, ClientSettingsPacket::ChatMode::Enabled, true, skinFlags, ClientSettingsPacket::MainHand::Right);

        SendPacket(&clientSettings);
    });
}

void Connection::HandlePacket(Minecraft::Packets::Inbound::SetCompressionPacket* packet) {
    delete m_Compressor;
    m_Compressor = new Minecraft::CompressionZ(packet->GetMaxPacketSize());
}

bool Connection::Connect(const std::string& server, u16 port) {
    bool result = false;

    m_Socket = std::make_shared<Network::TCPSocket>();
    m_Socket->SetBlocking(false);
    m_Yggdrasil = std::unique_ptr<Yggdrasil>(new Yggdrasil());
    m_ProtocolState = Protocol::State::Handshake;


    delete m_Compressor;
    m_Compressor = new Minecraft::CompressionNone();
    delete m_Encrypter;
    m_Encrypter = new Minecraft::EncryptionStrategyNone();

    while (!m_FuturePackets.empty())
        m_FuturePackets.pop();

    m_Server = server;
    m_Port = port;

    if (isdigit(m_Server.at(0))) {
        Network::IPAddress addr(m_Server);

        result = m_Socket->Connect(addr, m_Port);
    } else {
        auto addrs = Network::Dns::Resolve(m_Server);
        if (addrs.size() == 0) return false;

        for (auto addr : addrs) {
            if (m_Socket->Connect(addr, m_Port)) {
                result = true;
                break;
            }
        }
    }

    if (result)
        NotifyListeners(&ConnectionListener::OnSocketStateChange, m_Socket->GetStatus());

    return result;
}

void Connection::Disconnect() {
    m_Socket->Disconnect();
    NotifyListeners(&ConnectionListener::OnSocketStateChange, m_Socket->GetStatus());
}

Minecraft::Packets::Packet* Connection::CreatePacketSync(Minecraft::DataBuffer& buffer) {
    std::size_t readOffset = buffer.GetReadOffset();
    Minecraft::VarInt length;

    try {
        buffer >> length;
    } catch (const std::out_of_range&) {
        // This will happen when the buffer only contains part of the VarInt, 
        // so only part of the packet was received so far.
        // The buffer read offset isn't advanced when the exception is thrown, so no need to set it back to what it was.
        return nullptr;
    }

    if (buffer.GetRemaining() < (u32)length.GetInt()) {
        // Reset the read offset back to what it was because the full packet hasn't been received yet.
        buffer.SetReadOffset(readOffset);
        return nullptr;
    }

    Minecraft::DataBuffer decompressed = m_Compressor->Decompress(buffer, length.GetInt());

    return Packets::PacketFactory::CreatePacket(m_ProtocolState, decompressed, length.GetInt());
}

std::future<Minecraft::Packets::Packet*> Connection::CreatePacket(Minecraft::DataBuffer& buffer) {
    std::size_t readOffset = buffer.GetReadOffset();
    Minecraft::VarInt length;

    try {
        buffer >> length;
    } catch (const std::out_of_range&) {
        // This will happen when the buffer only contains part of the VarInt, 
        // so only part of the packet was received so far.
        // The buffer read offset isn't advanced when the exception is thrown, so no need to set it back to what it was.
        return std::future<Minecraft::Packets::Packet*>();
    }

    if (buffer.GetRemaining() < (u32)length.GetInt()) {
        // Reset the read offset back to what it was because the full packet hasn't been received yet.
        buffer.SetReadOffset(readOffset);
        return std::future<Minecraft::Packets::Packet*>();
    }

    Minecraft::DataBuffer decompressed = m_Compressor->Decompress(buffer, length.GetInt());

    return std::async(std::launch::async, &Minecraft::Packets::PacketFactory::CreatePacket, m_ProtocolState, decompressed, length.GetInt());
}

void Connection::CreatePacket() {
    Minecraft::DataBuffer buffer;

    m_Socket->Receive(buffer, 4096);

    if (m_Socket->GetStatus() != Network::Socket::Connected) {
        NotifyListeners(&ConnectionListener::OnSocketStateChange, m_Socket->GetStatus());
        return;
    }

    //if (buffer.GetSize() == 0) return;

    m_HandleBuffer << m_Encrypter->Decrypt(buffer);

    Minecraft::Packets::Packet* packet = nullptr;
    
    do {
        try {
            if (m_ProtocolState == Protocol::State::Login) {
                Minecraft::Packets::Packet* packet = CreatePacketSync(m_HandleBuffer);

                if (packet) {
                    this->GetDispatcher()->Dispatch(packet);
                    Minecraft::Packets::PacketFactory::FreePacket(packet);
                } else {
                    break;
                }
            } else {
                std::future<Minecraft::Packets::Packet*> futurePacket = CreatePacket(m_HandleBuffer);
                if (futurePacket.valid())
                    m_FuturePackets.push(std::move(futurePacket));
                else 
                    break;
            }
            //packet = CreatePacket(m_HandleBuffer);
            /*if (packet) {
                this->GetDispatcher()->Dispatch(packet);
                Minecraft::Packets::PacketFactory::FreePacket(packet);
            } else {
                break;
            }*/
        } catch (const Minecraft::Protocol::UnfinishedProtocolException&) {
            // Ignore for now
        }
    } while (!m_HandleBuffer.IsFinished() && m_HandleBuffer.GetSize() > 0);

    if (m_HandleBuffer.IsFinished())
        m_HandleBuffer = Minecraft::DataBuffer();
    else if (m_HandleBuffer.GetReadOffset() != 0)
        m_HandleBuffer = Minecraft::DataBuffer(m_HandleBuffer, m_HandleBuffer.GetReadOffset());

    while (!m_FuturePackets.empty()) {
        auto& futurePacket = m_FuturePackets.front();
        if (!futurePacket.valid()) {
            m_FuturePackets.pop();
            continue;
        }

        std::future_status status = futurePacket.wait_for(std::chrono::milliseconds(1));

        if (status != std::future_status::ready) break;

        try {
            Minecraft::Packets::Packet* packet = futurePacket.get();
            m_FuturePackets.pop();

            if (packet) {
                this->GetDispatcher()->Dispatch(packet);
                Minecraft::Packets::PacketFactory::FreePacket(packet);
            }
        } catch (const Minecraft::Protocol::UnfinishedProtocolException&) {
            // Ignore
            m_FuturePackets.pop();
        } catch (std::exception& e) {
            m_FuturePackets.pop();
            throw e;
        }
    }
}

void Connection::Login(const std::string& username, const std::string& password) {
    Minecraft::Packets::Outbound::HandshakePacket handshake(315, m_Server, m_Port, Minecraft::Protocol::State::Login);
    SendPacket(&handshake);
    m_ProtocolState = Minecraft::Protocol::State::Login;

    m_Email = username;

    if (username.find("@") != std::string::npos) {
        m_Yggdrasil->Authenticate(username, password);

        m_Username = m_Yggdrasil->GetPlayerName();
    } else {
        m_Username = username;
    }

    m_Password = password;

    Minecraft::Packets::Outbound::LoginStartPacket loginStart(m_Username);
    SendPacket(&loginStart);
}

void Connection::SendPacket(Minecraft::Packets::Packet* packet) {
    Minecraft::DataBuffer packetBuffer = packet->Serialize();
    Minecraft::DataBuffer compressed = m_Compressor->Compress(packetBuffer);
    Minecraft::DataBuffer encrypted = m_Encrypter->Encrypt(compressed);

    m_Socket->Send(encrypted);
}

} // ns Minecraft
