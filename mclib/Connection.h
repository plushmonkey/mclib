#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Types.h"
#include "DataBuffer.h"
#include "Packets/Packet.h"
#include "Packets/PacketHandler.h"
#include "ObserverSubject.h"
#include "Network/Socket.h"
#include "Yggdrasil.h"

#include <string>
#include <queue>
#include <future>

namespace Minecraft {

struct EncryptionStrategy;
class CompressionStrategy;

class ConnectionListener {
public:
    virtual MCLIB_API ~ConnectionListener() { }

    virtual void MCLIB_API OnSocketStateChange(Network::Socket::Status newStatus) { }
    virtual void MCLIB_API OnLogin(bool success) { }
    virtual void MCLIB_API OnAuthentication(bool success, std::string error) { }
    virtual void MCLIB_API OnPingResponse(std::wstring response) { }
};

class Connection : public Minecraft::Packets::PacketHandler, public ObserverSubject<ConnectionListener> {
private:
    Minecraft::EncryptionStrategy* m_Encrypter;
    Minecraft::CompressionStrategy* m_Compressor;
    Minecraft::Protocol::State m_ProtocolState;
    std::shared_ptr<Network::Socket> m_Socket;
    std::unique_ptr<Minecraft::Yggdrasil> m_Yggdrasil;
    std::string m_Server;
    u16 m_Port;
    std::string m_Email;
    std::string m_Username;
    std::string m_Password;
    Minecraft::DataBuffer m_HandleBuffer;
    Minecraft::Protocol::Version m_Version;

    std::queue<std::future<Minecraft::Packets::Packet*>> m_FuturePackets;

    void AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey);
    std::future<Minecraft::Packets::Packet*> CreatePacket(Minecraft::DataBuffer& buffer);
    Minecraft::Packets::Packet* CreatePacketSync(Minecraft::DataBuffer& buffer);

public:
    MCLIB_API Connection(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::Protocol::Version version = Minecraft::Protocol::Version::Minecraft_1_11_2);
    MCLIB_API ~Connection();

    Minecraft::Yggdrasil* GetYggdrasil() { return m_Yggdrasil.get(); }

    void MCLIB_API HandlePacket(Minecraft::Packets::Inbound::KeepAlivePacket* packet);
    void MCLIB_API HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet);
    void MCLIB_API HandlePacket(Minecraft::Packets::Inbound::DisconnectPacket* packet);
    void MCLIB_API HandlePacket(Minecraft::Packets::Inbound::EncryptionRequestPacket* packet);
    void MCLIB_API HandlePacket(Minecraft::Packets::Inbound::LoginSuccessPacket* packet);
    void MCLIB_API HandlePacket(Minecraft::Packets::Inbound::SetCompressionPacket* packet);

    void MCLIB_API HandlePacket(Minecraft::Packets::Inbound::Status::ResponsePacket* packet);

    bool MCLIB_API Connect(const std::string& server, u16 port);
    void MCLIB_API Disconnect();
    void MCLIB_API CreatePacket();

    void MCLIB_API Login(const std::string& username, const std::string& password);
    void MCLIB_API SendPacket(Minecraft::Packets::Packet* packet);

    void MCLIB_API Ping();
};

} // ns Minecraft

#endif
