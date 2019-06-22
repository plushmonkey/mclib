#include <mclib/block/Banner.h>

namespace mc {
namespace block {

bool Banner::ImportNBT(nbt::NBT* nbt) {
    auto nameTag = nbt->GetTag<nbt::TagString>(L"CustomName");

    if (nameTag) {
        this->SetCustomName(nameTag->GetValue());
    }

    auto baseTag = nbt->GetTag<nbt::TagInt>(L"Base");
    if (!baseTag) return false;

    m_Base = static_cast<DyeColor>(baseTag->GetValue());

    auto patternsTag = nbt->GetTag<nbt::TagList>(L"Patterns");
    if (!patternsTag) return true;

    for (auto iter = patternsTag->begin(); iter != patternsTag->end(); ++iter) {
        nbt::TagCompound* sectionCompound = (nbt::TagCompound*)iter->get();

        auto colorTag = sectionCompound->GetTag<nbt::TagInt>(L"Color");
        auto patternTag = sectionCompound->GetTag<nbt::TagString>(L"Pattern");

        if (!colorTag || !patternTag) return false;

        Pattern pattern;

        pattern.color = static_cast<DyeColor>(colorTag->GetValue());
        pattern.section = patternTag->GetValue();

        m_Patterns.push_back(pattern);
    }

    return true;
}

}  // namespace block
}  // namespace mc
