#ifndef POSITION_H_
#define POSITION_H_

#include "Types.h"
#include "mclib.h"
#include <iosfwd>

namespace Minecraft {

class DataBuffer;

class Position {
private:
    s64 m_X;
    s64 m_Y;
    s64 m_Z;

public:
    Position() : m_X(0), m_Y(0), m_Z(0) { }
    MCLIB_API Position(s32 x, s32 y, s32 z);
    s64 MCLIB_API Encode64() const;

    s64 GetX() const { return m_X; }
    s64 GetY() const { return m_Y; }
    s64 GetZ() const { return m_Z; }

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Position& pos);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, Position& pos);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Position& pos);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, Position& pos);

MCLIB_API std::string to_string(const Position& data);

}

MCLIB_API std::ostream& operator<<(std::ostream& out, const Minecraft::Position& pos);
MCLIB_API std::wostream& operator<<(std::wostream& out, const Minecraft::Position& pos);

#endif
