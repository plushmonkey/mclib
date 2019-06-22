#include <mclib/core/Compression.h>

#include <mclib/common/DataBuffer.h>

#include <zlib.h>
#include <cassert>
#include <iostream>

namespace mc {
namespace core {

DataBuffer CompressionNone::Compress(DataBuffer& buffer) {
    DataBuffer packet;

    VarInt length((s32)buffer.GetSize());
    packet << length;
    packet << buffer;

    return packet;
}

DataBuffer CompressionNone::Decompress(DataBuffer& buffer,
                                       std::size_t packetLength) {
    DataBuffer ret;
    buffer.ReadSome(ret, packetLength);
    return ret;
}

unsigned long inflate(const std::string& source, std::string& dest) {
    unsigned long size = dest.size();
    uncompress((Bytef*)&dest[0], &size, (const Bytef*)source.c_str(),
               source.length());
    return size;
}

unsigned long deflate(const std::string& source, std::string& dest) {
    unsigned long size = source.length();
    dest.resize(size);

    compress((Bytef*)&dest[0], &size, (const Bytef*)source.c_str(),
             source.length());
    dest.resize(size);
    return size;
}

DataBuffer CompressionZ::Compress(DataBuffer& buffer) {
    std::string compressedData;
    DataBuffer packet;

    if (buffer.GetSize() < m_CompressionThreshold) {
        // Don't compress since it's a small packet
        VarInt dataLength(0);
        VarInt packetLength(
            (s32)(buffer.GetSize() + dataLength.GetSerializedLength()));

        packet << packetLength;
        packet << dataLength;
        packet << buffer;
        return packet;
    }

    deflate(buffer.ToString(), compressedData);

    VarInt dataLength((s32)buffer.GetSize());
    VarInt packetLength(
        (s32)(compressedData.length() + dataLength.GetSerializedLength()));

    packet << packetLength;
    packet << dataLength;
    packet << compressedData;
    return packet;
}

DataBuffer CompressionZ::Decompress(DataBuffer& buffer,
                                    std::size_t packetLength) {
    VarInt uncompressedLength;

    buffer >> uncompressedLength;

    std::size_t compressedLength =
        packetLength - uncompressedLength.GetSerializedLength();

    if (uncompressedLength.GetInt() == 0) {
        // Uncompressed
        DataBuffer ret;
        buffer.ReadSome(ret, compressedLength);
        return ret;
    }

    assert(buffer.GetReadOffset() + compressedLength <= buffer.GetSize());

    std::string deflatedData;
    buffer.ReadSome(deflatedData, compressedLength);

    std::string inflated;
    inflated.resize(uncompressedLength.GetInt());

    inflate(deflatedData, inflated);

    assert(inflated.length() == uncompressedLength.GetInt());
    return DataBuffer(inflated);
}

}  // namespace core
}  // namespace mc
