#include "EntityManager.h"
#include "../Packets/PacketDispatcher.h"

#include <algorithm>

namespace Minecraft {

EntityManager::EntityManager(Packets::PacketDispatcher* dispatcher)
    : Packets::PacketHandler(dispatcher)
{
    using namespace Minecraft;

    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::JoinGame, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerPositionAndLook, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnPlayer, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnMob, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::Entity, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityRelativeMove, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityLookAndRelativeMove, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityTeleport, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityHeadLook, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityMetadata, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityProperties, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::DestroyEntities, this);
}

EntityManager::~EntityManager() {
    GetDispatcher()->UnregisterHandler(this);
}

void EntityManager::HandlePacket(Packets::Inbound::JoinGamePacket* packet) {
    EntityId id = packet->GetEntityId();

    m_EntityId = id;

    std::shared_ptr<PlayerEntity> entity = std::make_shared<PlayerEntity>(id);

    m_Entities[id] = entity;

    // Maybe do a listener call?
}

void EntityManager::HandlePacket(Packets::Inbound::PlayerPositionAndLookPacket* packet) {
    GetPlayerEntity()->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnPlayerPacket* packet) {
    EntityId id = packet->GetEntityId();

    std::shared_ptr<PlayerEntity> entity = std::make_shared<PlayerEntity>(id);

    m_Entities[id] = entity;
    
    entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));
    // todo: other data

    UUID uuid = packet->GetUUID();

    NotifyListeners(&EntityListener::OnPlayerSpawn, PlayerEntityPtr(entity), uuid);
    NotifyListeners(&EntityListener::OnEntitySpawn, entity);
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnMobPacket* packet) {
    EntityId id = packet->GetEntityId();

    EntityPtr entity = std::make_shared<Entity>(id);

    m_Entities[id] = entity;
    entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));

    NotifyListeners(&EntityListener::OnEntitySpawn, entity);
}

void EntityManager::HandlePacket(Packets::Inbound::DestroyEntitiesPacket* packet) {
    std::vector<EntityId> eids = packet->GetEntityIds();

    for (EntityId eid : eids) {
        if (!m_Entities[eid]) continue;

        NotifyListeners(&EntityListener::OnEntityDestroy, m_Entities[eid]);

        m_Entities.erase(eid);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityPacket* packet) {
    
}

void EntityManager::HandlePacket(Packets::Inbound::EntityRelativeMovePacket* packet) {
    EntityId id = packet->GetEntityId();

    Vector3d delta(packet->GetDeltaX(), packet->GetDeltaY(), packet->GetDeltaZ());

    auto entity = m_Entities[id];

    if (entity) {
        Vector3d oldPos = m_Entities[id]->GetPosition();
        Vector3d newPos = m_Entities[id]->GetPosition() + delta;

        m_Entities[id]->SetPosition(newPos);

        NotifyListeners(&EntityListener::OnEntityMove, m_Entities[id], oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityLookAndRelativeMovePacket* packet) {
    EntityId id = packet->GetEntityId();

    Vector3d delta = ToVector3d(packet->GetDeltaPosition());

    auto entity = m_Entities.at(id);
    if (entity) {
        Vector3d oldPos = m_Entities[id]->GetPosition();
        Vector3d newPos = m_Entities[id]->GetPosition() + delta;

        m_Entities[id]->SetPosition(newPos);

        NotifyListeners(&EntityListener::OnEntityMove, m_Entities[id], oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityTeleportPacket* packet) {
    EntityId id = packet->GetEntityId();

    auto entity = m_Entities[id];
    if (entity) {
        Vector3d oldPos = m_Entities[id]->GetPosition();
        Vector3d newPos = ToVector3d(packet->GetPosition());

        m_Entities[id]->SetPosition(newPos);

        NotifyListeners(&EntityListener::OnEntityMove, m_Entities[id], oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet) {

}

void EntityManager::HandlePacket(Packets::Inbound::EntityMetadataPacket* packet) {
    EntityId id = packet->GetEntityId();

}

void EntityManager::HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet) {

}

} // ns Minecraft
