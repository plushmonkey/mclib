#include <mclib/block/EndGateway.h>

namespace mc {
namespace block {

bool EndGateway::ImportNBT(nbt::NBT* nbt) {
    auto ageTag = nbt->GetTag<nbt::TagLong>(L"Age");
    auto exactTag = nbt->GetTag<nbt::TagByte>(L"ExactTeleport");
    auto exitTag = nbt->GetTag<nbt::TagCompound>(L"ExitPortal");

    if (ageTag) m_Age = ageTag->GetValue();

    if (exactTag) m_ExactTeleport = exactTag->GetValue() != 0;

    if (exitTag) {
        auto xTag = exitTag->GetTag<nbt::TagInt>(L"X");
        auto yTag = exitTag->GetTag<nbt::TagInt>(L"Y");
        auto zTag = exitTag->GetTag<nbt::TagInt>(L"Z");

        if (!xTag || !yTag || !zTag) return true;

        m_Exit = Vector3i(xTag->GetValue(), yTag->GetValue(), zTag->GetValue());
    }

    return true;
}

}  // namespace block
}  // namespace mc
