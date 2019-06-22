#include <mclib/world/World.h>

namespace mc {
namespace world {

World::World(protocol::packets::PacketDispatcher* dispatcher)
    : protocol::packets::PacketHandler(dispatcher) {
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::MultiBlockChange, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::BlockChange, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::ChunkData, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::UnloadChunk, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::Explosion, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::UpdateBlockEntity, this);
    dispatcher->RegisterHandler(protocol::State::Play, protocol::play::Respawn,
                                this);
}

World::~World() { GetDispatcher()->UnregisterHandler(this); }

bool World::SetBlock(Vector3i position, u32 blockData) {
    ChunkColumnPtr chunk = GetChunk(position);
    if (!chunk) return false;

    Vector3i relative(position);

    relative.x %= 16;
    relative.z %= 16;

    if (relative.x < 0) relative.x += 16;
    if (relative.z < 0) relative.z += 16;

    std::size_t index = (std::size_t)position.y / 16;
    if ((*chunk)[index] == nullptr) {
        ChunkPtr section = std::make_shared<Chunk>();

        (*chunk)[index] = section;
    }

    relative.y %= 16;
    (*chunk)[index]->SetBlock(
        relative, block::BlockRegistry::GetInstance()->GetBlock(blockData));
    return true;
}

void World::HandlePacket(protocol::packets::in::ExplosionPacket* packet) {
    Vector3d position = packet->GetPosition();

    for (Vector3s offset : packet->GetAffectedBlocks()) {
        Vector3d absolute = position + ToVector3d(offset);

        block::BlockPtr oldBlock = GetBlock(absolute);

        // Set all affected blocks to air
        SetBlock(ToVector3i(absolute), 0);

        block::BlockPtr newBlock =
            block::BlockRegistry::GetInstance()->GetBlock(0);
        NotifyListeners(&WorldListener::OnBlockChange, ToVector3i(absolute),
                        newBlock, oldBlock);
    }
}

void World::HandlePacket(protocol::packets::in::ChunkDataPacket* packet) {
    ChunkColumnPtr col = packet->GetChunkColumn();
    const ChunkColumnMetadata& meta = col->GetMetadata();
    ChunkCoord key(meta.x, meta.z);

    if (meta.continuous && meta.sectionmask == 0) {
        m_Chunks[key] = nullptr;
        return;
    }

    if (!m_Chunks[key]) m_Chunks[key] = col;

    for (s32 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
        ChunkPtr chunk = (*col)[i];

        NotifyListeners(&WorldListener::OnChunkLoad, chunk, meta, i);
    }
}

void World::HandlePacket(
    protocol::packets::in::MultiBlockChangePacket* packet) {
    Vector3i chunkStart(packet->GetChunkX() * 16, 0, packet->GetChunkZ() * 16);
    auto iter =
        m_Chunks.find(ChunkCoord(packet->GetChunkX(), packet->GetChunkZ()));
    if (iter == m_Chunks.end()) return;

    ChunkColumnPtr chunk = iter->second;
    if (!chunk) return;

