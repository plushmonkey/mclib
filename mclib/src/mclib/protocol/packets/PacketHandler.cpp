#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/protocol/packets/PacketDispatcher.h>

namespace mc {
namespace protocol {
namespace packets {

PacketHandler::PacketHandler(PacketDispatcher* dispatcher) : m_Dispatcher(dispatcher) { }

PacketDispatcher* PacketHandler::GetDispatcher() {
    return m_Dispatcher;
}

PacketHandler::~PacketHandler() {

}

} // ns packets
} // ns protocol
} // ns mc
