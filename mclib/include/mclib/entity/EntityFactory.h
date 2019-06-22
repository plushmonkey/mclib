#ifndef MCLIB_ENTITY_ENTITY_FACTORY_H_
#define MCLIB_ENTITY_ENTITY_FACTORY_H_

#include <mclib/entity/Entity.h>

namespace mc {
namespace entity {

class EntityFactory {
public:
    static Entity* CreateEntity(EntityId id, u8 type);
};

}  // namespace entity
}  // namespace mc

#endif
