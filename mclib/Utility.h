#ifndef UTILITY_H
#define UTILITY_H

#include <json/json.h>
#include <string>
#include "PlayerManager.h"
#include "Connection.h"
#include "World.h"
#include "Vector.h"
#include "AABB.h"
#include <queue>

MCLIB_API std::string ParseChatNode(Json::Value node);
MCLIB_API std::string StripChatMessage(const std::string& message);

class PlayerController : public Minecraft::PlayerListener {
private:
    Minecraft::PlayerManager& m_PlayerManager;
    Minecraft::Connection* m_Connection;
    Minecraft::World& m_World;
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;
    AABB m_BoundingBox;
    Minecraft::EntityId m_EntityId;
    u64 m_LastUpdate;
    Vector3d m_TargetPos;
    bool m_Sprinting;
    bool m_LoadedIn;

    double m_MoveSpeed;

    std::queue<Vector3d> m_DigQueue;

    // todo: gravity
    const float FallSpeed = 8.3f * (50.0f / 1000.0f);

public:
    MCLIB_API PlayerController(Minecraft::Connection* connection, Minecraft::World& world, Minecraft::PlayerManager& playerManager);
    MCLIB_API ~PlayerController();

    void MCLIB_API OnClientSpawn(Minecraft::PlayerPtr player);
    bool MCLIB_API ClearPath(Vector3d target);
    
    void MCLIB_API Dig(Vector3d target);
    void MCLIB_API Attack(Minecraft::EntityId id);
    void MCLIB_API Move(Vector3d delta);

    bool MCLIB_API HandleJump();
    bool MCLIB_API HandleFall();
    void MCLIB_API UpdateDigging();
    void MCLIB_API UpdatePosition();
    void MCLIB_API Update();

    Vector3d MCLIB_API GetPosition() const;
    Vector3d MCLIB_API GetHeading() const;
    float MCLIB_API GetYaw() const;
    float MCLIB_API GetPitch() const;
    AABB MCLIB_API GetBoundingBox() const;

    void MCLIB_API SetYaw(float yaw);
    void MCLIB_API SetPitch(float pitch);
    void MCLIB_API LookAt(Vector3d target);
    void MCLIB_API SetMoveSpeed(double speed);
    void MCLIB_API SetTargetPosition(Vector3d target);

};

#endif // UTILITY_H

