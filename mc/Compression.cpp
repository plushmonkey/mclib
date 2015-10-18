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

    std::string deflateddData;
    buffer.ReadSome(deflateddData, uncompressedLength.GetInt());
    return DataBuffer();
}

} // ns Minecraft
