#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

#include "Entity.h"
#include "Player.h"
#include "../Packets/Packet.h"
#include "../Packets/PacketHandler.h"
#include "../ObserverSubject.h"
#include <array>
#include <unordered_map>

namespace Minecraft {

class EntityListener {
public:
    virtual ~EntityListener() { }

    virtual void OnPlayerSpawn(PlayerEntityPtr entity, Minecraft::UUID uuid) { }
    virtual void OnEntitySpawn(EntityPtr entity) { }
    virtual void OnObjectSpawn(EntityPtr entity) { }
    virtual void OnEntityDestroy(EntityPtr entity) { }
    virtual void OnEntityMove(EntityPtr entity, Vector3d oldPos, Vector3d newPos) { }
};

class EntityManager : public Packets::PacketHandler, public ObserverSubject<EntityListener> {
private:
    std::unordered_map<EntityId, EntityPtr> m_Entities;
    // Entity Id for the client player
    EntityId m_EntityId;

public:
    EntityManager(Packets::PacketDispatcher* dispatcher);
    ~EntityManager();

    std::shared_ptr<PlayerEntity> GetPlayerEntity() const { return std::dynamic_pointer_cast<PlayerEntity>(m_Entities.at(m_EntityId)); }

    EntityPtr GetEntity(EntityId eid) const { 
        auto iter = m_Entities.find(eid);
        if (iter == m_Entities.end()) return nullptr;
        return iter->second;
    }

    void HandlePacket(Packets::Inbound::JoinGamePacket* packet);
    void HandlePacket(Packets::Inbound::PlayerPositionAndLookPacket* packet);
    void HandlePacket(Packets::Inbound::SpawnPlayerPacket* packet);
    void HandlePacket(Packets::Inbound::SpawnMobPacket* packet);
    void HandlePacket(Packets::Inbound::SpawnObjectPacket* packet);
    void HandlePacket(Packets::Inbound::EntityPacket* packet);
    void HandlePacket(Packets::Inbound::EntityRelativeMovePacket* packet);
    void HandlePacket(Packets::Inbound::EntityLookAndRelativeMovePacket* packet);
    void HandlePacket(Packets::Inbound::EntityTeleportPacket* packet);
    void HandlePacket(Packets::Inbound::EntityLookPacket* packet);
    void HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet);
    void HandlePacket(Packets::Inbound::EntityVelocityPacket* packet);
    void HandlePacket(Packets::Inbound::EntityMetadataPacket* packet);
    void HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet);
    void HandlePacket(Packets::Inbound::DestroyEntitiesPacket* packet);
    void HandlePacket(Packets::Inbound::AttachEntityPacket* packet);
};

} // ns Minecraft

#endif
