#include "Chunk.h"
#include "DataBuffer.h"
#include <iostream>
#include <algorithm>
namespace Minecraft {

Chunk::Chunk()
{
    for (s32 y = 0; y < 16; ++y) {
        for (s32 z = 0; z < 16; ++z) {
            for (s32 x = 0; x < 16; ++x) {
                std::size_t index = y * 16 * 16 + z * 16 + x;

                m_Blocks[index] = BlockRegistry::GetInstance().GetBlock(0, 0);
            }
        }
    }
}

void Chunk::Load(DataBuffer& in, ChunkColumnMetadata* meta, s32 chunkIndex) {
    for (s32 y = 0; y < 16; ++y) {
        for (s32 z = 0; z < 16; ++z) {
            for (s32 x = 0; x < 16; ++x) {
                s16 data;                

                in >> data;
                std::reverse((u8*)&data, (u8*)&data + sizeof(s16));

                std::size_t index = y * 16 * 16 + z * 16 + x;

                m_Blocks[index] = BlockRegistry::GetInstance().GetBlock(data);
                if (m_Blocks[index] == nullptr) {
                    u16 type = data >> 4;
                    u16 meta = data & 15;
                    m_Blocks[index] = BlockRegistry::GetInstance().GetBlock(type, meta);
                }
            }
        }
    }
}

BlockPtr Chunk::GetBlock(Vector3i chunkPosition) {
    std::size_t index = (std::size_t)(chunkPosition.y * 16 * 16 + chunkPosition.z * 16 + chunkPosition.x);
    return m_Blocks[index];
}

void Chunk::SetBlock(Vector3i chunkPosition, BlockPtr block) {
    std::size_t index = (std::size_t)(chunkPosition.y * 16 * 16 + chunkPosition.z * 16 + chunkPosition.x);
    m_Blocks[index] = block;
}

ChunkColumn::ChunkColumn(ChunkColumnMetadata metadata)
    : m_Metadata(metadata)
{
    for (std::size_t i = 0; i < m_Chunks.size(); ++i)
        m_Chunks[i] = nullptr;
}

BlockPtr ChunkColumn::GetBlock(Vector3i position) {
    s32 chunkIndex = (s32)(position.y / 16);
    Vector3i relativePosition(position.x, position.y % 16, position.z);

    if (!m_Chunks[chunkIndex]) return nullptr;

    return m_Chunks[chunkIndex]->GetBlock(relativePosition);
}

DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column) {
    ChunkColumnMetadata* meta = &column.m_Metadata;

    for (s16 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
        // The section mask says whether or not there is data in this chunk.
        if (meta->sectionmask & (1 << i)) {
            column.m_Chunks[i] = std::make_shared<Chunk>();

            column.m_Chunks[i]->Load(in, meta, i);
        } else {
            // Air section, leave null
            column.m_Chunks[i] = nullptr;
        }
    }

    static const s64 lightSize = 16 * 16 * 16 / 2;

    for (s16 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
        // Block light data
        if (meta->sectionmask & (1 << i))
            in.SetReadOffset(in.GetReadOffset() + lightSize);
    }

    if (meta->skylight) {
        for (s16 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
            // Skylight data
            if (meta->sectionmask & (1 << i))
                in.SetReadOffset(in.GetReadOffset() + lightSize);
        }
    }

    if (meta->continuous)
        in.SetReadOffset(in.GetReadOffset() + 256);

    return in;
}

} // ns Minecraft
