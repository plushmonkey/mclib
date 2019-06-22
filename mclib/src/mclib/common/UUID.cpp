#include <mclib/common/UUID.h>

#include <mclib/common/DataBuffer.h>

#include <cassert>
#include <iomanip>
#include <sstream>

namespace mc {

std::string UUID::ToString(bool dashes) const {
    DataBuffer buffer;
    buffer << *this;

    std::stringstream out;
    std::size_t pos = 0;

    for (u32 i = 0; i < 4; ++i)
        out << std::hex << std::setfill('0') << std::setw(2)
            << ((int)buffer[pos + i] & 0xFF);
    if (dashes) out << '-';
    pos += 4;

    for (u32 j = 0; j < 3; ++j) {
        for (u32 i = 0; i < 2; ++i)
            out << std::hex << std::setfill('0') << std::setw(2)
                << ((int)buffer[pos + i] & 0xFF);
        if (dashes) out << '-';
        pos += 2;
    }

    for (u32 i = 0; i < 6; ++i)
        out << std::hex << std::setfill('0') << std::setw(2)
            << ((int)buffer[pos + i] & 0xFF);

    return out.str();
}

UUID UUID::FromString(const std::string& str, bool dashes) {
    std::wstring wstr(str.begin(), str.end());
    return UUID::FromString(wstr, dashes);
}

UUID UUID::FromString(const std::wstring& str, bool dashes) {
    if (dashes)
        assert(str.length() == 36);
    else
        assert(str.length() == 32);

    // 0ba955da-bce3-3796-b74a-9faae6cc08a2

    std::wstring upperStr, lowerStr;

    upperStr.reserve(18);
    lowerStr.reserve(18);

    upperStr = L"0x";
    lowerStr = L"0x";

    if (dashes) {
        for (u32 i = 0; i < 8; ++i) upperStr += str[i];
        for (u32 i = 0; i < 4; ++i) upperStr += str[i + 9];
        for (u32 i = 0; i < 4; ++i) upperStr += str[i + 14];

        for (u32 i = 0; i < 4; ++i) lowerStr += str[i + 19];
        for (u32 i = 0; i < 12; ++i) lowerStr += str[i + 24];
    } else {
        for (u32 i = 0; i < 16; ++i) upperStr += str[i];

        for (u32 i = 0; i < 16; ++i) lowerStr += str[i + 16];
    }

    UUID uuid;

    uuid.m_MostSigBits = std::stoull(upperStr, nullptr, 16);
    uuid.m_LeastSigBits = std::stoull(lowerStr, nullptr, 16);

    return uuid;
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

}  // namespace mc
