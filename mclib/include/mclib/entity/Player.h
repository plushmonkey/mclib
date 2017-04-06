#ifndef MCLIB_ENTITY_ENTITY_PLAYER_H_
#define MCLIB_ENTITY_ENTITY_PLAYER_H_

#include <mclib/entity/LivingEntity.h>

namespace mc {
namespace entity {

class PlayerEntity : public LivingEntity {
public:
    PlayerEntity(EntityId id) : LivingEntity(id) { }
};

typedef std::weak_ptr<PlayerEntity> PlayerEntityPtr;

} // ns entity
} // ns mc

#endif
