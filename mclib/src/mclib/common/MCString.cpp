#include <mclib/common/MCString.h>

#include <mclib/common/DataBuffer.h>
#include <mclib/common/VarInt.h>

#include <utf8.h>

namespace mc {

MCString::MCString() {

}

MCString::MCString(const std::string& str) : m_UTF16(str.begin(), str.end())
{
}

MCString::MCString(const std::wstring& str) : m_UTF16(str)
{
}

std::wstring MCString::GetUTF16() const { return m_UTF16; }
std::string MCString::GetUTF8() const {
    std::string utf8;
    utf8::utf16to8(m_UTF16.begin(), m_UTF16.end(), std::back_inserter(utf8));
    return utf8;
}

MCString MCString::FromUTF8(const std::string& utf8) {
    std::wstring utf16;
    utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    return MCString(utf16);
}

DataBuffer& operator<<(DataBuffer& out, const MCString& str) {
    std::string utf8;
    utf8::utf16to8(str.m_UTF16.begin(), str.m_UTF16.end(), std::back_inserter(utf8));

    //s32 bytes = utf8.size();
    VarInt bytes = (s32)utf8.size();
    out << bytes;
    out << utf8;

    return out;
}
DataBuffer& operator>>(DataBuffer& in, MCString& str) {
    VarInt bytes;
    in >> bytes;

    std::string utf8;
    in.ReadSome(utf8, bytes.GetInt());

    str.m_UTF16.clear();
    utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(str.m_UTF16));

    return in;
}

} // ns mc
