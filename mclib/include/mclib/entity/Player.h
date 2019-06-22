#ifndef MCLIB_ENTITY_ENTITY_PLAYER_H_
#define MCLIB_ENTITY_ENTITY_PLAYER_H_

#include <mclib/entity/LivingEntity.h>

namespace mc {
namespace entity {

class PlayerEntity : public LivingEntity {
public:
    PlayerEntity(EntityId id, protocol::Version protocolVersion)
        : LivingEntity(id, protocolVersion) {}
};

typedef std::weak_ptr<PlayerEntity> PlayerEntityPtr;

}  // namespace entity
}  // namespace mc

#endif
