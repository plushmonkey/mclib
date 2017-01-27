#ifndef WORLD_H_
#define WORLD_H_

#include "Chunk.h"
#include "Packets/PacketHandler.h"
#include "Packets/PacketDispatcher.h"
#include "ObserverSubject.h"
#include <map>

namespace Minecraft {

class MCLIB_API WorldListener {
public:
    // yIndex is the chunk section index of the column, 0 means bottom chunk, 15 means top
    virtual void OnChunkLoad(ChunkPtr chunk, const ChunkColumnMetadata& meta, u16 yIndex) { }
    virtual void OnChunkUnload(ChunkColumnPtr chunk) { }
    virtual void OnBlockChange(Vector3i position, BlockState newBlock, BlockState oldBlock) { }
};

class World : public Packets::PacketHandler, public ObserverSubject<WorldListener> {
private:
    typedef std::pair<s32, s32> ChunkCoord;

    std::map<ChunkCoord, ChunkColumnPtr> m_Chunks;

    bool MCLIB_API SetBlock(Vector3i position, s16 blockData);

public:
    MCLIB_API World(Packets::PacketDispatcher* dispatcher);
    MCLIB_API ~World();

    void MCLIB_API HandlePacket(Packets::Inbound::ChunkDataPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::UnloadChunkPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::MultiBlockChangePacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::BlockChangePacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::ExplosionPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::UpdateBlockEntityPacket* packet);

    /**
     * Pos can be any world position inside of the chunk
     */
    ChunkColumnPtr MCLIB_API GetChunk(Vector3i pos) const;

    BlockState MCLIB_API GetBlock(Vector3d pos) const;
    BlockState MCLIB_API GetBlock(Vector3f pos) const;
    BlockState MCLIB_API GetBlock(Vector3i pos) const;

    MCLIB_API BlockEntityPtr GetBlockEntity(Vector3i pos) const;
    // Gets all of the known block entities in loaded chunks
    MCLIB_API std::vector<BlockEntityPtr> GetBlockEntities() const;

    const std::map<ChunkCoord, ChunkColumnPtr>::const_iterator begin() const { return m_Chunks.begin(); }
    const std::map<ChunkCoord, ChunkColumnPtr>::const_iterator end() const { return m_Chunks.end(); }
};

} // ns Minecraft

#endif
