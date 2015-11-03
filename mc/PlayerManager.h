#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "Entity/EntityManager.h"
#include "Entity/Player.h"
#include "UUID.h"
#include "ObserverSubject.h"

#include <memory>
#include <string>


// TODO: Add properties, gamemode, and ping

namespace Minecraft {

class Player {
private:
    UUID m_UUID;
    std::wstring m_Name;
    PlayerEntityPtr m_Entity;

public:
    Player(UUID uuid, std::wstring name)
        : m_UUID(uuid),
          m_Name(name)
    {

    }

    std::shared_ptr<PlayerEntity> GetEntity() const { return m_Entity.lock(); }

    void SetEntity(PlayerEntityPtr entity) { m_Entity = entity; }

    const std::wstring& GetName() const { return m_Name; }
    UUID GetUUID() const { return m_UUID; }
};
typedef std::shared_ptr<Player> PlayerPtr;

class PlayerListener {
public:
    virtual ~PlayerListener() { }

    // Called when a PlayerPositionAndLook packet is received (when client spawns or is teleported by server). The player's position is already updated in the entity.
    virtual void OnClientSpawn(PlayerPtr player) { }
    // Called when a player joins the server.
    virtual void OnPlayerJoin(PlayerPtr player) { }
    // Called when a player leaves the server
    virtual void OnPlayerLeave(PlayerPtr player) { }
    // Called when a player comes within visible range of the client
    virtual void OnPlayerSpawn(PlayerPtr player) { }
    // Called when a player leaves visible range of the client
    // The PlayerPtr entity is already set to null at this point.
    // The entity still exists in the entity manager, which can be grabbed by the entity id
    virtual void OnPlayerDestroy(PlayerPtr player, EntityId eid) { }
    // Called when a player changes position. Isn't called when player only rotates.
    virtual void OnPlayerMove(PlayerPtr player, Vector3d oldPos, Vector3d newPos) { }
};

class PlayerManager : public Packets::PacketHandler, public EntityListener, public ObserverSubject<PlayerListener> {
public:
    typedef std::map<UUID, PlayerPtr> PlayerList;
    typedef PlayerList::iterator iterator;

private:
    PlayerList m_Players;
    EntityManager* m_EntityManager;
    UUID m_ClientUUID;

public:
    PlayerManager(Packets::PacketDispatcher* dispatcher, EntityManager* entityManager);
    ~PlayerManager();

    iterator begin();
    iterator end();

    // Gets a player by their UUID. Fast method, just requires map lookup.
    PlayerPtr GetPlayerByUUID(UUID uuid) const;
    // Gets a player by their EntityId. Somewhat slow method, has to loop through player map to find the player with that eid.
    // It should still be pretty fast though since there aren't many players on a server usually.
    PlayerPtr GetPlayerByEntityId(EntityId eid) const;

    void OnPlayerSpawn(PlayerEntityPtr entity, UUID uuid);
    void OnEntityDestroy(EntityPtr entity);
    void OnEntityMove(EntityPtr entity, Vector3d oldPos, Vector3d newPos);
    void HandlePacket(Packets::Inbound::LoginSuccessPacket* packet);
    void HandlePacket(Packets::Inbound::PlayerPositionAndLookPacket* packet);
    void HandlePacket(Packets::Inbound::PlayerListItemPacket* packet);
};

} // ns Minecraft

#endif
