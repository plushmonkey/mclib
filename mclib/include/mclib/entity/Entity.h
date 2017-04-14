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
    EntityMetadata m_Metadata;
    Vector3d m_Position;
    Vector3d m_Velocity;
    EntityId m_EntityId;
    // Stored in radians
    float m_Yaw, m_Pitch, m_HeadPitch;
    EntityId m_VehicleId;

public:
    Entity(EntityId id) noexcept : m_EntityId(id), m_VehicleId(-1) { }
    virtual ~Entity() { }

    Entity(const Entity& rhs) = default;
    Entity& operator=(const Entity& rhs) = default;
    Entity(Entity&& rhs) = default;
    Entity& operator=(Entity&& rhs) = default;

    EntityId GetEntityId() const noexcept { return m_EntityId; }
    EntityId GetVehicleId() const noexcept { return m_VehicleId; }
    const Vector3d& GetPosition() const noexcept { return m_Position; }
    const Vector3d& GetVelocity() const noexcept { return m_Velocity; }
    float GetYaw() const noexcept { return m_Yaw; }
    float GetPitch() const noexcept { return m_Pitch; }
    float GetHeadPitch() const noexcept { return m_HeadPitch; }

    const EntityMetadata& GetMetadata() const noexcept { return m_Metadata; }

    void SetPosition(const Vector3d& pos) noexcept { m_Position = pos; }
    void SetVelocity(const Vector3d& vel) noexcept { m_Velocity = vel; }
    void SetYaw(float yaw) noexcept { m_Yaw = yaw; }
    void SetPitch(float pitch) noexcept { m_Pitch = pitch; }
    void SetVehicleId(EntityId vid) noexcept { m_VehicleId = vid; }
};

typedef std::shared_ptr<Entity> EntityPtr;

} // ns entity
} // ns mc

#endif
