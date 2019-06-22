#include <mclib/block/NoteBlock.h>

namespace mc {
namespace block {

bool NoteBlock::ImportNBT(nbt::NBT* nbt) {
    auto noteTag = nbt->GetTag<nbt::TagByte>(L"note");
    auto poweredTag = nbt->GetTag<nbt::TagByte>(L"powered");

    if (noteTag) m_Note = noteTag->GetValue();

    if (poweredTag) m_Powered = poweredTag->GetValue();

    return true;
}

}  // namespace block
}  // namespace mc
