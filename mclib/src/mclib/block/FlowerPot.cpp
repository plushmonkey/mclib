#include <mclib/block/FlowerPot.h>

namespace mc {
namespace block {

bool FlowerPot::ImportNBT(nbt::NBT* nbt) {
    auto itemTag = nbt->GetTag<nbt::TagString>(L"Item");
    auto dataTag = nbt->GetTag<nbt::TagInt>(L"Data");

    if (itemTag) m_ItemId = itemTag->GetValue();

    if (dataTag) m_ItemData = dataTag->GetValue();

    return true;
}

}  // namespace block
}  // namespace mc
