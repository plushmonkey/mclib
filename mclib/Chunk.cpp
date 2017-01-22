#include "Chunk.h"
#include "DataBuffer.h"
#include <algorithm>
#include <iostream>

namespace Minecraft {

Chunk::Chunk()
{
    m_BitsPerBlock = 0;
    /*for (s32 y = 0; y < 16; ++y) {
        for (s32 z = 0; z < 16; ++z) {
            for (s32 x = 0; x < 16; ++x) {
                std::size_t index = y * 16 * 16 + z * 16 + x;

                m_Blocks[index] = BlockRegistry::GetInstance()->GetBlock(0, 0);
            }
        }
    }*/
}

Chunk::Chunk(const Chunk& other) {
    m_Data = other.m_Data;
    m_BitsPerBlock = other.m_BitsPerBlock;
}

void Chunk::Load(DataBuffer& in, ChunkColumnMetadata* meta, s32 chunkIndex) {
    in >> m_BitsPerBlock;

    VarInt paletteLength;
    in >> paletteLength;

    s32 abc = paletteLength.GetInt();

    for (s32 i = 0; i < paletteLength.GetInt(); ++i) {
        VarInt paletteValue;
        in >> paletteValue;
        m_Palette.push_back((u16)paletteValue.GetInt());
    }

    VarInt dataArrayLength;
    in >> dataArrayLength;
    s32 arrayLen = dataArrayLength.GetInt();

    m_Data.resize(dataArrayLength.GetInt());
    
    for (s32 i = 0; i < dataArrayLength.GetInt(); ++i) {
        s64 data;
        in >> data;

        m_Data[i] = data;
    }

    static const s64 lightSize = 16 * 16 * 16 / 2;

    // Block light data
    in.SetReadOffset(in.GetReadOffset() + lightSize);

    meta->skylight = true;

    // Sky Light
    if (meta->skylight) {
        in.SetReadOffset(in.GetReadOffset() + lightSize);
    }
}

BlockPtr Chunk::GetBlock(Vector3i chunkPosition) {
    if (chunkPosition.y < 0 || chunkPosition.y > 15) return BlockRegistry::GetInstance()->GetBlock(0, 0);

    std::size_t index = (std::size_t)(chunkPosition.y * 16 * 16 + chunkPosition.z * 16 + chunkPosition.x);
    std::size_t entryIndex = (index * m_BitsPerBlock) / 64;
    std::size_t endIndex = (((index + 1) * m_BitsPerBlock) - 1) / 64;
    std::size_t bitIndex = (index * m_BitsPerBlock) % 64;
    
    s64 maxValue = (1 << m_BitsPerBlock) - 1;
    u16 value;

    if (entryIndex == endIndex) {
        value = (m_Data[entryIndex] >> (64 - bitIndex - m_BitsPerBlock)) & maxValue;
    } else {
        u64 startMask = ((1 << (64 - bitIndex)) - 1);
        u64 endBits = m_BitsPerBlock - (64 - bitIndex);
        value = ((m_Data[entryIndex] & startMask) << endBits) | (m_Data[endIndex] >> (64 - endBits));
    }

    u16 blockType = m_Palette[value];
    return BlockRegistry::GetInstance()->GetBlock(blockType);
}

void Chunk::SetBlock(Vector3i chunkPosition, BlockPtr block) {
    std::size_t index = (std::size_t)(chunkPosition.y * 16 * 16 + chunkPosition.z * 16 + chunkPosition.x);
    std::size_t entryIndex = (index * m_BitsPerBlock) / 64;
    std::size_t endIndex = (((index + 1) * m_BitsPerBlock) - 1) / 64;
    std::size_t bitIndex = (index * m_BitsPerBlock) % 64;

    s64 maxValue = (1 << m_BitsPerBlock) - 1;
    u16 blockType = block->GetData();

    auto iter = std::find_if(m_Palette.begin(), m_Palette.end(), [blockType](u16 ptype) {
        return ptype == blockType;
    });

    if (iter == m_Palette.end())
        iter = m_Palette.insert(m_Palette.end(), blockType);

    u16 value = std::distance(m_Palette.begin(), iter);

    // Erase old value in data entry and OR with new data
    m_Data[entryIndex] = (m_Data[entryIndex] & ~(maxValue << (64 - m_BitsPerBlock - bitIndex))) | (((s64)value & maxValue) << (64 - m_BitsPerBlock - bitIndex));

    if (entryIndex != endIndex) {
        // Erase beginning of data and then OR with new data
        u64 endMask = (1 << (m_BitsPerBlock - 64 - bitIndex)) - 1;
        u64 endBits = m_BitsPerBlock - (64 - bitIndex);
        m_Data[endIndex] = (m_Data[endIndex] & ~(endMask << (64 - endBits))) | (((s64)value & endMask) << (64 - endBits));
    }
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

    if (!m_Chunks[chunkIndex]) return BlockRegistry::GetInstance()->GetBlock(0, 0);

    return m_Chunks[chunkIndex]->GetBlock(relativePosition);
}

DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column) {
    ChunkColumnMetadata* meta = &column.m_Metadata;

  /*  if (meta->continuous) {
        auto chunk = std::make_shared<Chunk>();
        chunk->Load(in, meta, 0);

        for (s16 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
            // The section mask says whether or not there is data in this chunk.
            if (meta->sectionmask & (1 << i)) {
                column.m_Chunks[i] = std::make_shared<Chunk>(*chunk);
            } else {
                // Air section, leave null
                column.m_Chunks[i] = nullptr;
            }
        }
    } else {*/
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
  //  }

    return in;
}

} // ns Minecraft
