#ifndef MCLIB_CORE_CONNECTION_H_
#define MCLIB_CORE_CONNECTION_H_

#include <mclib/common/DataBuffer.h>
#include <mclib/common/Types.h>
#include <mclib/core/ClientSettings.h>
#include <mclib/network/Socket.h>
#include <mclib/protocol/packets/Packet.h>
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/util/ObserverSubject.h>
#include <mclib/util/Yggdrasil.h>

#include <string>
#include <queue>
#include <future>
#include <memory>

namespace mc {
namespace core {

struct EncryptionStrategy;
class CompressionStrategy;

class ConnectionListener {
public:
    virtual MCLIB_API ~ConnectionListener() { }

    virtual void MCLIB_API OnSocketStateChange(network::Socket::Status newStatus) { }
    virtual void MCLIB_API OnLogin(bool success) { }
    virtual void MCLIB_API OnAuthentication(bool success, std::string error) { }
    virtual void MCLIB_API OnPingResponse(std::wstring response) { }
};

class Connection : public protocol::packets::PacketHandler, public util::ObserverSubject<ConnectionListener> {
private:
    std::unique_ptr<EncryptionStrategy> m_Encrypter;
    std::unique_ptr<CompressionStrategy> m_Compressor;
    std::unique_ptr<network::Socket> m_Socket;
    std::unique_ptr<util::Yggdrasil> m_Yggdrasil;
    std::queue<std::future<protocol::packets::Packet*>> m_FuturePackets;
    ClientSettings m_ClientSettings;
    std::string m_Server;
    std::string m_Email;
    std::string m_Username;
    std::string m_Password;
    DataBuffer m_HandleBuffer;
    protocol::State m_ProtocolState;
    protocol::Version m_Version;
    u16 m_Port;
    bool m_SentSettings;

    void AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey);
    std::future<protocol::packets::Packet*> CreatePacket(DataBuffer& buffer);
    protocol::packets::Packet* CreatePacketSync(DataBuffer& buffer);
    void SendSettingsPacket();

public:
    MCLIB_API Connection(protocol::packets::PacketDispatcher* dispatcher, protocol::Version version = protocol::Version::Minecraft_1_11_2);
    MCLIB_API ~Connection();

    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& rhs) = delete;
    Connection(Connection&& other) = delete;
    Connection& operator=(Connection&& rhs) = delete;

    util::Yggdrasil* GetYggdrasil() { return m_Yggdrasil.get(); }
    network::Socket::Status MCLIB_API GetSocketState() const;
    ClientSettings& GetSettings() noexcept { return m_ClientSettings; }

    void SendSettings() noexcept { m_SentSettings = false; }

    void MCLIB_API HandlePacket(protocol::packets::in::KeepAlivePacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::DisconnectPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::EncryptionRequestPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::LoginSuccessPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::SetCompressionPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::UpdateHealthPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::status::ResponsePacket* packet);

    bool MCLIB_API Connect(const std::string& server, u16 port);
    void MCLIB_API Disconnect();
    void MCLIB_API CreatePacket();

    void MCLIB_API Ping();
    void MCLIB_API Login(const std::string& username, const std::string& password);
    void MCLIB_API SendPacket(protocol::packets::Packet* packet);
};

} // ns core
} // ns mc

#endif
