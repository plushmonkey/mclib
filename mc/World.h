#ifndef WORLD_H_
#define WORLD_H_

#include "Chunk.h"
#include "Packets/PacketHandler.h"
#include "Packets/PacketDispatcher.h"
#include <map>

namespace Minecraft {

class World : public Packets::PacketHandler {
private:
    typedef std::pair<s32, s32> ChunkCoord;

    std::map<ChunkCoord, ChunkColumnPtr> m_Chunks;

public:
    World(Packets::PacketDispatcher& dispatcher);

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
