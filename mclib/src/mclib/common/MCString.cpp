#include <mclib/common/MCString.h>

#include <mclib/common/DataBuffer.h>
#include <mclib/common/VarInt.h>
#include <codecvt>
#include <locale>

namespace mc {

MCString::MCString() {}

MCString::MCString(const std::string& str) : m_UTF16(str.begin(), str.end()) {}

MCString::MCString(const std::wstring& str) : m_UTF16(str) {}

std::wstring MCString::GetUTF16() const { return m_UTF16; }
std::string MCString::GetUTF8() const { return utf16to8(m_UTF16); }

MCString MCString::FromUTF8(const std::string& utf8) {
    return MCString(utf8to16(utf8));
}

DataBuffer& operator<<(DataBuffer& out, const MCString& str) {
    std::string utf8 = utf16to8(str.m_UTF16);

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

    str.m_UTF16 = utf8to16(utf8);

    return in;
}

std::string utf16to8(std::wstring str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

std::wstring utf8to16(std::string str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

}  // namespace mc
