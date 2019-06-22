#include <mclib/block/BrewingStand.h>

namespace mc {
namespace block {

bool BrewingStand::ImportNBT(nbt::NBT* nbt) {
    auto nameTag = nbt->GetTag<nbt::TagString>(L"CustomName");

    if (nameTag) {
        this->SetCustomName(nameTag->GetValue());
    }

    auto brewTimeTag = nbt->GetTag<nbt::TagInt>(L"BrewTime");
    if (brewTimeTag) m_BrewTime = brewTimeTag->GetValue();

    auto fuelTag = nbt->GetTag<nbt::TagByte>(L"Fuel");
    if (fuelTag) m_Fuel = fuelTag->GetValue();

    return InventoryBlock::ImportNBT(nbt);
}

inventory::Slot BrewingStand::GetLeftSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(0);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

inventory::Slot BrewingStand::GetMiddleSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(1);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

inventory::Slot BrewingStand::GetRightSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(2);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

inventory::Slot BrewingStand::GetIngredientSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(3);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

inventory::Slot BrewingStand::GetFuelSlot() const noexcept {
    const auto& items = GetItems();
    auto iter = items.find(4);
    if (iter == items.end()) return inventory::Slot();
    return iter->second;
}

}  // namespace block
}  // namespace mc
