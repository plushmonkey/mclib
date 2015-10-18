#include "Compression.h"
#include "DataBuffer.h"
#include <zlib.h>

namespace Minecraft {

DataBuffer CompressionNone::Compress(DataBuffer& buffer) {
    return buffer;
}

DataBuffer CompressionNone::Decompress(DataBuffer& buffer, std::size_t packetLength) {
    DataBuffer ret;
    buffer.ReadSome(ret, packetLength);
    return ret;
}

unsigned long inflate(const std::string& source, std::string& dest) {
    unsigned long size = dest.size();
    uncompress((Bytef*)&dest[0], &size, (const Bytef*)source.c_str(), source.length());
    return size;
}

unsigned long deflate(const std::string& source, std::string& dest) {
    unsigned long size = source.length();
    dest.resize(size);

    compress((Bytef*)&dest[0], &size, (const Bytef*)source.c_str(), source.length());
    dest.resize(size);
    return size;
}

DataBuffer CompressionZ::Compress(DataBuffer& buffer) {
    return buffer;
}

DataBuffer CompressionZ::Decompress(DataBuffer& buffer, std::size_t packetLength) {
    VarInt uncompressedLength;

    std::size_t startRead = buffer.GetReadOffset();

    buffer >> uncompressedLength;

    std::size_t compressedLength = packetLength - (buffer.GetReadOffset() - startRead);

    if (uncompressedLength.GetLong() == 0) {
        // Uncompressed
        DataBuffer ret;
        buffer.ReadSome(ret, compressedLength);
        return ret;
    }

    std::string deflatedData;
    buffer.ReadSome(deflatedData, compressedLength);

    std::string inflated;
    inflated.resize(uncompressedLength.GetInt());

    inflate(deflatedData, inflated);
    return DataBuffer(inflated);
}

} // ns Minecraft
