#ifndef MCLIB_COMMON_DATA_BUFFER_H_
#define MCLIB_COMMON_DATA_BUFFER_H_

#include <mclib/common/Common.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>

namespace mc {

class MCString;
class VarInt;

class DataBuffer {
private:
    typedef std::vector<u8> Data;
    Data m_Buffer;
    std::size_t m_ReadOffset = 0;

public:
    typedef Data::iterator iterator;
    typedef Data::const_iterator const_iterator;
    typedef Data::reference reference;
    typedef Data::const_reference const_reference;

    MCLIB_API DataBuffer();
    MCLIB_API DataBuffer(const DataBuffer& other);
    MCLIB_API DataBuffer(const DataBuffer& other, std::size_t offset);
    MCLIB_API DataBuffer(DataBuffer&& other);
    MCLIB_API DataBuffer(const std::string& str);

    MCLIB_API DataBuffer& operator=(const DataBuffer& other);
    MCLIB_API DataBuffer& operator=(DataBuffer&& other);

    template <typename T>
    void Append(T data) {
        std::size_t size = sizeof(data);
        std::size_t end_pos = m_Buffer.size();
        m_Buffer.resize(m_Buffer.size() + size);
        memcpy(&m_Buffer[end_pos], &data, size);
    }

    template <typename T>
    DataBuffer& operator<<(T data) {
        // Switch to big endian
        std::reverse((u8*)&data, (u8*)&data + sizeof(T));
        Append(data);
        return *this;
    }

    DataBuffer& operator<<(std::string data) {
        m_Buffer.insert(m_Buffer.end(), data.begin(), data.end());
        return *this;
    }

    DataBuffer& operator<<(DataBuffer& data) {
        m_Buffer.insert(m_Buffer.end(), data.begin(), data.end());
        return *this;
    }

    DataBuffer& operator<<(const DataBuffer& data) {
        m_Buffer.insert(m_Buffer.end(), data.begin(), data.end());
        return *this;
    }

    template <typename T>
    DataBuffer& operator>>(T& data) {
        assert(m_ReadOffset + sizeof(T) <= GetSize());
        data = *(T*)&m_Buffer[m_ReadOffset];
        std::reverse((u8*)&data, (u8*)&data + sizeof(T));
        m_ReadOffset += sizeof(T);
        return *this;
    }

    DataBuffer& operator>>(DataBuffer& data) {
        data.Resize(GetSize() - m_ReadOffset);
        std::copy(m_Buffer.begin() + m_ReadOffset, m_Buffer.end(),
                  data.begin());
        m_ReadOffset = m_Buffer.size();
        return *this;
    }

    DataBuffer& operator>>(std::string& data) {
        data.resize(GetSize() - m_ReadOffset);
        std::copy(m_Buffer.begin() + m_ReadOffset, m_Buffer.end(),
                  data.begin());
        m_ReadOffset = m_Buffer.size();
        return *this;
    }

    void ReadSome(char* buffer, std::size_t amount) {
        assert(m_ReadOffset + amount <= GetSize());
        std::copy_n(m_Buffer.begin() + m_ReadOffset, amount, buffer);
        m_ReadOffset += amount;
    }

    void ReadSome(u8* buffer, std::size_t amount) {
        assert(m_ReadOffset + amount <= GetSize());
        std::copy_n(m_Buffer.begin() + m_ReadOffset, amount, buffer);
        m_ReadOffset += amount;
    }

    void ReadSome(DataBuffer& buffer, std::size_t amount) {
        assert(m_ReadOffset + amount <= GetSize());
        buffer.Resize(amount);
        std::copy_n(m_Buffer.begin() + m_ReadOffset, amount, buffer.begin());
        m_ReadOffset += amount;
    }

    void ReadSome(std::string& buffer, std::size_t amount) {
        assert(m_ReadOffset + amount <= GetSize());
        buffer.resize(amount);
        std::copy_n(m_Buffer.begin() + m_ReadOffset, amount, buffer.begin());
        m_ReadOffset += amount;
    }

    void Resize(std::size_t size) { m_Buffer.resize(size); }

    void Reserve(std::size_t amount) { m_Buffer.reserve(amount); }

    void erase(iterator it) { m_Buffer.erase(it); }

    void Clear() {
        m_Buffer.clear();
        m_ReadOffset = 0;
    }

    bool IsFinished() const { return m_ReadOffset >= m_Buffer.size(); }

    std::size_t GetReadOffset() const { return m_ReadOffset; }
    void MCLIB_API SetReadOffset(std::size_t pos);

    std::string MCLIB_API ToString() const;
    std::size_t MCLIB_API GetSize() const;
    bool MCLIB_API IsEmpty() const;
    std::size_t MCLIB_API GetRemaining() const;

    iterator MCLIB_API begin();
    iterator MCLIB_API end();
    const_iterator MCLIB_API begin() const;
    const_iterator MCLIB_API end() const;

    reference operator[](Data::size_type i) { return m_Buffer[i]; }
    const_reference operator[](Data::size_type i) const { return m_Buffer[i]; }
};

MCLIB_API std::ostream& operator<<(std::ostream& os, const DataBuffer& buffer);

}  // namespace mc

#endif
