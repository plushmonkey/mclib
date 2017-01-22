#ifndef CHUNK_H_
#define CHUNK_H_

#include "Types.h"
#include "Block.h"

#include <array>
#include <memory>

namespace Minecraft {

class DataBuffer;

struct ChunkColumnMetadata {
    s32 x;
    s32 z;
    u16 sectionmask;
    bool continuous;
    bool skylight;
};


/**
 * A 16x16x16 area. A ChunkColumn is made up of 16 of these
 */
class Chunk {
private:
    std::vector<u16> m_Palette;
    std::vector<s64> m_Data;
    u8 m_BitsPerBlock;

public:
    MCLIB_API Chunk();

    MCLIB_API Chunk(const Chunk& other);
    /**
     * Position is relative to this chunk position
     */
    BlockPtr MCLIB_API GetBlock(Vector3i chunkPosition);

    /**
    * Position is relative to this chunk position
    */
    void MCLIB_API SetBlock(Vector3i chunkPosition, BlockPtr block);

    /**
     * chunkIndex is the index (0-16) of this chunk in the ChunkColumn
     */
    void MCLIB_API Load(DataBuffer& in, ChunkColumnMetadata* meta, s32 chunkIndex);
};

typedef std::shared_ptr<Chunk> ChunkPtr;

/**
 * Stores a 16x256x16 area. Uses chunks (16x16x16) to store the data vertically.
 * A null chunk is fully air.
 */
class ChunkColumn {
public:
    enum { ChunksPerColumn = 16 };

    typedef std::array<ChunkPtr, ChunksPerColumn>::iterator iterator;
    typedef std::array<ChunkPtr, ChunksPerColumn>::reference reference;

private:
    std::array<ChunkPtr, ChunksPerColumn> m_Chunks;
    ChunkColumnMetadata m_Metadata;

public:
    MCLIB_API ChunkColumn(ChunkColumnMetadata metadata);

    iterator MCLIB_API begin() {
        return m_Chunks.begin();
    }

    iterator MCLIB_API end() {
        return m_Chunks.end();
    }

    reference MCLIB_API operator[](std::size_t index) {
        return m_Chunks[index];
    }

    /**
     * Position is relative to this ChunkColumn position.
     */
    BlockPtr MCLIB_API GetBlock(Vector3i position);
    const ChunkColumnMetadata& GetMetadata() const { return m_Metadata; }

    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);
};

typedef std::shared_ptr<ChunkColumn> ChunkColumnPtr;

MCLIB_API DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);

} // ns Minecraft

#endif
