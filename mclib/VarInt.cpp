#include "VarInt.h"
#include "DataBuffer.h"

namespace Minecraft {

VarInt::VarInt() : m_Value(0) 
{

}
VarInt::VarInt(s8 val) : m_Value(val)
{

}
VarInt::VarInt(s16 val) : m_Value(val)
{

}
VarInt::VarInt(s32 val) : m_Value(val)
{

}
VarInt::VarInt(s64 val) : m_Value(val)
{

}

std::size_t VarInt::GetSerializedLength() const {
    DataBuffer buffer;
    buffer << *this;
    return buffer.GetSize();
}

DataBuffer& operator<<(DataBuffer& out, const VarInt& var) {
    u64 uval = u64(var.m_Value < 0 ? ~(var.m_Value << 1) : var.m_Value);

    int encoded = 0;
    char data[10];

    do {
        u8 nextByte = uval & 0x7F;
        uval >>= 7;
        if (uval)
            nextByte |= 0x80;
        data[encoded++] = nextByte;
    } while (uval);
    out << std::string(data, encoded);

    return out;
}

DataBuffer& operator>>(DataBuffer& in, VarInt& var) {
    u64 value = 0;
    int shift = 0;

    if (in.IsFinished()) {
        var.m_Value = 0;
        return in;
    }

    std::size_t i = in.GetReadOffset();
    if (i >= in.GetSize())
        throw std::out_of_range("Failed reading VarInt from DataBuffer.");
    do {
        value |= (u64)(in[i] & 0x7F) << shift;
        shift += 7;

        if (i >= in.GetSize())
            throw std::out_of_range("Failed reading VarInt from DataBuffer.");
    } while ((in[i++] & 0x80) != 0);
    in.SetReadOffset(i);
    if (value & (1 << 31))
        var.m_Value = ~(value >> 1);
    else
        var.m_Value = value;

    return in;
}

}

#include <ostream>

std::ostream& operator<<(std::ostream& out, const Minecraft::VarInt& v) {
    return out << v.GetLong();
}
