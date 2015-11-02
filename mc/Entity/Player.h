#ifndef ENTITY_PLAYER_H_
#define ENTITY_PLAYER_H_

#include "LivingEntity.h"

namespace Minecraft {

class PlayerEntity : public LivingEntity {
public:
    PlayerEntity(EntityId id) : LivingEntity(id) { }
};

typedef std::weak_ptr<PlayerEntity> PlayerEntityPtr;

} // ns Minecraft

#endif
