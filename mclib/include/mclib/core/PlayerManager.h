#ifndef MCLIB_CORE_PLAYER_MANAGER_H_
#define MCLIB_CORE_PLAYER_MANAGER_H_

#include <mclib/common/UUID.h>
#include <mclib/entity/EntityManager.h>
#include <mclib/entity/Player.h>
#include <mclib/util/ObserverSubject.h>

#include <memory>
#include <string>

// TODO: Add properties, gamemode, and ping

namespace mc {
namespace core {

class PlayerManager;

class Player {
private:
    UUID m_UUID;
    std::wstring m_Name;
    entity::PlayerEntityPtr m_Entity;

public:
    Player(UUID uuid, std::wstring name) : m_UUID(uuid), m_Name(name) {}

    std::shared_ptr<entity::PlayerEntity> GetEntity() const {
        return m_Entity.lock();
    }

    void SetEntity(entity::PlayerEntityPtr entity) { m_Entity = entity; }

    const std::wstring& GetName() const { return m_Name; }
    UUID GetUUID() const { return m_UUID; }

    friend class PlayerManager;
};
typedef std::shared_ptr<Player> PlayerPtr;

class PlayerListener {
public:
    virtual ~PlayerListener() {}

    // Called when a PlayerPositionAndLook packet is received (when client
    // spawns or is teleported by server). The player's position is already
    // updated in the entity.
    virtual void OnClientSpawn(PlayerPtr player) {}
    // Called when a player joins the server.
    virtual void OnPlayerJoin(PlayerPtr player) {}
    // Called when a player leaves the server
    virtual void OnPlayerLeave(PlayerPtr player) {}
    // Called when a player comes within visible range of the client
    virtual void OnPlayerSpawn(PlayerPtr player) {}
    // Called when a player leaves visible range of the client
    // The PlayerPtr entity is already set to null at this point.
    // The entity still exists in the entity manager, which can be grabbed by
    // the entity id
    virtual void OnPlayerDestroy(PlayerPtr player, EntityId eid) {}
    // Called when a player changes position. Isn't called when player only
    // rotates.
    virtual void OnPlayerMove(PlayerPtr player, Vector3d oldPos,
                              Vector3d newPos) {}
};

class PlayerManager : public protocol::packets::PacketHandler,
                      public entity::EntityListener,
                      public util::ObserverSubject<PlayerListener> {
public:
    typedef std::map<UUID, PlayerPtr> PlayerList;
    typedef PlayerList::iterator iterator;

private:
    PlayerList m_Players;
    entity::EntityManager* m_EntityManager;
    UUID m_ClientUUID;

public:
    MCLIB_API PlayerManager(protocol::packets::PacketDispatcher* dispatcher,
                            entity::EntityManager* entityManager);
    MCLIB_API ~PlayerManager();

    PlayerManager(const PlayerManager& rhs) = delete;
    PlayerManager& operator=(const PlayerManager& rhs) = delete;
    PlayerManager(PlayerManager&& rhs) = delete;
    PlayerManager& operator=(PlayerManager&& rhs) = delete;

    iterator MCLIB_API begin();
    iterator MCLIB_API end();

    // Gets a player by their UUID. Fast method, just requires map lookup.
    PlayerPtr MCLIB_API GetPlayerByUUID(UUID uuid) const;
    // Gets a player by their EntityId. Somewhat slow method, has to loop
    // through player map to find the player with that eid. It should still be
    // pretty fast though since there aren't many players on a server usually.
    PlayerPtr MCLIB_API GetPlayerByEntityId(EntityId eid) const;
    // Gets a player by their username.
    PlayerPtr MCLIB_API GetPlayerByName(const std::wstring& name) const;

    void MCLIB_API OnPlayerSpawn(entity::PlayerEntityPtr entity, UUID uuid);
    void MCLIB_API OnEntityDestroy(entity::EntityPtr entity);
    void MCLIB_API OnEntityMove(entity::EntityPtr entity, Vector3d oldPos,
                                Vector3d newPos);
    void MCLIB_API
    HandlePacket(protocol::packets::in::LoginSuccessPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::PlayerListItemPacket* packet);
};

}  // namespace core
}  // namespace mc

#endif
