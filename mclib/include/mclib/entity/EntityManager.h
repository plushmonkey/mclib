#ifndef MCLIB_ENTITY_ENTITY_MANAGER_H_
#define MCLIB_ENTITY_ENTITY_MANAGER_H_

#include <mclib/entity/Entity.h>
#include <mclib/entity/Player.h>
#include <mclib/mclib.h>
#include <mclib/protocol/packets/Packet.h>
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/util/ObserverSubject.h>

#include <array>
#include <unordered_map>

namespace mc {
namespace entity {

class EntityListener {
public:
    virtual ~EntityListener() {}

    virtual void OnPlayerSpawn(PlayerEntityPtr entity, UUID uuid) {}
    virtual void OnEntitySpawn(EntityPtr entity) {}
    virtual void OnObjectSpawn(EntityPtr entity) {}
    virtual void OnEntityDestroy(EntityPtr entity) {}
    virtual void OnEntityMove(EntityPtr entity, Vector3d oldPos,
                              Vector3d newPos) {}
};

class EntityManager : public protocol::packets::PacketHandler,
                      public util::ObserverSubject<EntityListener> {
public:
    using EntityMap = std::unordered_map<EntityId, EntityPtr>;
    using iterator = EntityMap::iterator;
    using const_iterator = EntityMap::const_iterator;

private:
    std::unordered_map<EntityId, EntityPtr> m_Entities;
    // Entity Id for the client player
    EntityId m_EntityId;
    protocol::Version m_ProtocolVersion;

public:
    MCLIB_API EntityManager(protocol::packets::PacketDispatcher* dispatcher,
                            protocol::Version protocolVersion);
    MCLIB_API ~EntityManager();

    EntityManager(const EntityManager& rhs) = delete;
    EntityManager& operator=(const EntityManager& rhs) = delete;
    EntityManager(EntityManager&& rhs) = delete;
    EntityManager& operator=(EntityManager&& rhs) = delete;

    std::shared_ptr<PlayerEntity> GetPlayerEntity() const {
        auto iter = m_Entities.find(m_EntityId);
        if (iter == m_Entities.end()) return nullptr;

        return std::dynamic_pointer_cast<PlayerEntity>(iter->second);
    }

    EntityPtr GetEntity(EntityId eid) const {
        auto iter = m_Entities.find(eid);
        if (iter == m_Entities.end()) return nullptr;
        return iter->second;
    }

    iterator begin() { return m_Entities.begin(); }
    iterator end() { return m_Entities.end(); }

    const_iterator begin() const { return m_Entities.begin(); }
    const_iterator end() const { return m_Entities.end(); }

    void MCLIB_API HandlePacket(protocol::packets::in::JoinGamePacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::SpawnPlayerPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::SpawnPaintingPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::SpawnExperienceOrbPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::SpawnGlobalEntityPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::SpawnMobPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::SpawnObjectPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::EntityPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EntityRelativeMovePacket* packet);
    void MCLIB_API HandlePacket(
        protocol::packets::in::EntityLookAndRelativeMovePacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EntityTeleportPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EntityLookPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EntityHeadLookPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EntityVelocityPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EntityMetadataPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::EntityPropertiesPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::DestroyEntitiesPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::AttachEntityPacket* packet);
};

}  // namespace entity
}  // namespace mc

#endif
