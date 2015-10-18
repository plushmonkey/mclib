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

void VarInt::Serialize(DataBuffer& buffer) const {
    u64 uval = u64(m_Value < 0 ? ~(m_Value << 1) : m_Value);
    
    int encoded = 0;
    char data[10];

    do {
        u8 nextByte = uval & 0x7F;
        uval >>= 7;
        if (uval)
            nextByte |= 0x80;
        data[encoded++] = nextByte;
    } while (uval);
    buffer << std::string(data, encoded);
}

void VarInt::Deserialize(DataBuffer& buffer) {
    u64 value = 0;
    int shift = 0;

    if (buffer.IsFinished()) {
        m_Value = 0;
        return;
    }

    int i = buffer.GetReadOffset();
    do {
        value |= (u64)(buffer[i] & 0x7F) << shift;
        shift += 7;
    } while ((buffer[i++] & 0x80) != 0);
    buffer.SetReadOffset(i);
    if (value & (1<<31))
        m_Value = ~(value >> 1);
    else
        m_Value = value;
    //m_Value = (s64)(value & 1 ? ~(value >> 1) : (value >> 1));
}

}

#include <ostream>

std::ostream& operator<<(std::ostream& out, const Minecraft::VarInt& v) {
    return out << v.GetLong();
}
