#include <mclib/block/Banner.h>

namespace mc {
namespace block {

bool Banner::ImportNBT(nbt::NBT* nbt) {
    auto& root = nbt->GetRoot();

    for (auto iter = root.begin(); iter != root.end(); ++iter) {
        auto tag = iter->second;

        if (tag->GetName() == L"CustomName") {
            std::wstring name = ((nbt::TagString*)iter->second.get())->GetValue();
            this->SetCustomName(name);
        } else if (tag->GetName() == L"Base") {
            m_Base = static_cast<DyeColor>(((nbt::TagInt*)iter->second.get())->GetValue());
        } else if (tag->GetName() == L"Patterns") {
            nbt::TagList* listTag = (nbt::TagList*)iter->second.get();

            for (auto listIter = listTag->begin(); listIter != listTag->end(); ++listIter) {
                nbt::TagCompound* sectionCompound = (nbt::TagCompound*)listIter->get();
                Pattern pattern;

                bool hasColor = false;
                bool hasPattern = false;

                for (auto sectionIter = sectionCompound->begin(); sectionIter != sectionCompound->end(); ++sectionIter) {
                    auto sectionTag = sectionIter->second;
                    if (sectionTag->GetName() == L"Color") {
                        pattern.color = static_cast<DyeColor>(((nbt::TagInt*)sectionTag.get())->GetValue());
                        hasColor = true;
                    } else if (sectionTag->GetName() == L"Pattern") {
                        pattern.section = ((nbt::TagString*)sectionTag.get())->GetValue();
                        hasPattern = true;
                    }
                }

                if (!hasColor || !hasPattern) 
                    return false;

                m_Patterns.push_back(pattern);
            }
        }
    }

    return true;
}

} // ns block
} // ns mc
