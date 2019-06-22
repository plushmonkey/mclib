#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/protocol/packets/PacketHandler.h>

namespace mc {
namespace protocol {
namespace packets {

PacketHandler::PacketHandler(PacketDispatcher* dispatcher)
    : m_Dispatcher(dispatcher) {}

PacketDispatcher* PacketHandler::GetDispatcher() { return m_Dispatcher; }

PacketHandler::~PacketHandler() {}

}  // namespace packets
}  // namespace protocol
}  // namespace mc
