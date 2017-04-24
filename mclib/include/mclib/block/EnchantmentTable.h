#ifndef MCLIB_BLOCK_ENCHANTMENTTABLE_H_
#define MCLIB_BLOCK_ENCHANTMENTTABLE_H_

#include <mclib/common/Nameable.h>
#include <mclib/block/BlockEntity.h>

#include <unordered_map>

namespace mc {
namespace block {

class EnchantmentTable : public BlockEntity, public Nameable {
public:
    MCLIB_API EnchantmentTable(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);
};

} // ns block
} // ns mc

#endif
