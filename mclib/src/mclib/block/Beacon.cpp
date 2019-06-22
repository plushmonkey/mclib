#include <mclib/block/Beacon.h>

namespace mc {
namespace block {

bool Beacon::ImportNBT(nbt::NBT* nbt) {
    auto levelsTag = nbt->GetTag<nbt::TagInt>(L"Levels");
    if (levelsTag) m_Levels = levelsTag->GetValue();

    auto primaryTag = nbt->GetTag<nbt::TagInt>(L"Primary");
    if (primaryTag) m_Primary = primaryTag->GetValue();

    auto secondaryTag = nbt->GetTag<nbt::TagInt>(L"Secondary");
    if (secondaryTag) m_Secondary = secondaryTag->GetValue();

    return true;
}

}  // namespace block
}  // namespace mc
