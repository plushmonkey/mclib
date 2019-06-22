#include <mclib/common/DataBuffer.h>

#include <mclib/common/VarInt.h>

#include <iomanip>

namespace mc {

DataBuffer::DataBuffer() {}
DataBuffer::DataBuffer(const DataBuffer& other)
    : m_Buffer(other.m_Buffer), m_ReadOffset(other.m_ReadOffset) {}
DataBuffer::DataBuffer(DataBuffer&& other)
    : m_Buffer(std::move(other.m_Buffer)),
      m_ReadOffset(std::move(other.m_ReadOffset)) {}
DataBuffer::DataBuffer(const std::string& str)
    : m_Buffer(str.begin(), str.end()) {}
DataBuffer::DataBuffer(const DataBuffer& other, std::size_t offset) {
    m_Buffer.reserve(other.GetSize() - offset);
    std::copy(other.m_Buffer.begin() + offset, other.m_Buffer.end(),
              std::back_inserter(m_Buffer));
}

DataBuffer& DataBuffer::operator=(const DataBuffer& other) {
    m_Buffer = other.m_Buffer;
    m_ReadOffset = other.m_ReadOffset;
    return *this;
}

DataBuffer& DataBuffer::operator=(DataBuffer&& other) {
    m_Buffer = std::move(other.m_Buffer);
    m_ReadOffset = std::move(other.m_ReadOffset);
    return *this;
}

void DataBuffer::SetReadOffset(std::size_t pos) {
    assert(pos <= GetSize());
    m_ReadOffset = pos;
}

std::string DataBuffer::ToString() const {
    return std::string(m_Buffer.begin(), m_Buffer.end());
}

std::size_t DataBuffer::GetSize() const { return m_Buffer.size(); }
bool DataBuffer::IsEmpty() const { return m_Buffer.empty(); }
std::size_t DataBuffer::GetRemaining() const {
    return m_Buffer.size() - m_ReadOffset;
}

DataBuffer::iterator DataBuffer::begin() { return m_Buffer.begin(); }
DataBuffer::iterator DataBuffer::end() { return m_Buffer.end(); }
DataBuffer::const_iterator DataBuffer::begin() const {
    return m_Buffer.begin();
}
DataBuffer::const_iterator DataBuffer::end() const { return m_Buffer.end(); }

std::ostream& operator<<(std::ostream& os, const DataBuffer& buffer) {
    for (unsigned char u : buffer)
        os << std::hex << std::setfill('0') << std::setw(2) << (int)u << " ";
    os << std::dec;
    return os;
}

}  // namespace mc
