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
    in >> nbt.m_Root;
    return in;
}

} // ns nbt
} // ns mc
