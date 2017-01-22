#include "World.h"
#include <iostream>
namespace Minecraft {

World::World(Packets::PacketDispatcher* dispatcher) 
    : Packets::PacketHandler(dispatcher)
{
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::MultiBlockChange, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::BlockChange, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::ChunkData, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Explosion, this);
}

World::~World() {
    GetDispatcher()->UnregisterHandler(this);
}

bool World::SetBlock(Vector3i position, s16 blockData) {
    ChunkColumnPtr chunk = GetChunk(position);
    if (!chunk) return false;

    Vector3i relative(position);

    relative.x %= 16;
    relative.z %= 16;

    if (relative.x < 0)
        relative.x += 16;
    if (relative.z < 0)
        relative.z += 16;

    std::size_t index = (std::size_t)position.y / 16;
    if ((*chunk)[index] == nullptr) {
        ChunkPtr section = std::make_shared<Chunk>();

        (*chunk)[index] = section;
    }

    relative.y %= 16;
    (*chunk)[index]->SetBlock(relative, BlockRegistry::GetInstance()->GetBlock(blockData));
    return true;
}

void World::HandlePacket(Packets::Inbound::ExplosionPacket* packet) {
    Vector3d position = packet->GetPosition();
    for (Vector3s offset : packet->GetAffectedBlocks()) {
        Vector3d absolute = position + ToVector3d(offset);

        // Set all affected blocks to air
        SetBlock(ToVector3i(absolute), 0);
    }
}

void World::HandlePacket(Packets::Inbound::ChunkDataPacket* packet) {
    ChunkColumnPtr col = packet->GetChunkColumn();
    const ChunkColumnMetadata& meta = col->GetMetadata();
    ChunkCoord key(meta.x, meta.z);

    if (meta.continuous && meta.sectionmask == 0) {
        m_Chunks[key] = nullptr;
        return;
    }

    if (!m_Chunks[key])
        m_Chunks[key] = std::make_shared<ChunkColumn>(meta);

    for (s32 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
        if (meta.sectionmask & (1 << i)) {
            ChunkPtr chunk = (*col)[i];        

            (*m_Chunks[key])[i] = chunk;

            NotifyListeners(&WorldListener::OnChunkLoad, chunk, meta, i);
        }
    }
}

void World::HandlePacket(Packets::Inbound::MultiBlockChangePacket* packet) {
    Vector3i chunkStart(packet->GetChunkX() * 16, 0, packet->GetChunkZ() * 16);

    ChunkColumnPtr chunk = GetChunk(chunkStart);
    if (!chunk)
        return;

    const auto& changes = packet->GetBlockChanges();
    for (const auto& change : changes) {
        Vector3i relative(change.x, change.y, change.z);

        std::size_t index = change.y / 16;
        if ((*chunk)[index] == nullptr) {
            ChunkPtr section = std::make_shared<Chunk>();

            (*chunk)[index] = section;
        }
        relative.y %= 16;
        (*chunk)[index]->SetBlock(relative, BlockRegistry::GetInstance()->GetBlock(change.blockData));
    }
}

void World::HandlePacket(Packets::Inbound::BlockChangePacket* packet) {
    SetBlock(packet->GetPosition(), packet->GetBlockId());
}

ChunkColumnPtr World::GetChunk(Vector3i pos) const {
    s32 x = (s32)std::floor(pos.x / 16.0);
    s32 z = (s32)std::floor(pos.z / 16.0);

    ChunkCoord key(x, z);

    auto iter = m_Chunks.find(key);

    if (iter == m_Chunks.end()) return nullptr;

    return iter->second;
}

BlockPtr World::GetBlock(Vector3f pos) const {
    return GetBlock(Vector3i((s64)std::floor(pos.x), (s64)std::floor(pos.y), (s64)std::floor(pos.z)));
}

BlockPtr World::GetBlock(Vector3d pos) const {
    return GetBlock(Vector3i((s64)std::floor(pos.x), (s64)std::floor(pos.y), (s64)std::floor(pos.z)));
}

BlockPtr World::GetBlock(Vector3i pos) const {
    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return nullptr;

    s64 x = pos.x % 16;
    s64 z = pos.z % 16;

    if (x < 0)
        x += 16;
    if (z < 0)
        z += 16;

    return col->GetBlock(Vector3i(x, pos.y, z));
}

}
