#include <mclib/block/InventoryBlock.h>

namespace mc {
namespace block {

bool InventoryBlock::ImportNBT(nbt::NBT* nbt) {
    auto lockTag = nbt->GetTag<nbt::TagString>(L"Lock");
    auto lootTableTag = nbt->GetTag<nbt::TagString>(L"LootTable");
    auto lootTableSeedTag = nbt->GetTag<nbt::TagInt>(L"LootTableSeed");

    if (lockTag) m_Lock = lockTag->GetValue();

    if (lootTableTag) m_LootTable = lootTableTag->GetValue();

    if (lootTableSeedTag) m_LootTableSeed = lootTableSeedTag->GetValue();

    auto itemsTag = nbt->GetTag<nbt::TagList>(L"Items");
    if (itemsTag == nullptr) return false;

    for (auto iter = itemsTag->begin(); iter != itemsTag->end(); ++iter) {
        nbt::TagCompound* itemTag = (nbt::TagCompound*)iter->get();

        auto slotTag = itemTag->GetTag<nbt::TagByte>(L"Slot");
        if (!slotTag) return false;

        inventory::Slot item = inventory::Slot::FromNBT(*itemTag);
        m_Items.insert(std::make_pair(slotTag->GetValue(), item));
    }

    return true;
}

}  // namespace block
}  // namespace mc
