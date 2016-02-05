#ifndef LIVING_ENTITY_H_
#define LIVING_ENTITY_H_

#include "Entity.h"

namespace Minecraft {

class LivingEntity : public Entity {
private:
    float m_Health;

public:
    LivingEntity(EntityId id) : Entity(id) { }

    float GetHealth() const { return m_Health; }
};

} // ns Minecraft

#endif
