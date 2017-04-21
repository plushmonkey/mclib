#include <mclib/block/Chest.h>

namespace mc {
namespace block {

bool Chest::ImportNBT(nbt::NBT* nbt) {
    auto& root = nbt->GetRoot();
    s32 textCount = 0;

    m_LootTableSeed = 0;

    for (auto iter = root.begin(); iter != root.end(); ++iter) {
        auto tag = iter->second;
        if (tag->GetName() == L"CustomName") {
            std::wstring name = ((nbt::TagString*)iter->second.get())->GetValue();
            this->SetCustomName(name);
        } else if (tag->GetName() == L"Lock") {
            m_Lock = ((nbt::TagString*)iter->second.get())->GetValue();
        } else if (tag->GetName() == L"Items") {
            nbt::TagList* itemListTag = (nbt::TagList*)iter->second.get();
            const auto& list = itemListTag->GetList();

            for (auto itemIter = list.begin(); itemIter != list.end(); ++itemIter) {
                nbt::TagCompound* itemTag = (nbt::TagCompound*)itemIter->get();

                u8 slot = 0;
                bool slotFound = false;
                for (auto itemTagIter = itemTag->begin(); itemTagIter != itemTag->end(); ++itemTagIter) {
                    if (itemTagIter->second->GetName() == L"Slot") {
                        slot = ((nbt::TagByte*)itemTagIter->second.get())->GetValue();
                        slotFound = true;
                        break;
                    }
                }

                if (!slotFound) {
                    return false;
                }

                inventory::Slot item = inventory::Slot::FromNBT(*itemTag);
                m_Items.insert(std::make_pair(slot, item));
            }
        } else if (tag->GetName() == L"LootTable") {
            m_LootTable = ((nbt::TagString*)iter->second.get())->GetValue();
        } else if (tag->GetName() == L"LootTableSeed") {
            m_LootTableSeed = ((nbt::TagLong*)iter->second.get())->GetValue();
        }

    }

    return true;
}

} // ns block
} // ns mc
