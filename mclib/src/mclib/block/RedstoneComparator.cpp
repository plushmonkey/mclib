#include <mclib/block/RedstoneComparator.h>

namespace mc {
namespace block {

bool RedstoneComparator::ImportNBT(nbt::NBT* nbt) {
    auto outputSignalTag = nbt->GetTag<nbt::TagInt>(L"OutputSignal");

    if (outputSignalTag)
        m_OutputSignal = outputSignalTag->GetValue();

    return true;
}

} // ns block
} // ns mc
