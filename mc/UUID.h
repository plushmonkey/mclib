#ifndef UUID_H_
#define UUID_H_

#include "Types.h"

namespace Minecraft {

class DataBuffer;

class UUID {
private:
    u64 m_MostSigBits;
    u64 m_LeastSigBits;

public:
    UUID() : m_MostSigBits(0), m_LeastSigBits(0) { }
    UUID(u64 most, u64 least) : m_MostSigBits(most), m_LeastSigBits(least) { }

    friend DataBuffer& operator<<(DataBuffer& out, const UUID& uuid);
    friend DataBuffer& operator>>(DataBuffer& in, UUID& uuid);
};

DataBuffer& operator<<(DataBuffer& out, const UUID& uuid);
DataBuffer& operator>>(DataBuffer& in, UUID& uuid);

} // ns Minecraft

#endif
