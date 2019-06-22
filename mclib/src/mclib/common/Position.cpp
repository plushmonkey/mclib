#include <mclib/common/Position.h>

#include <mclib/common/DataBuffer.h>

#include <cmath>
#include <sstream>

namespace mc {

Position::Position(s32 x, s32 y, s32 z) noexcept : m_X(x), m_Y(y), m_Z(z) {}

s64 Position::Encode64() const noexcept {
    return ((m_X & 0x3FFFFFF) << 38) | ((m_Y & 0xFFF) << 26) |
           (m_Z & 0x3FFFFFF);
}

DataBuffer& operator<<(DataBuffer& out, const Position& pos) {
    return out << pos.Encode64();
}

DataBuffer& operator>>(DataBuffer& in, Position& pos) {
    u64 val;
    in >> val;

    pos.m_X = val >> 38;
    pos.m_Y = (val >> 26) & 0xFFF;
    pos.m_Z = val << 38 >> 38;

    if (pos.m_X >= std::pow(2, 25)) pos.m_X -= (s64)std::pow(2, 26);
    if (pos.m_Y >= std::pow(2, 11)) pos.m_Y -= (s64)std::pow(2, 12);
    if (pos.m_Z >= std::pow(2, 25)) pos.m_Z -= (s64)std::pow(2, 26);

    return in;
}

std::string to_string(const Position& pos) {
    std::stringstream ss;

    ss << "(" << pos.GetX() << ", " << pos.GetY() << ", " << pos.GetZ() << ")";
    return ss.str();
}

}  // namespace mc

std::ostream& operator<<(std::ostream& out, const mc::Position& pos) {
    return out << mc::to_string(pos);
}

std::wostream& operator<<(std::wostream& out, const mc::Position& pos) {
    std::string str = mc::to_string(pos);
    std::wstring wstr(str.begin(), str.end());

    return out << wstr;
}
