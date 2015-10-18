#ifndef VAR_INT_H_
#define VAR_INT_H_

#include "Types.h"
#include <iosfwd>

namespace Minecraft {

class DataBuffer;

class VarInt {
private:
    s64 m_Value;

public:
    VarInt();
    VarInt(s8 val);
    VarInt(s16 val);
    VarInt(s32 val);
    VarInt(s64 val);

    s8 GetByte() const { return (s8)m_Value; }
    s16 GetShort() const { return (s16)m_Value; }
    s32 GetInt() const { return (s32)m_Value; }
    s64 GetLong() const { return m_Value; }


    friend DataBuffer& operator<<(DataBuffer& out, const VarInt& pos);
    friend DataBuffer& operator>>(DataBuffer& in, VarInt& pos);
};

typedef VarInt VarLong;

DataBuffer& operator<<(DataBuffer& out, const VarInt& var);
DataBuffer& operator>>(DataBuffer& in, VarInt& var);

}

std::ostream& operator<<(std::ostream& out, const Minecraft::VarInt& v);

#endif
