#include "EntityManager.h"
#include "../Packets/PacketDispatcher.h"

#include <algorithm>

// todo: Use factories to create the entities

#define TAU 3.14159f * 2.0f
#define DEG_TO_RAD 3.14159f / 180.0f

namespace Minecraft {

EntityManager::EntityManager(Packets::PacketDispatcher* dispatcher)
    : Packets::PacketHandler(dispatcher)
{
    using namespace Minecraft;

    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::JoinGame, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerPositionAndLook, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnPlayer, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnPainting, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnExperienceOrb, this);
    GetDispatcher()->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnGlobalEntity, this);
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

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;
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
    auto iter = m_Entities.find(m_EntityId);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;
    if (entity) {
        entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));
        entity->SetYaw(packet->GetYaw() * DEG_TO_RAD);
        entity->SetPitch(packet->GetPitch() * DEG_TO_RAD);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnPlayerPacket* packet) {
    EntityId id = packet->GetEntityId();

    std::shared_ptr<PlayerEntity> entity = std::make_shared<PlayerEntity>(id);

    m_Entities[id] = entity;
    
    entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));
    entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
    entity->SetPitch(packet->GetPitch() / 256.0f * TAU);

    UUID uuid = packet->GetUUID();

    NotifyListeners(&EntityListener::OnPlayerSpawn, PlayerEntityPtr(entity), uuid);
    NotifyListeners(&EntityListener::OnEntitySpawn, entity);
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnObjectPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(ToVector3d(packet->GetPosition()));
    entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
    entity->SetPitch(packet->GetPitch() / 256.0f * TAU);

    NotifyListeners(&EntityListener::OnObjectSpawn, entity);
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnPaintingPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(ToVector3d(packet->GetPosition()));
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnExperienceOrbPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(packet->GetPosition());
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnGlobalEntityPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(packet->GetPosition());
}

void EntityManager::HandlePacket(Packets::Inbound::SpawnMobPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));

    NotifyListeners(&EntityListener::OnEntitySpawn, entity);
}

void EntityManager::HandlePacket(Packets::Inbound::DestroyEntitiesPacket* packet) {
    std::vector<EntityId> eids = packet->GetEntityIds();

    for (EntityId eid : eids) {
        auto iter = m_Entities.find(eid);
        if (iter == m_Entities.end()) continue;

        auto entity = iter->second;
        if (entity)
            NotifyListeners(&EntityListener::OnEntityDestroy, iter->second);

        m_Entities.erase(eid);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
}

void EntityManager::HandlePacket(Packets::Inbound::EntityVelocityPacket* packet) {
    Minecraft::EntityId eid = packet->GetEntityId();
    
    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;

    if (entity) {
        Vector3s velocity = packet->GetVelocity();
        entity->SetVelocity(velocity);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityRelativeMovePacket* packet) {
    EntityId eid = packet->GetEntityId();

    Vector3d delta(packet->GetDeltaX(), packet->GetDeltaY(), packet->GetDeltaZ());

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;

    if (entity) {
        Vector3d oldPos = entity->GetPosition();
        Vector3d newPos = entity->GetPosition() + delta;

        entity->SetPosition(newPos);

        NotifyListeners(&EntityListener::OnEntityMove, entity, oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityLookAndRelativeMovePacket* packet) {
    EntityId eid = packet->GetEntityId();

    Vector3d delta = ToVector3d(packet->GetDeltaPosition());

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;

    if (entity) {
        Vector3d oldPos = entity->GetPosition();
        Vector3d newPos = entity->GetPosition() + delta;

        entity->SetPosition(newPos);
        entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
        entity->SetPitch(packet->GetPitch() / 256.0f * TAU);

        NotifyListeners(&EntityListener::OnEntityMove, entity, oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityTeleportPacket* packet) {
    EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;

    if (entity) {
        Vector3d oldPos = entity->GetPosition();
        Vector3d newPos = ToVector3d(packet->GetPosition());

        entity->SetPosition(newPos);
        entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
        entity->SetPitch(packet->GetPitch() / 256.0f * TAU);

        NotifyListeners(&EntityListener::OnEntityMove, entity, oldPos, newPos);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityLookPacket* packet) {
    Minecraft::EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;
    if (entity) {
        entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
        entity->SetPitch(packet->GetPitch() / 256.0f * TAU);
    }
}

void EntityManager::HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet) {
    Minecraft::EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;
    if (entity)
        entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
}

void EntityManager::HandlePacket(Packets::Inbound::EntityMetadataPacket* packet) {
    EntityId id = packet->GetEntityId();

}

void EntityManager::HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet) {

}

} // ns Minecraft
