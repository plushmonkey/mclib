#include "MCString.h"
#include "DataBuffer.h"
#include "VarInt.h"
#include <utf8.h>

namespace Minecraft {

MCString::MCString() {

}

MCString::MCString(const std::string& str) : m_UTF16(str.begin(), str.end())
{
}

MCString::MCString(const std::wstring& str) : m_UTF16(str)
{
}

std::wstring MCString::GetUTF16() const { return m_UTF16; }

void MCString::Serialize(DataBuffer& buffer) const {
    std::string utf8;
    utf8::utf16to8(m_UTF16.begin(), m_UTF16.end(), std::back_inserter(utf8));

    //s32 bytes = utf8.size();
    VarInt bytes = (s32)utf8.size();
    buffer << bytes;
    buffer << utf8;
}

void MCString::Deserialize(DataBuffer& buffer) {
    //s32 bytes;
    VarInt bytes;
    buffer >> bytes;

    std::string utf8;
    buffer.ReadSome(utf8, bytes.GetInt());

    m_UTF16.clear();
    utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(m_UTF16));
}

} // ns Minecraft
