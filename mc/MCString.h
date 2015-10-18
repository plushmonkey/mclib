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

    // Store MCString into the buffer
    void Serialize(DataBuffer& buffer) const;
    // Grab MCString from the buffer
    void Deserialize(DataBuffer& buffer);
};

} // ns Minecraft

#endif
