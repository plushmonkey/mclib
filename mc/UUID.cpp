#include "UUID.h"
#include "DataBuffer.h"
#include <iomanip>
#include <sstream>

namespace Minecraft {

std::string UUID::ToString() const {
    DataBuffer buffer;
    buffer << *this;

    std::stringstream out;
    std::size_t pos = 0;

    for (u32 i = 0; i < 4; ++i)
        out << std::hex << std::setfill('0') << std::setw(2) << ((int)buffer[pos + i] & 0xFF);
    out << '-';
    pos += 4;

    for (u32 j = 0; j < 3; ++j) {
        for (u32 i = 0; i < 2; ++i)
            out << std::hex << std::setfill('0') << std::setw(2) << ((int)buffer[pos + i] & 0xFF);
        out << '-';
        pos += 2;
    }

    for (u32 i = 0; i < 6; ++i)
        out << std::hex << std::setfill('0') << std::setw(2) << ((int)buffer[pos + i] & 0xFF);

    return out.str();
}

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

std::ostream& operator<<(std::ostream& out, const UUID& uuid) {
    out << uuid.ToString();
    return out;
}

std::wostream& operator<<(std::wostream& out, const UUID& uuid) {
    std::string str = uuid.ToString();
    out << std::wstring(str.begin(), str.end());
    return out;
}

} // ns Minecraft
