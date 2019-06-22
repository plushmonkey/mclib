#include <mclib/block/Furnace.h>

namespace mc {
namespace block {

bool Furnace::ImportNBT(nbt::NBT* nbt) {
    auto nameTag = nbt->GetTag<nbt::TagString>(L"CustomName");

    if (nameTag) {
        this->SetCustomName(nameTag->GetValue());
    }

    auto burnTimeTag = nbt->GetTag<nbt::TagShort>(L"BurnTime");
    if (burnTimeTag) m_BurnTime = burnTimeTag->GetValue();

    auto cookTimeTag = nbt->GetTag<nbt::TagShort>(L"CookTime");
    if (cookTimeTag) m_CookTime = cookTimeTag->GetValue();

    auto cookTimeTotalTag = nbt->GetTag<nbt::TagShort>(L"CookTimeTotal");
    if (cookTimeTotalTag) m_CookTimeTotal = cookTimeTotalTag->GetValue();

    return InventoryBlock::ImportNBT(nbt);
}

inventory::Slot Furnace::GetSmeltingSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(0);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

inventory::Slot Furnace::GetFuelSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(1);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

inventory::Slot Furnace::GetResultSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(2);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

}  // namespace block
}  // namespace mc
