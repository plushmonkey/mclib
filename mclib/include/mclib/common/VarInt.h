#ifndef MCLIB_COMMON_VAR_INT_H_
#define MCLIB_COMMON_VAR_INT_H_

#include <mclib/common/Types.h>
#include <mclib/mclib.h>

#include <iosfwd>

namespace mc {

class DataBuffer;

class VarInt {
private:
    s64 m_Value;

public:
    MCLIB_API VarInt() noexcept;
    MCLIB_API VarInt(s8 val) noexcept;
    MCLIB_API VarInt(s16 val) noexcept;
    MCLIB_API VarInt(s32 val) noexcept;
    MCLIB_API VarInt(s64 val) noexcept;

    s8 GetByte() const noexcept { return (s8)m_Value; }
    s16 GetShort() const noexcept { return (s16)m_Value; }
    s32 GetInt() const noexcept { return (s32)m_Value; }
    s64 GetLong() const noexcept { return m_Value; }

    // Returns how many bytes this will take up in a buffer
    std::size_t MCLIB_API GetSerializedLength() const;

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const VarInt& pos);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, VarInt& pos);
};

typedef VarInt VarLong;

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const VarInt& var);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, VarInt& var);

}  // namespace mc

MCLIB_API std::ostream& operator<<(std::ostream& out, const mc::VarInt& v);

#endif
