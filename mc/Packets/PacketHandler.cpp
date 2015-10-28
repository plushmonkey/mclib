#include "PacketHandler.h"
#include "PacketDispatcher.h"

namespace Minecraft {
namespace Packets {

PacketHandler::PacketHandler(PacketDispatcher* dispatcher) : m_Dispatcher(dispatcher) { }

PacketDispatcher* PacketHandler::GetDispatcher() {
    return m_Dispatcher;
}

PacketHandler::~PacketHandler() {
    
}

} // ns packets
} // ns Minecraft
