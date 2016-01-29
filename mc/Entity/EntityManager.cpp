#include "EntityManager.h"
#include "../Packets/PacketDispatcher.h"

#include <algorithm>

// todo: Use factories to create the entities

namespace Minecraft {

EntityManager::EntityManager(Packets::PacketDispatcher* dispatcher)
    : Packets::PacketHandler(dispatcher)
{
    using namespace Minecraft;

    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::JoinGame, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerPositionAndLook, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnPlayer, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnMob, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnObject, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::Entity, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityRelativeMove, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityLookAndRelativeMove, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityTeleport, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityLook, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityHeadLook, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityMetadata, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityVelocity, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityProperties, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::DestroyEntities, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::AttachEntity, this);
}

EntityManager::~EntityManager() {
    GetDispatcher()->UnregisterHandler(this);
}

void EntityManager::HandlePacket(Packets::Inbound::AttachEntityPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityId vid = packet->GetVehicleId();

    auto entity = m_Entities[eid];
    if (entity)
        entity->SetVehicleId(vid);
}

void EntityManager::HandlePacket(Packets::Inbound::JoinGamePacket* packet) {
    EntityId id = packet->GetEntityId();

    m_EntityId = id;

    std::shared_ptr<PlayerEntity> entity = std::make_shared<PlayerEntity>(id);

    m_Entities[id] = entity;
}

void EntityManager::HandlePacket(Packets::Inbound::PlayerPositionAndLookPacket* packet) {
    m_Entities.at(m_EntityId)->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));
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

void EntityManager::HandlePacket(Packets::Inbound::SpawnObjectPacket* packet) {
    EntityId id = packet->GetEntityId();

    EntityPtr entity = std::make_shared<Entity>(id);

    m_Entities[id] = entity;
    entity->SetPosition(ToVector3d(packet->GetPosition()));
    //

    NotifyListeners(&EntityListener::OnObjectSpawn, entity);
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

void EntityManager::HandlePacket(Packets::Inbound::EntityVelocityPacket* packet) {
    Minecraft::EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    iter->second->SetVelocity(packet->GetVelocity());
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

        entity->SetPosition(newPos);
        entity->SetYaw(packet->GetYaw());
        entity->SetPitch(packet->GetPitch());

        NotifyListeners(&EntityListener::OnEntityMove, m_Entities[id], oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityTeleportPacket* packet) {
    EntityId id = packet->GetEntityId();

    auto entity = m_Entities[id];
    if (entity) {
        Vector3d oldPos = m_Entities[id]->GetPosition();
        Vector3d newPos = ToVector3d(packet->GetPosition());

        entity->SetPosition(newPos);
        entity->SetYaw(packet->GetYaw());
        entity->SetPitch(packet->GetPitch());

        NotifyListeners(&EntityListener::OnEntityMove, entity, oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityLookPacket* packet) {
    u8 pitch = packet->GetPitch();
    u8 yaw = packet->GetYaw();
    Minecraft::EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    iter->second->SetYaw(yaw);
    iter->second->SetPitch(pitch);
}

void EntityManager::HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet) {
    u8 yaw = packet->GetYaw();
    Minecraft::EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    iter->second->SetYaw(yaw);
}

void EntityManager::HandlePacket(Packets::Inbound::EntityMetadataPacket* packet) {
    EntityId id = packet->GetEntityId();

}

void EntityManager::HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet) {

}

} // ns Minecraft
