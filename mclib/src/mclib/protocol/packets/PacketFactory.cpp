#include <mclib/protocol/packets/PacketFactory.h>

#include <mclib/core/Connection.h>

#include <exception>
#include <string>
#include <iostream>

namespace mc {
namespace protocol {
namespace packets {

Packet* PacketFactory::CreatePacket(Protocol& protocol, protocol::State state, DataBuffer data, std::size_t length, core::Connection* connection) {
    if (data.GetSize() == 0) return nullptr;

    VarInt vid;
    data >> vid;

    InboundPacket* packet = protocol.CreateInboundPacket(state, vid.GetInt());

    if (packet) {
        packet->SetConnection(connection);
        try {
          packet->Deserialize(data, length);
        } catch (std::exception & e) {
          std::cerr << vid.GetInt() << ": " << e.what();
        }
    } else {
        throw protocol::UnfinishedProtocolException(vid, state);
    }

    return packet;
}

void PacketFactory::FreePacket(Packet* packet) {
    delete packet;
}

} // ns packets
} // ns protocol
} // ns mc
