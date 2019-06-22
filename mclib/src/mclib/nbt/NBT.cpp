#include <mclib/nbt/NBT.h>

#include <mclib/common/DataBuffer.h>

namespace mc {
namespace nbt {

DataBuffer& operator<<(DataBuffer& out, const NBT& nbt) {
    const auto& root = nbt.GetRoot();

    out << root;
    return out;
}

DataBuffer& operator>>(DataBuffer& in, NBT& nbt) {
    size_t offset = in.GetReadOffset();
    u8 type;
    in >> type;

    // There is no NBT data.
    if (type == 0) return in;

    in.SetReadOffset(offset);
    in >> nbt.m_Root;
    return in;
}

}  // namespace nbt
}  // namespace mc
