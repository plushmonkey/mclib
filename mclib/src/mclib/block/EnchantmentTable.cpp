#include <mclib/block/EnchantmentTable.h>

namespace mc {
namespace block {

bool EnchantmentTable::ImportNBT(nbt::NBT* nbt) {
    auto nameTag = nbt->GetTag<nbt::TagString>(L"CustomName");

    if (nameTag) {
        this->SetCustomName(nameTag->GetValue());
    }

    return true;
}

}  // namespace block
}  // namespace mc
