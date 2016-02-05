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
    virtual void OnChunkLoad(ChunkPtr chunk, const ChunkColumnMetadata& meta, u16 yIndex) = 0;
};

class World : public Packets::PacketHandler, public ObserverSubject<WorldListener> {
private:
    typedef std::pair<s32, s32> ChunkCoord;

    std::map<ChunkCoord, ChunkColumnPtr> m_Chunks;

public:
    MCLIB_API World(Packets::PacketDispatcher* dispatcher);
    MCLIB_API ~World();

    void MCLIB_API HandlePacket(Packets::Inbound::ChunkDataPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::MapChunkBulkPacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::MultiBlockChangePacket* packet);
    void MCLIB_API HandlePacket(Packets::Inbound::BlockChangePacket* packet);

    /**
     * Pos can be any world position inside of the chunk
     */
    ChunkColumnPtr MCLIB_API GetChunk(Vector3i pos) const;

    BlockPtr MCLIB_API GetBlock(Vector3d pos) const;
    BlockPtr MCLIB_API GetBlock(Vector3f pos) const;
    BlockPtr MCLIB_API GetBlock(Vector3i pos) const;
};

} // ns Minecraft

#endif
