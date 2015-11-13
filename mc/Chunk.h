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
    // This array could just be s16 for the block data instead of a pointer. 
    // GetBlock would just do a lookup in BlockRegistry when needed. 
    // It would reduce memory use, but it could have performance cost.
    std::array<BlockPtr, 16*16*16> m_Blocks;

public:
    Chunk();

    /**
     * Position is relative to this chunk position
     */
    BlockPtr GetBlock(Vector3i chunkPosition);

    /**
    * Position is relative to this chunk position
    */
    void SetBlock(Vector3i chunkPosition, BlockPtr block);

    /**
     * chunkIndex is the index (0-16) of this chunk in the ChunkColumn
     */
    void Load(DataBuffer& in, ChunkColumnMetadata* meta, s32 chunkIndex);
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
    ChunkColumn(ChunkColumnMetadata metadata);

    iterator begin() {
        return m_Chunks.begin();
    }

    iterator end() {
        return m_Chunks.end();
    }

    reference operator[](std::size_t index) {
        return m_Chunks[index];
    }

    /**
     * Position is relative to this ChunkColumn position.
     */
    BlockPtr GetBlock(Vector3i position);
    const ChunkColumnMetadata& GetMetadata() const { return m_Metadata; }

    friend DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);
};

typedef std::shared_ptr<ChunkColumn> ChunkColumnPtr;

DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);

} // ns Minecraft

#endif
