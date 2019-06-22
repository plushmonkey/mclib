#include <mclib/block/Dropper.h>

namespace mc {
namespace block {

bool Dropper::ImportNBT(nbt::NBT* nbt) {
    auto nameTag = nbt->GetTag<nbt::TagString>(L"CustomName");

    if (nameTag) {
        this->SetCustomName(nameTag->GetValue());
    }

    return InventoryBlock::ImportNBT(nbt);
}

}  // namespace block
}  // namespace mc
