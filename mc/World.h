#ifndef WORLD_H_
#define WORLD_H_

#include "Chunk.h"
#include "Packets/PacketHandler.h"
#include "Packets/PacketDispatcher.h"
#include "ObserverSubject.h"
#include <map>

namespace Minecraft {

class WorldListener {
public:
    // yIndex is the chunk section index of the column, 0 means bottom chunk, 15 means top
    virtual void OnChunkLoad(ChunkPtr chunk, const ChunkColumnMetadata& meta, u16 yIndex) = 0;
};

class World : public Packets::PacketHandler, public ObserverSubject<WorldListener> {
private:
    typedef std::pair<s32, s32> ChunkCoord;

    std::map<ChunkCoord, ChunkColumnPtr> m_Chunks;

public:
    World(Packets::PacketDispatcher* dispatcher);
    ~World();

    void HandlePacket(Packets::Inbound::ChunkDataPacket* packet);
    void HandlePacket(Packets::Inbound::MapChunkBulkPacket* packet);
    void HandlePacket(Packets::Inbound::MultiBlockChangePacket* packet);
    void HandlePacket(Packets::Inbound::BlockChangePacket* packet);

    /**
     * Pos can be any world position inside of the chunk
     */
    ChunkColumnPtr GetChunk(Vector3i pos) const;

    BlockPtr GetBlock(Vector3d pos) const;
    BlockPtr GetBlock(Vector3f pos) const;
    BlockPtr GetBlock(Vector3i pos) const;
};

} // ns Minecraft

#endif
