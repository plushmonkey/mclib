#ifndef PACKETS_PACKET_FACTORY_H_
#define PACKETS_PACKET_FACTORY_H_

#include "../DataBuffer.h"
#include "../Protocol.h"
#include "Packet.h"

namespace Minecraft {
namespace Packets {

class PacketFactory {
public:
    static Packet* CreatePacket(ProtocolState state, DataBuffer& data, std::size_t length);
};

} // ns Packets
} // ns Minecraft

#endif
