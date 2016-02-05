#include "PacketDispatcher.h"

#include "PacketDispatcher.h"
#include "PacketHandler.h"

#include <algorithm>

namespace Minecraft {
namespace Packets {

void PacketDispatcher::RegisterHandler(Minecraft::Protocol::State protocolState, PacketId id, PacketHandler* handler) {
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found == m_Handlers[type].end())
        m_Handlers[type].push_back(handler);
}

void PacketDispatcher::UnregisterHandler(Minecraft::Protocol::State protocolState, PacketId id, PacketHandler* handler) {
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found != m_Handlers[type].end())
        m_Handlers[type].erase(found);
}

void PacketDispatcher::UnregisterHandler(PacketHandler* handler) {
    for (auto& pair : m_Handlers) {
        if (pair.second.empty()) continue;

        auto state = pair.first.first;
        auto id = pair.first.second;
        PacketType type(state, id);

        m_Handlers[type].erase(std::remove(m_Handlers[type].begin(), m_Handlers[type].end(), handler), m_Handlers[type].end());
    }
}

void PacketDispatcher::Dispatch(Packet* packet) {
    if (!packet) return;

    PacketType type(packet->GetProtocolState(), packet->GetId().GetLong());
    for (PacketHandler* handler : m_Handlers[type])
        packet->Dispatch(handler);
}

} // ns Packets
} // ns Minecraft
