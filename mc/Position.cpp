#include "Position.h"
#include "DataBuffer.h"
#include <cmath>

namespace Minecraft {

Position::Position(s32 x, s32 y, s32 z)
    : m_X(x), m_Y(y), m_Z(z)
{
    
}

s64 Position::Encode64() const {
    return ((m_X & 0x3FFFFFF) << 38) | ((m_Y & 0xFFF) << 26) | (m_Z & 0x3FFFFFF);
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

} // ns Minecraft

std::ostream& operator<<(std::ostream& out, const Minecraft::Position& pos) {
    return out << "(" << pos.GetX() << ", " << pos.GetY() << ", " << pos.GetZ() << ")";
}
