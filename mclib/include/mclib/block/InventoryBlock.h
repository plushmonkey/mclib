#ifndef MCLIB_BLOCK_INVENTORYBLOCK_H_
#define MCLIB_BLOCK_INVENTORYBLOCK_H_

#include <mclib/inventory/Slot.h>
#include <mclib/nbt/NBT.h>

#include <string>
#include <unordered_map>

namespace mc {
namespace block {

// Stores the items that some blocks can contain, such as chests, dispensers,
// furnaces, etc. Server doesn't actually send this data. Keep it here for the
// future in case reading map files is ever needed.
class InventoryBlock {
public:
    using ItemMap = std::unordered_map<u8, inventory::Slot>;

private:
    std::wstring m_Lock;
    ItemMap m_Items;
    std::wstring m_LootTable;
    s64 m_LootTableSeed;

public:
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    const std::wstring& GetLock() const noexcept { return m_Lock; }
    const ItemMap& GetItems() const noexcept { return m_Items; }
    const std::wstring& GetLootTable() const noexcept { return m_LootTable; }
    s64 GetLootTableSeed() const noexcept { return m_LootTableSeed; }
};

}  // namespace block
}  // namespace mc

#endif
