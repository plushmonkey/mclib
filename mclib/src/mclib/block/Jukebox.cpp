#include <mclib/block/Jukebox.h>

namespace mc {
namespace block {

bool Jukebox::ImportNBT(nbt::NBT* nbt) {
    auto recordTag = nbt->GetTag<nbt::TagInt>(L"Record");
    auto itemTag = nbt->GetTag<nbt::TagCompound>(L"RecordItem");

    if (recordTag) m_RecordId = recordTag->GetValue();

    if (itemTag) m_RecordItem = inventory::Slot::FromNBT(*itemTag);

    return true;
}

}  // namespace block
}  // namespace mc
