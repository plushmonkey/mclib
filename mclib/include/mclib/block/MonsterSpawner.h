#ifndef MCLIB_BLOCK_MONSTERSPAWNER_H_
#define MCLIB_BLOCK_MONSTERSPAWNER_H_

#include <mclib/block/BlockEntity.h>

#include <unordered_map>

namespace mc {
namespace block {

class MonsterSpawner : public BlockEntity {
public:
    struct SpawnPotential {
        std::wstring type;
        s32 weight;
    };

private:
    std::vector<SpawnPotential> m_SpawnPotentials;
    std::wstring m_EntityId;
    s16 m_SpawnCount;
    s16 m_SpawnRange;
    s16 m_Delay;
    s16 m_MinSpawnDelay;
    s16 m_MaxSpawnDelay;
    s16 m_MaxNearbyEntities;
    s16 m_RequiredPlayerRange;

public:
    MCLIB_API MonsterSpawner(BlockEntityType type, Vector3i position)
        : BlockEntity(type, position) {}
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    inline const std::vector<SpawnPotential>& GetSpawnPotentials() const
        noexcept {
        return m_SpawnPotentials;
    }
    inline const std::wstring& GetEntityId() const noexcept {
        return m_EntityId;
    }
    inline s16 GetSpawnCount() const noexcept { return m_SpawnCount; }
    inline s16 GetSpawnRange() const noexcept { return m_SpawnRange; }
    inline s16 GetDelay() const noexcept { return m_Delay; }
    inline s16 GetMinSpawnDelay() const noexcept { return m_MinSpawnDelay; }
    inline s16 GetMaxSpawnDelay() const noexcept { return m_MaxSpawnDelay; }
    inline s16 GetMaxNearbyEntities() const noexcept {
        return m_MaxNearbyEntities;
    }
    inline s16 GetRequiredPlayerRange() const noexcept {
        return m_RequiredPlayerRange;
    }
};

}  // namespace block
}  // namespace mc

#endif