    const auto& changes = packet->GetBlockChanges();
    for (const auto& change : changes) {
        Vector3i relative(change.x, change.y, change.z);

        chunk->RemoveBlockEntity(chunkStart + relative);

        std::size_t index = change.y / 16;
        block::BlockPtr oldBlock =
            block::BlockRegistry::GetInstance()->GetBlock(0);
        if ((*chunk)[index] == nullptr) {
            ChunkPtr section = std::make_shared<Chunk>();

            (*chunk)[index] = section;
        } else {
            oldBlock = chunk->GetBlock(relative);
        }

        block::BlockPtr newBlock =
            block::BlockRegistry::GetInstance()->GetBlock(change.blockData);

        Vector3i blockChangePos = chunkStart + relative;

        relative.y %= 16;
        (*chunk)[index]->SetBlock(relative, newBlock);
        NotifyListeners(&WorldListener::OnBlockChange, blockChangePos, newBlock,
                        oldBlock);
    }
}

void World::HandlePacket(protocol::packets::in::BlockChangePacket* packet) {
    block::BlockPtr newBlock = block::BlockRegistry::GetInstance()->GetBlock(
        (u16)packet->GetBlockId());
    block::BlockPtr oldBlock = GetBlock(packet->GetPosition());

    SetBlock(packet->GetPosition(), packet->GetBlockId());

    NotifyListeners(&WorldListener::OnBlockChange, packet->GetPosition(),
                    newBlock, oldBlock);

    ChunkColumnPtr col = GetChunk(packet->GetPosition());
    if (col) {
        col->RemoveBlockEntity(packet->GetPosition());
    }
}

void World::HandlePacket(
    protocol::packets::in::UpdateBlockEntityPacket* packet) {
    Vector3i pos = packet->GetPosition();

    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return;

    col->RemoveBlockEntity(pos);

    block::BlockEntityPtr entity = packet->GetBlockEntity();
    if (entity) col->AddBlockEntity(entity);
}

void World::HandlePacket(protocol::packets::in::UnloadChunkPacket* packet) {
    ChunkCoord coord(packet->GetChunkX(), packet->GetChunkZ());

    auto iter = m_Chunks.find(coord);

    if (iter == m_Chunks.end()) return;

    ChunkColumnPtr chunk = iter->second;
    NotifyListeners(&WorldListener::OnChunkUnload, chunk);

    m_Chunks.erase(iter);
}

// Clear all chunks because the server will resend the chunks after this.
void World::HandlePacket(protocol::packets::in::RespawnPacket* packet) {
    for (auto entry : m_Chunks) {
        ChunkColumnPtr chunk = entry.second;

        NotifyListeners(&WorldListener::OnChunkUnload, chunk);
    }
    m_Chunks.clear();
}

ChunkColumnPtr World::GetChunk(Vector3i pos) const {
    s32 x = (s32)std::floor(pos.x / 16.0);
    s32 z = (s32)std::floor(pos.z / 16.0);

    ChunkCoord key(x, z);

    auto iter = m_Chunks.find(key);

    if (iter == m_Chunks.end()) return nullptr;

    return iter->second;
}

block::BlockPtr World::GetBlock(Vector3f pos) const {
    return GetBlock(Vector3i((s64)std::floor(pos.x), (s64)std::floor(pos.y),
                             (s64)std::floor(pos.z)));
}

block::BlockPtr World::GetBlock(Vector3d pos) const {
    return GetBlock(Vector3i((s64)std::floor(pos.x), (s64)std::floor(pos.y),
                             (s64)std::floor(pos.z)));
}

block::BlockPtr World::GetBlock(Vector3i pos) const {
    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return block::BlockRegistry::GetInstance()->GetBlock(0);

    s64 x = pos.x % 16;
    s64 z = pos.z % 16;

    if (x < 0) x += 16;
    if (z < 0) z += 16;

    return col->GetBlock(Vector3i(x, pos.y, z));
}

block::BlockEntityPtr World::GetBlockEntity(Vector3i pos) const {
    ChunkColumnPtr col = GetChunk(pos);

    if (!col) return nullptr;

    return col->GetBlockEntity(pos);
}

std::vector<block::BlockEntityPtr> World::GetBlockEntities() const {
    std::vector<block::BlockEntityPtr> blockEntities;

    for (auto iter = m_Chunks.begin(); iter != m_Chunks.end(); ++iter) {
        if (iter->second == nullptr) continue;
        std::vector<block::BlockEntityPtr> chunkBlockEntities =
            iter->second->GetBlockEntities();
        if (chunkBlockEntities.empty()) continue;
        blockEntities.insert(blockEntities.end(), chunkBlockEntities.begin(),
                             chunkBlockEntities.end());
    }

    return blockEntities;
}

}  // namespace world
}  // namespace mc
