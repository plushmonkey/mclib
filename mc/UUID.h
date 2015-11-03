#ifndef UUID_H_
#define UUID_H_

#include "Types.h"

#include <iosfwd>
#include <string>

namespace Minecraft {

class DataBuffer;

class UUID {
private:
    u64 m_MostSigBits;
    u64 m_LeastSigBits;

public:
    UUID() : m_MostSigBits(0), m_LeastSigBits(0) { }
    UUID(u64 most, u64 least) : m_MostSigBits(most), m_LeastSigBits(least) { }

    std::string ToString() const;

    u64 GetUpperBits() const { return m_MostSigBits; }
    u64 GetLowerBits() const { return m_LeastSigBits; }

    static UUID FromString(const std::wstring& str);

    bool operator<(const UUID& r) const {
        const UUID& l = *this;
        if (l.GetUpperBits() < r.GetUpperBits()) return true;
        if (l.GetUpperBits() > r.GetUpperBits()) return false;
        if (l.GetLowerBits() < r.GetLowerBits()) return true;
        if (l.GetLowerBits() > r.GetLowerBits()) return false;
        return false;
    }

    friend DataBuffer& operator<<(DataBuffer& out, const UUID& uuid);
    friend DataBuffer& operator>>(DataBuffer& in, UUID& uuid);
};

DataBuffer& operator<<(DataBuffer& out, const UUID& uuid);
DataBuffer& operator>>(DataBuffer& in, UUID& uuid);

std::ostream& operator<<(std::ostream& out, const UUID& uuid);
std::wostream& operator<<(std::wostream& out, const UUID& uuid);

} // ns Minecraft

#endif
