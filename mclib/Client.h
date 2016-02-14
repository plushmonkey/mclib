#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"
#include "Entity/EntityManager.h"
#include "PlayerManager.h"
#include "World.h"
#include "Utility.h"
#include "ObserverSubject.h"
#include <thread>

class ClientListener {
public:
    virtual void OnTick() = 0;
};

class Client : public ObserverSubject<ClientListener>, public Minecraft::ConnectionListener {
private:
    Minecraft::Packets::PacketDispatcher* m_Dispatcher;
    Minecraft::Connection m_Connection;
    Minecraft::EntityManager m_EntityManager;
    Minecraft::PlayerManager m_PlayerManager;
    Minecraft::World m_World;
    PlayerController m_PlayerController;
    //PlayerFollower m_Follower;
    //TrackFollower m_Follower;
    bool m_Connected;
    std::thread m_UpdateThread;

public:
    MCLIB_API Client(Minecraft::Packets::PacketDispatcher* dispatcher);
    MCLIB_API ~Client();

    void MCLIB_API OnSocketStateChange(Network::Socket::Status newState);
    void MCLIB_API UpdateThread();
    void MCLIB_API Login(const std::string& host, unsigned short port, const std::string& user, const std::string& password);

    Minecraft::Packets::PacketDispatcher* GetDispatcher() { return m_Dispatcher; }
    Minecraft::Connection* GetConnection() { return &m_Connection; }
    Minecraft::EntityManager* GetEntityManager() { return &m_EntityManager; }
    Minecraft::PlayerManager* GetPlayerManager() { return &m_PlayerManager; }
    Minecraft::World* GetWorld() { return &m_World; }
    PlayerController* GetPlayerController() { return &m_PlayerController; }
};

#endif // CLIENT_H

