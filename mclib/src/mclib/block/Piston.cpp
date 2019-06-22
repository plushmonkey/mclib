#include <mclib/block/Piston.h>

namespace mc {
namespace block {

bool Piston::ImportNBT(nbt::NBT* nbt) {
    auto idTag = nbt->GetTag<nbt::TagInt>(L"blockId");
    auto dataTag = nbt->GetTag<nbt::TagInt>(L"blockData");
    auto facingTag = nbt->GetTag<nbt::TagInt>(L"facing");
    auto progressTag = nbt->GetTag<nbt::TagFloat>(L"progress");
    auto extendingTag = nbt->GetTag<nbt::TagByte>(L"extending");
    auto sourceTag = nbt->GetTag<nbt::TagByte>(L"source");

    if (idTag) m_BlockId = idTag->GetValue();

    if (dataTag) m_BlockData = dataTag->GetValue();

    if (facingTag) m_Facing = static_cast<Face>(facingTag->GetValue());

    if (progressTag) m_Progress = progressTag->GetValue();

    if (extendingTag) m_Extending = extendingTag->GetValue() != 0;

    if (sourceTag) m_Source = sourceTag->GetValue() != 0;

    return true;
}

}  // namespace block
}  // namespace mc
