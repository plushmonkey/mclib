#include <mclib/block/Hopper.h>

namespace mc {
namespace block {

bool Hopper::ImportNBT(nbt::NBT* nbt) {
    auto nameTag = nbt->GetTag<nbt::TagString>(L"CustomName");

    if (nameTag) {
        this->SetCustomName(nameTag->GetValue());
    }

    auto transferCooldownTag = nbt->GetTag<nbt::TagInt>(L"TransferCooldown");

    if (transferCooldownTag)
        m_TransferCooldown = transferCooldownTag->GetValue();

    return InventoryBlock::ImportNBT(nbt);
}

}  // namespace block
}  // namespace mc
