#include <mclib/common/VarInt.h>

#include <mclib/common/DataBuffer.h>

#include <ostream>

namespace mc {

VarInt::VarInt() noexcept : m_Value(0) {}
VarInt::VarInt(s8 val) noexcept : m_Value(val) {}
VarInt::VarInt(s16 val) noexcept : m_Value(val) {}
VarInt::VarInt(s32 val) noexcept : m_Value(val) {}
VarInt::VarInt(s64 val) noexcept : m_Value(val) {}

std::size_t VarInt::GetSerializedLength() const {
    DataBuffer buffer;
    buffer << *this;
    return buffer.GetSize();
}

DataBuffer& operator<<(DataBuffer& out, const VarInt& var) {
    u64 uval = var.m_Value;

    int encoded = 0;
    char data[10];

    do {
        u8 nextByte = uval & 0x7F;
        uval >>= 7;
        if (uval) nextByte |= 0x80;
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

    do {
        if (i >= in.GetSize())
            throw std::out_of_range("Failed reading VarInt from DataBuffer.");
        value |= (u64)(in[i] & 0x7F) << shift;
        shift += 7;
    } while ((in[i++] & 0x80) != 0);

    in.SetReadOffset(i);

    var.m_Value = value;

    return in;
}

}  // namespace mc

std::ostream& operator<<(std::ostream& out, const mc::VarInt& v) {
    return out << v.GetLong();
}
