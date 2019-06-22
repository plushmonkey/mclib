#include <mclib/block/Sign.h>

namespace mc {
namespace block {

bool Sign::ImportNBT(nbt::NBT* nbt) {
    nbt::TagString* textTags[] = {
        nbt->GetTag<nbt::TagString>(L"Text1"),
        nbt->GetTag<nbt::TagString>(L"Text2"),
        nbt->GetTag<nbt::TagString>(L"Text3"),
        nbt->GetTag<nbt::TagString>(L"Text4"),
    };

    s32 textCount = 0;
    for (s32 i = 0; i < 4; ++i) {
        if (textTags[i] != nullptr) {
            m_Text[i] = textTags[i]->GetValue();
            textCount++;
        }
    }

    // There must be 4 text lines to be considered valid.
    return textCount == 4;
}

const std::wstring& Sign::GetText(std::size_t index) const {
    return m_Text[index];
}

}  // namespace block
}  // namespace mc
