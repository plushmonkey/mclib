#include "World.h"

namespace Minecraft {

World::World(Packets::PacketDispatcher* dispatcher) 
    : Packets::PacketHandler(dispatcher)
{
    dispatcher->RegisterHandler(Minecraft::Protocol::State::Play, 0x22, this);
    dispatcher->RegisterHandler(Minecraft::Protocol::State::Play, 0x23, this);
    dispatcher->RegisterHandler(Minecraft::Protocol::State::Play, 0x26, this);
}

World::~World() {
    GetDispatcher()->UnregisterHandler(this);
}

void World::HandlePacket(Packets::Inbound::MapChunkBulkPacket* packet) {
    const std::map<ChunkCoord, ChunkColumnPtr>& cols = packet->GetChunkColumns();

    for (const auto& kv : cols) {
        ChunkCoord key = kv.first;
        ChunkColumnPtr col = kv.second;

        m_Chunks[key] = col;
    }
}

void World::HandlePacket(Packets::Inbound::MultiBlockChangePacket* packet) {
    Vector3i chunkStart(packet->GetChunkX() * 16, 0, packet->GetChunkZ() * 16);

    ChunkColumnPtr chunk = GetChunk(chunkStart);
    if (!chunk) return;


    const auto& changes = packet->GetBlockChanges();
    for (const auto& change : changes) {
        Vector3i relative(change.x, change.y, change.z);
        BlockPtr block = chunk->GetBlock(relative);

        if (!block) continue;
        block->data = change.blockData;
    }
}

void World::HandlePacket(Packets::Inbound::BlockChangePacket* packet) {
    Vector3i pos = packet->GetPosition();

    ChunkColumnPtr chunk = GetChunk(pos);
    if (!chunk) return;

    BlockPtr block = chunk->GetBlock(pos);
    if (!block) return;

    s16 blockData = packet->GetBlockData();
    block->data = blockData;
}

ChunkColumnPtr World::GetChunk(Vector3i pos) const {
    s32 x = (s32)pos.x / 16;
    s32 z = (s32)pos.z / 16;

    ChunkCoord key(x, z);
    if (m_Chunks.find(key) == m_Chunks.end()) return nullptr;

    return m_Chunks.at(key);
}

BlockPtr World::GetBlock(Vector3f pos) const {
    return GetBlock(Vector3i((s64)pos.x, (s64)pos.y, (s64)pos.z));
}

BlockPtr World::GetBlock(Vector3d pos) const {
    return GetBlock(Vector3i((s64)pos.x, (s64)pos.y, (s64)pos.z));
}

BlockPtr World::GetBlock(Vector3i pos) const {
    s32 x = (s32)pos.x / 16;
    s32 z = (s32)pos.z / 16;

    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return nullptr;

    return col->GetBlock(Vector3i(pos.x % 16, pos.y, pos.z % 16));
}

}
