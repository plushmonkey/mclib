#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

#include "Entity.h"
#include "Player.h"
#include "../mclib.h"
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
    MCLIB_API EntityManager(Packets::PacketDispatcher* dispatcher);
    MCLIB_API ~EntityManager();

    std::shared_ptr<PlayerEntity> GetPlayerEntity() const { return std::dynamic_pointer_cast<PlayerEntity>(m_Entities.at(m_EntityId)); }

    EntityPtr GetEntity(EntityId eid) const { 
        auto iter = m_Entities.find(eid);
        if (iter == m_Entities.end()) return nullptr;
        return iter->second;
    }

    void MCLIB_API HandlePacket(Packets::Inbound::JoinGamePacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::PlayerPositionAndLookPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::SpawnPlayerPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::SpawnPaintingPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::SpawnExperienceOrbPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::SpawnGlobalEntityPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::SpawnMobPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::SpawnObjectPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityRelativeMovePacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityLookAndRelativeMovePacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityTeleportPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityLookPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityVelocityPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityMetadataPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::DestroyEntitiesPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::AttachEntityPacket* packet);
};

} // ns Minecraft

#endif
