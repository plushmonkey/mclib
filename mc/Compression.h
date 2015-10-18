#ifndef COMPRESSION_H_
#define COMPRESSION_H_

#include <cstddef>

namespace Minecraft {

class DataBuffer;

class CompressionStrategy {
public:
    virtual DataBuffer Compress(DataBuffer& buffer) = 0;
    virtual DataBuffer Decompress(DataBuffer& buffer, std::size_t packetLength) = 0;
};

class CompressionNone : public CompressionStrategy {
public:
    DataBuffer Compress(DataBuffer& buffer);
    DataBuffer Decompress(DataBuffer& buffer, std::size_t packetLength);
};

class CompressionZ : public CompressionStrategy {
public:
    DataBuffer Compress(DataBuffer& buffer);
    DataBuffer Decompress(DataBuffer& buffer, std::size_t packetLength);
};

} // ns Minecraft

#endif
