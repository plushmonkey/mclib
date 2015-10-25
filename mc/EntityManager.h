#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

#include "Entity.h"
#include "Packets/Packet.h"
#include "Packets/PacketHandler.h"

#include <array>

namespace Minecraft {

class EntityManager : public Packets::PacketHandler {
private:
    std::map<EntityId, Entity*> m_Entities;
    Packets::PacketDispatcher& m_Dispatcher;

public:
    EntityManager(Packets::PacketDispatcher& dispatcher)
        : m_Dispatcher(dispatcher),
        Packets::PacketHandler(&m_Dispatcher)
    {

    }

    void HandlePacket(Packets::Inbound::SpawnPlayerPacket* packet) {
        EntityId id = packet->GetEntityId();

        Entity* entity = new Entity(id);

        m_Entities[id] = entity;

        entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));

    }

    void HandlePacket(Packets::Inbound::SpawnMobPacket* packet) {
        EntityId id = packet->GetEntityId();

        Entity* entity = new Entity(id);

        m_Entities[id] = entity;
        entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));
    }

    void HandlePacket(Packets::Inbound::EntityPacket* packet) {

    }

    void HandlePacket(Packets::Inbound::EntityRelativeMovePacket* packet) {
        EntityId id = packet->GetEntityId();

        Vector3d delta(packet->GetDeltaX(), packet->GetDeltaY(), packet->GetDeltaZ());
        Vector3d newPos = m_Entities[id]->GetPosition() + delta;
        m_Entities[id]->SetPosition(newPos);
    }

    void HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet) {

    }

    void HandlePacket(Packets::Inbound::EntityMetadataPacket* packet) {
        EntityId id = packet->GetEntityId();

    }

    void HandlePacket(Packets::Inbound::SetExperiencePacket* packet) {

    }

    void HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet) {

    }
};

} // ns Minecraft

#endif
