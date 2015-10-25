#ifndef ENTITY_H_
#define ENTITY_H_

#include "Types.h"
#include "DataBuffer.h"
#include "Slot.h"

#include <string>

namespace Minecraft {

class Entity {
private:
    Vector3d m_Position;

    /**
     * Velocity is in units of 1/8000 of a block per tick. (-1343 would move -1343/8000 = -0.167875 blocks per tick)
     */
    Vector3s m_Velocity;
    EntityId m_EntityId;
    EntityMetadata m_Metadata;
    u8 m_Yaw, m_Pitch, m_HeadPitch;


public:
    Entity(EntityId id);

    EntityId GetEntityId() const { return m_EntityId; }
    const Vector3d& GetPosition() const { return m_Position; }
    const Vector3s& GetVelocity() const { return m_Velocity; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    u8 GetHeadPitch() const { return m_HeadPitch; }

    const EntityMetadata& GetMetadata() const { return m_Metadata; }

    void SetPosition(const Vector3d& pos) { m_Position = pos; }
};

} // ns Minecraft

#endif
