#include "PacketDispatcher.h"

#include "PacketDispatcher.h"
#include "PacketHandler.h"

#include <algorithm>

namespace Minecraft {
namespace Packets {

void PacketDispatcher::RegisterHandler(Minecraft::ProtocolState protocolState, PacketId id, PacketHandler* handler) {
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found == m_Handlers[type].end())
        m_Handlers[type].push_back(handler);
}

void PacketDispatcher::UnregisterHandler(Minecraft::ProtocolState protocolState, PacketId id, PacketHandler* handler) {
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found != m_Handlers[type].end())
        m_Handlers[type].erase(found);
}

void PacketDispatcher::UnregisterHandler(PacketHandler* handler) {
    for (auto& pair : m_Handlers)
        UnregisterHandler(pair.first.first, pair.first.second, handler);
}

void PacketDispatcher::Dispatch(Packet* packet) {
    if (!packet) return;

    PacketType type(packet->GetProtocolState(), packet->GetId().GetLong());
    for (PacketHandler* handler : m_Handlers[type])
        packet->Dispatch(handler);
}

} // ns Packets
} // ns Minecraft
