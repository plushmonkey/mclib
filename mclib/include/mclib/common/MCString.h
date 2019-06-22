#ifndef MCLIB_COMMON_MC_STRING_H_
#define MCLIB_COMMON_MC_STRING_H_

#include <mclib/common/Types.h>
#include <mclib/mclib.h>

#include <string>

namespace mc {

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

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                            const MCString& str);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, MCString& str);
};

MCLIB_API std::string utf16to8(std::wstring str);
MCLIB_API std::wstring utf8to16(std::string str);

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const MCString& pos);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, MCString& pos);

}  // namespace mc

#endif
