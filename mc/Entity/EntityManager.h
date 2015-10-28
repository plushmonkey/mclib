#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

#include "Entity.h"
#include "../Packets/Packet.h"
#include "../Packets/PacketHandler.h"

#include <array>

namespace Minecraft {

class EntityManager : public Packets::PacketHandler {
private:
    std::map<EntityId, Entity*> m_Entities;
    Packets::PacketDispatcher& m_Dispatcher;

public:
    EntityManager(Packets::PacketDispatcher& dispatcher);

    void HandlePacket(Packets::Inbound::SpawnPlayerPacket* packet);
    void HandlePacket(Packets::Inbound::SpawnMobPacket* packet);
    void HandlePacket(Packets::Inbound::EntityPacket* packet);
    void HandlePacket(Packets::Inbound::EntityRelativeMovePacket* packet);
    void HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet);
    void HandlePacket(Packets::Inbound::EntityMetadataPacket* packet);
    void HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet);
};

} // ns Minecraft

#endif
