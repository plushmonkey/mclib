#ifndef DATA_BUFFER_H_
#define DATA_BUFFER_H_

#include "Common.h"
#include <vector>
#include <algorithm>
#include <cstring>

namespace Minecraft {

class MCString;
class VarInt;

class DataBuffer {
private:
    typedef std::vector<u8> Data;
    Data m_Buffer;
    size_t m_ReadOffset = 0;

public:
    typedef Data::iterator iterator;
    typedef Data::const_iterator const_iterator;
    typedef Data::reference reference;
    typedef Data::const_reference const_reference;

    DataBuffer();
    DataBuffer(const DataBuffer& other);
    DataBuffer(const DataBuffer& other, std::size_t offset);
    DataBuffer(DataBuffer&& other);
    DataBuffer(const std::string& str);

    DataBuffer& operator=(const DataBuffer& other);
    DataBuffer& operator=(DataBuffer&& other);

    template <typename T>
    void Append(T data) {
        size_t size = sizeof(data);
        size_t end_pos = m_Buffer.size();
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
        data = *(T *)&m_Buffer[m_ReadOffset];
        std::reverse((u8*)&data, (u8*)&data + sizeof(T));
        m_ReadOffset += sizeof(T);
        return *this;
    }

    DataBuffer& operator>>(DataBuffer& data) {
        data.Resize(GetSize() - m_ReadOffset);
        std::copy(m_Buffer.begin() + m_ReadOffset, m_Buffer.end(), data.begin());
        m_ReadOffset = m_Buffer.size();
        return *this;
    }

    DataBuffer& operator>>(std::string& data) {
        data.resize(GetSize() - m_ReadOffset);
        std::copy(m_Buffer.begin() + m_ReadOffset, m_Buffer.end(), data.begin());
        m_ReadOffset = m_Buffer.size();
        return *this;
    }

    void ReadSome(char* buffer, std::size_t amount) {
        strncpy(buffer, (char*)&m_Buffer.at(m_ReadOffset), amount);
        m_ReadOffset += amount;
    }

    void ReadSome(u8* buffer, std::size_t amount) {
        strncpy((char*)buffer, (char*)&m_Buffer.at(m_ReadOffset), amount);
        m_ReadOffset += amount;
    }

    void ReadSome(DataBuffer& buffer, std::size_t amount) {
        buffer.Resize(amount);
        std::copy(m_Buffer.begin() + m_ReadOffset, m_Buffer.begin() + m_ReadOffset + amount, buffer.begin());
        m_ReadOffset += amount;
    }

    void ReadSome(std::string& buffer, std::size_t amount) {
        buffer.resize(amount);
        std::copy(m_Buffer.begin() + m_ReadOffset, m_Buffer.begin() + m_ReadOffset + amount, buffer.begin());
        m_ReadOffset += amount;
    }

    void Resize(std::size_t size) {
        m_Buffer.resize(size);
    }

    void erase(iterator it) {
        m_Buffer.erase(it);
    }

    void Clear() {
        m_Buffer.clear();
        m_ReadOffset = 0;
    }

    bool IsFinished() const {
        return m_ReadOffset >= m_Buffer.size();
    }

    std::size_t GetReadOffset() const { return m_ReadOffset; }
    void SetReadOffset(std::size_t pos);

    std::string ToString() const;
    std::size_t GetSize() const;
    std::size_t GetRemaining() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    reference operator[](Data::size_type i) { return m_Buffer[i]; }
    const_reference operator[](Data::size_type i) const { return m_Buffer[i]; }
};

std::ostream& operator<<(std::ostream& os, const DataBuffer& buffer);

} // ns Minecraft

#endif
