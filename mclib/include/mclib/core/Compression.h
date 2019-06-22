#ifndef MCLIB_CORE_COMPRESSION_H_
#define MCLIB_CORE_COMPRESSION_H_

#include <mclib/common/Types.h>
#include <mclib/mclib.h>

namespace mc {

class DataBuffer;

namespace core {

class CompressionStrategy {
public:
    virtual MCLIB_API ~CompressionStrategy() {}
    virtual DataBuffer MCLIB_API Compress(DataBuffer& buffer) = 0;
    virtual DataBuffer MCLIB_API Decompress(DataBuffer& buffer,
                                            std::size_t packetLength) = 0;
};

class CompressionNone : public CompressionStrategy {
public:
    DataBuffer MCLIB_API Compress(DataBuffer& buffer);
    DataBuffer MCLIB_API Decompress(DataBuffer& buffer,
                                    std::size_t packetLength);
};

class CompressionZ : public CompressionStrategy {
private:
    // How large a packet needs to be before it's compressed.
    // Don't compress packets smaller than this.
    // Received in SetCompressionPacket.
    u64 m_CompressionThreshold;

public:
    MCLIB_API CompressionZ(u64 threshold) : m_CompressionThreshold(threshold) {}

    DataBuffer MCLIB_API Compress(DataBuffer& buffer);
    DataBuffer MCLIB_API Decompress(DataBuffer& buffer,
                                    std::size_t packetLength);
};

}  // namespace core
}  // namespace mc

#endif
