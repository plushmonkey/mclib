#ifndef MCLIB_ENTITY_ENTITY_H_
#define MCLIB_ENTITY_ENTITY_H_

#include <mclib/common/DataBuffer.h>
#include <mclib/common/Types.h>
#include <mclib/entity/Metadata.h>

#include <string>
#include <memory>

namespace mc {
namespace entity {

class Entity {
protected:
    Vector3d m_Position;

    /**
     * Velocity is in units of 1/8000 of a block per tick. (-1343 would move -1343/8000 = -0.167875 blocks per tick)
     */
    Vector3d m_Velocity;
    EntityId m_EntityId;
    EntityMetadata m_Metadata;

    // Stored in radians
    float m_Yaw, m_Pitch, m_HeadPitch;

    EntityId m_VehicleId;

public:
    Entity(EntityId id) : m_EntityId(id), m_VehicleId(-1) { }
    virtual ~Entity() { }

    EntityId GetEntityId() const { return m_EntityId; }
    EntityId GetVehicleId() const { return m_VehicleId; }
    const Vector3d& GetPosition() const { return m_Position; }
    const Vector3d& GetVelocity() const { return m_Velocity; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }
    float GetHeadPitch() const { return m_HeadPitch; }

    const EntityMetadata& GetMetadata() const { return m_Metadata; }

    void SetPosition(const Vector3d& pos) { m_Position = pos; }
    void SetVelocity(const Vector3d& vel) { m_Velocity = vel; }
    void SetYaw(float yaw) { m_Yaw = yaw; }
    void SetPitch(float pitch) { m_Pitch = pitch; }
    void SetVehicleId(EntityId vid) { m_VehicleId = vid; }
};

typedef std::shared_ptr<Entity> EntityPtr;

} // ns entity
} // ns mc

#endif
