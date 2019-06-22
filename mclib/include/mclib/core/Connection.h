#ifndef MCLIB_CORE_CONNECTION_H_
#define MCLIB_CORE_CONNECTION_H_

#include <mclib/common/DataBuffer.h>
#include <mclib/common/JsonFwd.h>
#include <mclib/common/Types.h>
#include <mclib/core/AuthToken.h>
#include <mclib/core/ClientSettings.h>
#include <mclib/core/Compression.h>
#include <mclib/core/Encryption.h>
#include <mclib/network/Socket.h>
#include <mclib/protocol/Protocol.h>
#include <mclib/protocol/packets/Packet.h>
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/util/ObserverSubject.h>
#include <mclib/util/Yggdrasil.h>

#include <future>
#include <memory>
#include <queue>
#include <string>

namespace mc {
namespace core {

class ConnectionListener {
public:
    virtual MCLIB_API ~ConnectionListener() {}

    virtual void MCLIB_API
    OnSocketStateChange(network::Socket::Status newStatus) {}
    virtual void MCLIB_API OnLogin(bool success) {}
    virtual void MCLIB_API OnAuthentication(bool success, std::string error) {}
    virtual void MCLIB_API OnPingResponse(const nlohmann::json& node) {}
};

class Connection : public protocol::packets::PacketHandler,
                   public util::ObserverSubject<ConnectionListener> {
private:
    std::unique_ptr<EncryptionStrategy> m_Encrypter;
    std::unique_ptr<CompressionStrategy> m_Compressor;
    std::unique_ptr<network::Socket> m_Socket;
    std::unique_ptr<util::Yggdrasil> m_Yggdrasil;
    ClientSettings m_ClientSettings;
    std::string m_Server;
    std::string m_Email;
    std::string m_Username;
    std::string m_Password;
    DataBuffer m_HandleBuffer;
    protocol::Protocol& m_Protocol;
    protocol::State m_ProtocolState;
    u16 m_Port;
    bool m_SentSettings;
    s32 m_Dimension;

    void AuthenticateClient(const std::wstring& serverId,
                            const std::string& sharedSecret,
                            const std::string& pubkey);
    protocol::packets::Packet* CreatePacket(DataBuffer& buffer);
    void SendSettingsPacket();

public:
    MCLIB_API Connection(
        protocol::packets::PacketDispatcher* dispatcher,
        protocol::Version version = protocol::Version::Minecraft_1_11_2);
    MCLIB_API ~Connection();

    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& rhs) = delete;
    Connection(Connection&& other) = delete;
    Connection& operator=(Connection&& rhs) = delete;

    util::Yggdrasil* GetYggdrasil() { return m_Yggdrasil.get(); }
    network::Socket::Status MCLIB_API GetSocketState() const;
    ClientSettings& GetSettings() noexcept { return m_ClientSettings; }
    s32 GetDimension() const noexcept { return m_Dimension; }
    protocol::State GetProtocolState() const { return m_ProtocolState; }

    void SendSettings() noexcept { m_SentSettings = false; }

    void MCLIB_API HandlePacket(protocol::packets::in::KeepAlivePacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::DisconnectPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EncryptionRequestPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::LoginSuccessPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::SetCompressionPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::JoinGamePacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::RespawnPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::UpdateHealthPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::status::ResponsePacket* packet);

    bool MCLIB_API Connect(const std::string& server, u16 port);
    void MCLIB_API Disconnect();
    void MCLIB_API CreatePacket();

    void MCLIB_API Ping();
    bool MCLIB_API Login(const std::string& username,
                         const std::string& password);
    bool MCLIB_API Login(const std::string& username, AuthToken token);

    template <typename T>
    void SendPacket(T&& packet) {
        s32 id = m_Protocol.GetPacketId(packet);
        packet.SetId(id);
        packet.SetProtocolVersion(m_Protocol.GetVersion());
        DataBuffer packetBuffer = packet.Serialize();
        DataBuffer compressed = m_Compressor->Compress(packetBuffer);
        DataBuffer encrypted = m_Encrypter->Encrypt(compressed);

        m_Socket->Send(encrypted);
    }

    template <typename T>
    void SendPacket(T* packet) {
        SendPacket(*packet);
    }
};

}  // namespace core
}  // namespace mc

#endif
