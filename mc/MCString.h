#ifndef MC_STRING_H_
#define MC_STRING_H_

#include "Types.h"

#include <string>

namespace Minecraft {

class DataBuffer;

class MCString {
private:
    std::wstring m_UTF16;

public:
    MCString();
    MCString(const std::string& str);
    MCString(const std::wstring& str);

    std::wstring GetUTF16() const;

    friend DataBuffer& operator<<(DataBuffer& out, const MCString& str);
    friend DataBuffer& operator>>(DataBuffer& in, MCString& str);
};

DataBuffer& operator<<(DataBuffer& out, const MCString& pos);
DataBuffer& operator>>(DataBuffer& in, MCString& pos);

} // ns Minecraft

#endif
