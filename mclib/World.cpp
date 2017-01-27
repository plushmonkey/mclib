#include "World.h"
#include <iostream>
namespace Minecraft {

World::World(Packets::PacketDispatcher* dispatcher) 
    : Packets::PacketHandler(dispatcher)
{
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::MultiBlockChange, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::BlockChange, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::ChunkData, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::UnloadChunk, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Explosion, this);
    dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::UpdateBlockEntity, this);
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
    (*chunk)[index]->SetBlock(relative, BlockState(BlockRegistry::GetInstance()->GetBlock(blockData), blockData));
    return true;
}

void World::HandlePacket(Packets::Inbound::ExplosionPacket* packet) {
    Vector3d position = packet->GetPosition();
    BlockState newBlock = BlockState(BlockRegistry::GetInstance()->GetBlock(0), 0);

    for (Vector3s offset : packet->GetAffectedBlocks()) {
        Vector3d absolute = position + ToVector3d(offset);
    
        BlockState oldBlock = GetBlock(absolute);

        // Set all affected blocks to air
        SetBlock(ToVector3i(absolute), 0);

        NotifyListeners(&WorldListener::OnBlockChange, ToVector3i(absolute), newBlock, oldBlock);
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
        m_Chunks[key] = col;

    for (s32 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
        if (meta.sectionmask & (1 << i)) {
            ChunkPtr chunk = (*col)[i];

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

        chunk->RemoveBlockEntity(chunkStart + relative);

        std::size_t index = change.y / 16;
        if ((*chunk)[index] == nullptr) {
            ChunkPtr section = std::make_shared<Chunk>();

            (*chunk)[index] = section;
        }

        BlockState newBlock = BlockState(BlockRegistry::GetInstance()->GetBlock(change.blockData), change.blockData);
        BlockState oldBlock = chunk->GetBlock(relative);
        Vector3i blockChangePos = chunkStart + relative;

        relative.y %= 16;
        (*chunk)[index]->SetBlock(relative, BlockState(BlockRegistry::GetInstance()->GetBlock(change.blockData), change.blockData));
        NotifyListeners(&WorldListener::OnBlockChange, blockChangePos, newBlock, oldBlock);
    }
}

void World::HandlePacket(Packets::Inbound::BlockChangePacket* packet) {
    BlockState newBlock = BlockState(BlockRegistry::GetInstance()->GetBlock((u16)packet->GetBlockId()), (u16)packet->GetBlockId());
    BlockState oldBlock = GetBlock(packet->GetPosition());

    SetBlock(packet->GetPosition(), packet->GetBlockId());

    NotifyListeners(&WorldListener::OnBlockChange, packet->GetPosition(), newBlock, oldBlock);

    ChunkColumnPtr col = GetChunk(packet->GetPosition());
    if (col) {
        col->RemoveBlockEntity(packet->GetPosition());
    }
}

void World::HandlePacket(Packets::Inbound::UpdateBlockEntityPacket* packet) {
    Vector3i pos = packet->GetPosition();

    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return;

    col->RemoveBlockEntity(pos);

    BlockEntityPtr entity = packet->GetBlockEntity();
    col->AddBlockEntity(entity);
}

void World::HandlePacket(Packets::Inbound::UnloadChunkPacket* packet) {
    ChunkCoord coord(packet->GetChunkX(), packet->GetChunkZ());

    auto iter = m_Chunks.find(coord);

    if (iter == m_Chunks.end()) return;

    ChunkColumnPtr chunk = iter->second;
    NotifyListeners(&WorldListener::OnChunkUnload, chunk);

    m_Chunks.erase(iter);
}

ChunkColumnPtr World::GetChunk(Vector3i pos) const {
    s32 x = (s32)std::floor(pos.x / 16.0);
    s32 z = (s32)std::floor(pos.z / 16.0);

    ChunkCoord key(x, z);

    auto iter = m_Chunks.find(key);

    if (iter == m_Chunks.end()) return nullptr;

    return iter->second;
}

BlockState World::GetBlock(Vector3f pos) const {
    return GetBlock(Vector3i((s64)std::floor(pos.x), (s64)std::floor(pos.y), (s64)std::floor(pos.z)));
}

BlockState World::GetBlock(Vector3d pos) const {
    return GetBlock(Vector3i((s64)std::floor(pos.x), (s64)std::floor(pos.y), (s64)std::floor(pos.z)));
}

BlockState World::GetBlock(Vector3i pos) const {
    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return BlockState(nullptr, 0);

    s64 x = pos.x % 16;
    s64 z = pos.z % 16;

    if (x < 0)
        x += 16;
    if (z < 0)
        z += 16;

    return col->GetBlock(Vector3i(x, pos.y, z));
}

BlockEntityPtr World::GetBlockEntity(Vector3i pos) const {
    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return nullptr;

    return col->GetBlockEntity(pos);
}

std::vector<BlockEntityPtr> World::GetBlockEntities() const {
    std::vector<BlockEntityPtr> blockEntities;

    for (auto iter = m_Chunks.begin(); iter != m_Chunks.end(); ++iter) {
        std::vector<BlockEntityPtr> chunkBlockEntities = iter->second->GetBlockEntities();

        blockEntities.insert(blockEntities.end(), chunkBlockEntities.begin(), chunkBlockEntities.end());
    }

    return blockEntities;
}

}
