#ifndef MCLIB_CORE_CLIENT_H
#define MCLIB_CORE_CLIENT_H

#include <mclib/mclib.h>
#include <mclib/core/Connection.h>
#include <mclib/core/PlayerManager.h>
#include <mclib/entity/EntityManager.h>
#include <mclib/network/Network.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/util/ObserverSubject.h>
#include <mclib/util/Utility.h>
#include <mclib/world/World.h>
#include <thread>

namespace mc {
namespace core {

class ClientListener {
public:
    virtual void OnTick() = 0;
};

class Client : public util::ObserverSubject<ClientListener>, public core::ConnectionListener {
private:
    protocol::packets::PacketDispatcher* m_Dispatcher;
    core::Connection m_Connection;
    entity::EntityManager m_EntityManager;
    core::PlayerManager m_PlayerManager;
    world::World m_World;
    util::PlayerController m_PlayerController;
    //PlayerFollower m_Follower;
    //TrackFollower m_Follower;
    bool m_Connected;
    std::thread m_UpdateThread;

public:
    MCLIB_API Client(protocol::packets::PacketDispatcher* dispatcher, protocol::Version version = protocol::Version::Minecraft_1_11_2);
    MCLIB_API ~Client();

    void MCLIB_API OnSocketStateChange(network::Socket::Status newState);
    void MCLIB_API UpdateThread();
    void MCLIB_API Login(const std::string& host, unsigned short port, const std::string& user, const std::string& password, bool block = false);
    void MCLIB_API Ping(const std::string& host, unsigned short port);

    protocol::packets::PacketDispatcher* GetDispatcher() { return m_Dispatcher; }
    core::Connection* GetConnection() { return &m_Connection; }
    entity::EntityManager* GetEntityManager() { return &m_EntityManager; }
    core::PlayerManager* GetPlayerManager() { return &m_PlayerManager; }
    world::World* GetWorld() { return &m_World; }
    util::PlayerController* GetPlayerController() { return &m_PlayerController; }
};

} // ns core
} // ns mc

#endif // CLIENT_H

