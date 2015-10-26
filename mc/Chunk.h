#ifndef CHUNK_H_
#define CHUNK_H_

#include "Types.h"
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

// todo:
class Block {
public:
    Vector3i position;
    s16 type;
    s16 meta;
};
typedef Block* BlockPtr;

/**
 * A 16x16x16 area. A ChunkColumn is made up of 16 of these
 */
class Chunk {
private:
    std::array<Block, 16*16*16> m_Blocks;

public:
    Chunk();

    /**
     * Position is relative to this chunk position
     */
    BlockPtr GetBlock(Vector3i chunkPosition);

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
private:
    enum { ChunksPerColumn = 16 };

    std::array<ChunkPtr, ChunksPerColumn> m_Chunks;
    ChunkColumnMetadata m_Metadata;

public:
    ChunkColumn(ChunkColumnMetadata metadata);

    /**
     * Position is relative to this ChunkColumn position.
     */
    BlockPtr GetBlock(Vector3i position);

    friend DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);
};

typedef std::shared_ptr<ChunkColumn> ChunkColumnPtr;

DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);

} // ns Minecraft

#endif
