#include <mclib/block/MonsterSpawner.h>

namespace mc {
namespace block {

bool MonsterSpawner::ImportNBT(nbt::NBT* nbt) {
    auto spawnCountTag = nbt->GetTag<nbt::TagShort>(L"SpawnCount");
    if (spawnCountTag) m_SpawnCount = spawnCountTag->GetValue();

    auto spawnRangeTag = nbt->GetTag<nbt::TagShort>(L"SpawnRange");
    if (spawnRangeTag) m_SpawnRange = spawnRangeTag->GetValue();

    auto delayTag = nbt->GetTag<nbt::TagShort>(L"Delay");
    if (delayTag) m_Delay = delayTag->GetValue();

    auto minSpawnDelayTag = nbt->GetTag<nbt::TagShort>(L"MinSpawnDelay");
    if (minSpawnDelayTag) m_MinSpawnDelay = minSpawnDelayTag->GetValue();

    auto maxSpawnDelayTag = nbt->GetTag<nbt::TagShort>(L"MaxSpawnDelay");
    if (maxSpawnDelayTag) m_MaxSpawnDelay = maxSpawnDelayTag->GetValue();

    auto maxNearbyEntitiesTag =
        nbt->GetTag<nbt::TagShort>(L"MaxNearbyEntities");
    if (maxNearbyEntitiesTag)
        m_MaxNearbyEntities = maxNearbyEntitiesTag->GetValue();

    auto requiredPlayerRangeTag =
        nbt->GetTag<nbt::TagShort>(L"RequiredPlayerRange");
    if (requiredPlayerRangeTag)
        m_RequiredPlayerRange = requiredPlayerRangeTag->GetValue();

    auto spawnDataTag = nbt->GetTag<nbt::TagCompound>(L"SpawnData");
    if (spawnDataTag) {
        auto idTag = spawnDataTag->GetTag<nbt::TagString>(L"id");
        if (idTag) {
            m_EntityId = idTag->GetValue();
        }
    }

    return true;
}

}  // namespace block
}  // namespace mc
