#ifndef MCLIB_WORLD_WORLD_H_
#define MCLIB_WORLD_WORLD_H_

#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/util/ObserverSubject.h>
#include <mclib/world/Chunk.h>

#include <map>

namespace mc {
namespace world {

class MCLIB_API WorldListener {
public:
    // yIndex is the chunk section index of the column, 0 means bottom chunk, 15
    // means top
    virtual void OnChunkLoad(ChunkPtr chunk, const ChunkColumnMetadata& meta,
                             u16 yIndex) {}
    virtual void OnChunkUnload(ChunkColumnPtr chunk) {}
    virtual void OnBlockChange(Vector3i position, block::BlockPtr newBlock,
                               block::BlockPtr oldBlock) {}
};

class World : public protocol::packets::PacketHandler,
              public util::ObserverSubject<WorldListener> {
private:
    typedef std::pair<s32, s32> ChunkCoord;

    std::map<ChunkCoord, ChunkColumnPtr> m_Chunks;

    bool MCLIB_API SetBlock(Vector3i position, u32 blockData);

public:
    MCLIB_API World(protocol::packets::PacketDispatcher* dispatcher);
    MCLIB_API ~World();

    World(const World& rhs) = delete;
    World& operator=(const World& rhs) = delete;
    World(World&& rhs) = delete;
    World& operator=(World&& rhs) = delete;

    void MCLIB_API HandlePacket(protocol::packets::in::ChunkDataPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::UnloadChunkPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::MultiBlockChangePacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::BlockChangePacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::ExplosionPacket* packet);
    void MCLIB_API
    HandlePacket(protocol::packets::in::UpdateBlockEntityPacket* packet);
    void MCLIB_API HandlePacket(protocol::packets::in::RespawnPacket* packet);

    /**
     * Pos can be any world position inside of the chunk
     */
    ChunkColumnPtr MCLIB_API GetChunk(Vector3i pos) const;

    block::BlockPtr MCLIB_API GetBlock(Vector3d pos) const;
    block::BlockPtr MCLIB_API GetBlock(Vector3f pos) const;
    block::BlockPtr MCLIB_API GetBlock(Vector3i pos) const;

    MCLIB_API block::BlockEntityPtr GetBlockEntity(Vector3i pos) const;
    // Gets all of the known block entities in loaded chunks
    MCLIB_API std::vector<block::BlockEntityPtr> GetBlockEntities() const;

    const std::map<ChunkCoord, ChunkColumnPtr>::const_iterator begin() const {
        return m_Chunks.begin();
    }
    const std::map<ChunkCoord, ChunkColumnPtr>::const_iterator end() const {
        return m_Chunks.end();
    }
};

}  // namespace world
}  // namespace mc

#endif
