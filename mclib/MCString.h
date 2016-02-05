#ifndef MC_STRING_H_
#define MC_STRING_H_

#include "Types.h"
#include "mclib.h"

#include <string>

namespace Minecraft {

class DataBuffer;

class MCString {
private:
    std::wstring m_UTF16;

public:
    MCLIB_API MCString();
    MCLIB_API MCString(const std::string& str);
    MCLIB_API MCString(const std::wstring& str);

    std::wstring MCLIB_API GetUTF16() const;
    std::string MCLIB_API GetUTF8() const;


    static MCString MCLIB_API FromUTF8(const std::string& utf8);

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const MCString& str);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, MCString& str);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const MCString& pos);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, MCString& pos);

} // ns Minecraft

#endif
