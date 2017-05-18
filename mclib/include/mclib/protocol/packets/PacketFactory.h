#ifndef PACKETS_PACKET_FACTORY_H_
#define PACKETS_PACKET_FACTORY_H_

#include <mclib/common/DataBuffer.h>
#include <mclib/protocol/Protocol.h>
#include <mclib/protocol/packets/Packet.h>

namespace mc {

namespace core {
    class Connection;
} // ns core

namespace protocol {
namespace packets {

class PacketFactory {
public:
    static MCLIB_API Packet* CreatePacket(Protocol& protocol, State state, DataBuffer data, std::size_t length, core::Connection* connection = nullptr);
    static void MCLIB_API FreePacket(Packet* packet);
};

} // ns packets
} // ns protocol
} // ns mc

#endif
