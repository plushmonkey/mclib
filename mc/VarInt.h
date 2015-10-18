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

    void Serialize(DataBuffer& buffer) const;
    void Deserialize(DataBuffer& buffer);
};

typedef VarInt VarLong;

}

std::ostream& operator<<(std::ostream& out, const Minecraft::VarInt& v);

#endif
