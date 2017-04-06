#ifndef MCLIB_COMMON_VAR_INT_H_
#define MCLIB_COMMON_VAR_INT_H_

#include <mclib/mclib.h>
#include <mclib/common/Types.h>

#include <iosfwd>

namespace mc {

class DataBuffer;

class VarInt {
private:
    s64 m_Value;

public:
    MCLIB_API VarInt();
    MCLIB_API VarInt(s8 val);
    MCLIB_API VarInt(s16 val);
    MCLIB_API VarInt(s32 val);
    MCLIB_API VarInt(s64 val);

    s8 GetByte() const { return (s8)m_Value; }
    s16 GetShort() const { return (s16)m_Value; }
    s32 GetInt() const { return (s32)m_Value; }
    s64 GetLong() const { return m_Value; }

    // Returns how many bytes this will take up in a buffer
    std::size_t MCLIB_API GetSerializedLength() const;

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const VarInt& pos);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, VarInt& pos);
};

typedef VarInt VarLong;

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const VarInt& var);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, VarInt& var);

} // ns mc

MCLIB_API std::ostream& operator<<(std::ostream& out, const mc::VarInt& v);

#endif
