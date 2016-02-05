#ifndef ENTITY_FACTORY_H_
#define ENTITY_FACTORY_H_

#include "Entity.h"

namespace Minecraft {

class EntityFactory {
public:
    static Entity* CreateEntity(EntityId id, u8 type);
};

}

#endif
