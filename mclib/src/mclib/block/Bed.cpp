#include <mclib/block/Bed.h>

namespace mc {
namespace block {

bool Bed::ImportNBT(nbt::NBT* nbt) {
    auto colorTag = nbt->GetTag<nbt::TagInt>(L"color");

    if (colorTag) m_Color = colorTag->GetValue();

    return true;
}

}  // namespace block
}  // namespace mc
