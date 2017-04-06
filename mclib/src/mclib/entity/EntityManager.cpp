#include <mclib/entity/EntityManager.h>

#include <mclib/protocol/packets/PacketDispatcher.h>

#include <algorithm>

// todo: Use factories to create the entities

#define TAU 3.14159f * 2.0f
#define DEG_TO_RAD 3.14159f / 180.0f

namespace mc {
namespace entity {

EntityManager::EntityManager(protocol::packets::PacketDispatcher* dispatcher)
    : protocol::packets::PacketHandler(dispatcher)
{
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::JoinGame, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::PlayerPositionAndLook, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::SpawnPlayer, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::SpawnPainting, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::SpawnExperienceOrb, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::SpawnGlobalEntity, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::SpawnMob, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::SpawnObject, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::Entity, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityRelativeMove, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityLookAndRelativeMove, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityTeleport, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityLook, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityHeadLook, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityMetadata, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityVelocity, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::EntityProperties, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::DestroyEntities, this);
    GetDispatcher()->RegisterHandler(protocol::State::Play, protocol::play::AttachEntity, this);
}

EntityManager::~EntityManager() {
    GetDispatcher()->UnregisterHandler(this);
}

void EntityManager::HandlePacket(protocol::packets::in::AttachEntityPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityId vid = packet->GetVehicleId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;
    if (entity)
        entity->SetVehicleId(vid);
}

void EntityManager::HandlePacket(protocol::packets::in::JoinGamePacket* packet) {
    EntityId id = packet->GetEntityId();

    m_EntityId = id;

    std::shared_ptr<PlayerEntity> entity = std::make_shared<PlayerEntity>(id);

    m_Entities[id] = entity;
}

void EntityManager::HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet) {
    auto iter = m_Entities.find(m_EntityId);
    EntityPtr entity;

    if (iter == m_Entities.end()) {
        entity = std::make_shared<PlayerEntity>(m_EntityId);
    } else {
        entity = iter->second;
    }

    if (entity) {
        entity->SetPosition(packet->GetPosition());
        entity->SetYaw(packet->GetYaw() * DEG_TO_RAD);
        entity->SetPitch(packet->GetPitch() * DEG_TO_RAD);
    }
}

void EntityManager::HandlePacket(protocol::packets::in::SpawnPlayerPacket* packet) {
    EntityId id = packet->GetEntityId();

    std::shared_ptr<PlayerEntity> entity = std::make_shared<PlayerEntity>(id);

    m_Entities[id] = entity;

    entity->SetPosition(packet->GetPosition());
    entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
    entity->SetPitch(packet->GetPitch() / 256.0f * TAU);

    UUID uuid = packet->GetUUID();

    NotifyListeners(&EntityListener::OnPlayerSpawn, PlayerEntityPtr(entity), uuid);
    NotifyListeners(&EntityListener::OnEntitySpawn, entity);
}

void EntityManager::HandlePacket(protocol::packets::in::SpawnObjectPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(ToVector3d(packet->GetPosition()));
    entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
    entity->SetPitch(packet->GetPitch() / 256.0f * TAU);

    NotifyListeners(&EntityListener::OnObjectSpawn, entity);
}

void EntityManager::HandlePacket(protocol::packets::in::SpawnPaintingPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(ToVector3d(packet->GetPosition()));
}

void EntityManager::HandlePacket(protocol::packets::in::SpawnExperienceOrbPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(packet->GetPosition());
}

void EntityManager::HandlePacket(protocol::packets::in::SpawnGlobalEntityPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(packet->GetPosition());
}

void EntityManager::HandlePacket(protocol::packets::in::SpawnMobPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
    entity->SetPosition(packet->GetPosition());

    NotifyListeners(&EntityListener::OnEntitySpawn, entity);
}

void EntityManager::HandlePacket(protocol::packets::in::DestroyEntitiesPacket* packet) {
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

void EntityManager::HandlePacket(protocol::packets::in::EntityPacket* packet) {
    EntityId eid = packet->GetEntityId();
    EntityPtr entity = std::make_shared<Entity>(eid);

    m_Entities[eid] = entity;
}

void EntityManager::HandlePacket(protocol::packets::in::EntityVelocityPacket* packet) {
    EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;

    if (entity) {
        Vector3d velocity = ToVector3d(packet->GetVelocity()) / 8000.0;

        entity->SetVelocity(velocity);
    }
}

void EntityManager::HandlePacket(protocol::packets::in::EntityRelativeMovePacket* packet) {
    EntityId eid = packet->GetEntityId();

    Vector3d delta = ToVector3d(packet->GetDelta()) / (32.0 * 128.0);

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

void EntityManager::HandlePacket(protocol::packets::in::EntityLookAndRelativeMovePacket* packet) {
    EntityId eid = packet->GetEntityId();

    Vector3d delta = ToVector3d(packet->GetDelta()) / (32.0 * 128.0);

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

void EntityManager::HandlePacket(protocol::packets::in::EntityTeleportPacket* packet) {
    EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;

    if (entity) {
        Vector3d oldPos = entity->GetPosition();
        Vector3d newPos = packet->GetPosition();

        entity->SetPosition(newPos);
        entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
        entity->SetPitch(packet->GetPitch() / 256.0f * TAU);

        NotifyListeners(&EntityListener::OnEntityMove, entity, oldPos, newPos);
    }
}

void EntityManager::HandlePacket(protocol::packets::in::EntityLookPacket* packet) {
    EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;
    if (entity) {
        entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
        entity->SetPitch(packet->GetPitch() / 256.0f * TAU);
    }
}

void EntityManager::HandlePacket(protocol::packets::in::EntityHeadLookPacket* packet) {
    EntityId eid = packet->GetEntityId();

    auto iter = m_Entities.find(eid);
    if (iter == m_Entities.end()) return;

    auto entity = iter->second;
    if (entity)
        entity->SetYaw(packet->GetYaw() / 256.0f * TAU);
}

void EntityManager::HandlePacket(protocol::packets::in::EntityMetadataPacket* packet) {
    EntityId id = packet->GetEntityId();

}

void EntityManager::HandlePacket(protocol::packets::in::EntityPropertiesPacket* packet) {

}

} // ns entity
} // ns mc
