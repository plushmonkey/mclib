#include "UUID.h"
#include "DataBuffer.h"

namespace Minecraft {

DataBuffer& operator<<(DataBuffer& out, const UUID& uuid) {
    out << uuid.m_MostSigBits;
    out << uuid.m_LeastSigBits;
    return out;
}

DataBuffer& operator>>(DataBuffer& in, UUID& uuid) {
    in >> uuid.m_MostSigBits;
    in >> uuid.m_LeastSigBits;
    return in;
}

} // ns Minecraft
