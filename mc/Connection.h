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

namespace Minecraft {

struct EncryptionStrategy;
class CompressionStrategy;

class ConnectionListener {
public:
    virtual ~ConnectionListener() { }

    virtual void OnSocketStateChange(Network::Socket::Status newStatus) { }
    virtual void OnLogin(bool success) { }
    virtual void OnAuthentication(bool success, std::string error) { }
};

class Connection : public Minecraft::Packets::PacketHandler, public ObserverSubject<ConnectionListener> {
private:
    Minecraft::EncryptionStrategy* m_Encrypter;
    Minecraft::CompressionStrategy* m_Compressor;
    Minecraft::Protocol::State m_ProtocolState;
    std::shared_ptr<Network::Socket> m_Socket;
    Minecraft::Yggdrasil m_Yggdrasil;
    std::string m_Server;
    u16 m_Port;
    std::string m_Username;
    std::string m_Password;
    Minecraft::DataBuffer m_HandleBuffer;

    void AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey);
    Minecraft::Packets::Packet* CreatePacket(Minecraft::DataBuffer& buffer);

public:
    Connection(Minecraft::Packets::PacketDispatcher* dispatcher);
    ~Connection();

    void HandlePacket(Minecraft::Packets::Inbound::KeepAlivePacket* packet);
    void HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet);
    void HandlePacket(Minecraft::Packets::Inbound::DisconnectPacket* packet);
    void HandlePacket(Minecraft::Packets::Inbound::EncryptionRequestPacket* packet);
    void HandlePacket(Minecraft::Packets::Inbound::LoginSuccessPacket* packet);
    void HandlePacket(Minecraft::Packets::Inbound::SetCompressionPacket* packet);

    bool Connect(const std::string& server, u16 port);
    void CreatePacket();

    void Login(const std::string& username, const std::string& password);
    void SendPacket(Minecraft::Packets::Packet* packet);
};

} // ns Minecraft

#endif
