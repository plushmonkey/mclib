#include <mclib/block/Chest.h>

namespace mc {
namespace block {

bool Chest::ImportNBT(nbt::NBT* nbt) {
    auto nameTag = nbt->GetTag<nbt::TagString>(L"CustomName");

    if (nameTag) {
        this->SetCustomName(nameTag->GetValue());
    }

    return InventoryBlock::ImportNBT(nbt);
}

}  // namespace block
}  // namespace mc
