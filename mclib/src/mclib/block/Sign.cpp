#include <mclib/block/Sign.h>

namespace mc {
namespace block {

bool Sign::ImportNBT(nbt::NBT* nbt) {
    auto& root = nbt->GetRoot();
    s32 textCount = 0;

    for (auto iter = root.begin(); iter != root.end(); ++iter) {
        if (iter->second->GetName().find(L"Text") == 0 && iter->second->GetType() == nbt::TagType::String) {
            int index = iter->second->GetName()[4] - L'0' - 1;
            m_Text[index] = ((nbt::TagString*)iter->second.get())->GetValue();
            textCount++;
        }
    }

    return textCount == 4;
}

const std::wstring& Sign::GetText(std::size_t index) const {
    return m_Text[index];
}

} // ns block
} // ns mc
