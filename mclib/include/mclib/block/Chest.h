#ifndef MCLIB_BLOCK_CHEST_H_
#define MCLIB_BLOCK_CHEST_H_

#include <mclib/common/Nameable.h>
#include <mclib/block/BlockEntity.h>

#include <unordered_map>

namespace mc {
namespace block {

class Chest : public BlockEntity, public Nameable {
public:
    using ItemMap = std::unordered_map<u8, inventory::Slot>;

private:
    std::wstring m_Lock;
    ItemMap m_Items;
    std::wstring m_LootTable;
    s64 m_LootTableSeed;

public:
    MCLIB_API Chest(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    const std::wstring& GetLock() const noexcept { return m_Lock; }
    const ItemMap& GetItems() const noexcept { return m_Items; }
    const std::wstring& GetLootTable() const noexcept { return m_LootTable; }
    s64 GetLootTableSeed() const noexcept { return m_LootTableSeed; }
};

} // ns block
} // ns mc

#endif
