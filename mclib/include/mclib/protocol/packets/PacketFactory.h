#ifndef PACKETS_PACKET_FACTORY_H_
#define PACKETS_PACKET_FACTORY_H_

#include <mclib/common/DataBuffer.h>
#include <mclib/protocol/Protocol.h>
#include <mclib/protocol/packets/Packet.h>

namespace mc {
namespace protocol {
namespace packets {

class PacketFactory {
public:
    static MCLIB_API Packet* CreatePacket(State state, DataBuffer data, std::size_t length);
    static void MCLIB_API FreePacket(Packet* packet);
};

} // ns packets
} // ns protocol
} // ns mc

#endif
