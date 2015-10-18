#ifndef PACKETS_PACKET_HANDLER_H_
#define PACKETS_PACKET_HANDLER_H_

#include "Packet.h"

namespace Minecraft {
namespace Packets {

class PacketDispatcher;

class PacketHandler {
private:
    PacketDispatcher* m_Dispatcher;

public:
    PacketHandler(PacketDispatcher* dispatcher);
    virtual ~PacketHandler();

    PacketDispatcher* GetDispatcher();

    // Login protocol state
    virtual void HandlePacket(Inbound::DisconnectPacket* packet) { }
    virtual void HandlePacket(Inbound::EncryptionRequestPacket* packet) { }
    virtual void HandlePacket(Inbound::LoginSuccessPacket* packet) { }
    virtual void HandlePacket(Inbound::SetCompressionPacket* packet) { }

    // Play protocol state
    virtual void HandlePacket(Inbound::KeepAlivePacket* packet) { } // 0x00
    virtual void HandlePacket(Inbound::JoinGamePacket* packet) { } // 0x01

    virtual void HandlePacket(Inbound::PluginMessagePacket* packet) { } // 0x3F
};

} // ns Packets
} // ns Minecraft

#endif
