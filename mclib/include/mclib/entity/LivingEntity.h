#ifndef MCLIB_ENTITY_LIVING_ENTITY_H_
#define MCLIB_ENTITY_LIVING_ENTITY_H_

#include <mclib/entity/Entity.h>

namespace mc {
namespace entity {

class LivingEntity : public Entity {
private:
    float m_Health;

public:
    LivingEntity(EntityId id) : Entity(id) { }

    float GetHealth() const { return m_Health; }
};

} // ns entity
} // ns mc

#endif
