#include <mclib/block/BlockEntity.h>

#include <mclib/block/Banner.h>
#include <mclib/block/Beacon.h>
#include <mclib/block/Bed.h>
#include <mclib/block/BrewingStand.h>
#include <mclib/block/Chest.h>
#include <mclib/block/Dispenser.h>
#include <mclib/block/Dropper.h>
#include <mclib/block/EnchantmentTable.h>
#include <mclib/block/EndGateway.h>
#include <mclib/block/FlowerPot.h>
#include <mclib/block/Furnace.h>
#include <mclib/block/Hopper.h>
#include <mclib/block/Jukebox.h>
#include <mclib/block/MonsterSpawner.h>
#include <mclib/block/NoteBlock.h>
#include <mclib/block/Piston.h>
#include <mclib/block/RedstoneComparator.h>
#include <mclib/block/ShulkerBox.h>
#include <mclib/block/Sign.h>
#include <mclib/block/Skull.h>

#include <unordered_map>

namespace mc {
namespace block {

static const std::unordered_map<std::wstring, BlockEntityType>
    blockEntityTypes = {
        {L"minecraft:banner", BlockEntityType::Banner},
        {L"minecraft:beacon", BlockEntityType::Beacon},
        {L"minecraft:bed", BlockEntityType::Bed},
        {L"minecraft:cauldron", BlockEntityType::Cauldron},
        {L"minecraft:brewing_stand", BlockEntityType::BrewingStand},
        {L"minecraft:chest", BlockEntityType::Chest},
        {L"minecraft:comparator", BlockEntityType::Comparator},
        {L"minecraft:command_block", BlockEntityType::CommandBlock},
        {L"minecraft:daylight_detector", BlockEntityType::DaylightSensor},
        {L"minecraft:dispenser", BlockEntityType::Dispenser},
        {L"minecraft:dropper", BlockEntityType::Dropper},
        {L"minecraft:enchanting_table", BlockEntityType::EnchantingTable},
        {L"minecraft:ender_chest", BlockEntityType::EnderChest},
        {L"minecraft:end_gateway", BlockEntityType::EndGateway},
        {L"minecraft:end_portal", BlockEntityType::EndPortal},
        {L"minecraft:flower_pot", BlockEntityType::FlowerPot},
        {L"minecraft:furnace", BlockEntityType::Furnace},
        {L"minecraft:hopper", BlockEntityType::Hopper},
        {L"minecraft:jukebox", BlockEntityType::Jukebox},
        {L"minecraft:mob_spawner", BlockEntityType::MonsterSpawner},
        {L"minecraft:noteblock", BlockEntityType::Noteblock},
        {L"minecraft:piston", BlockEntityType::Piston},
        {L"minecraft:sign", BlockEntityType::Sign},
        {L"minecraft:skull", BlockEntityType::Skull},
        {L"minecraft:structure_block", BlockEntityType::StructureBlock},
        {L"minecraft:trapped_chest", BlockEntityType::TrappedChest}};

BlockEntityType GetTypeFromString(const std::wstring& str) {
    auto iter = blockEntityTypes.find(str);
    if (iter == blockEntityTypes.end()) return BlockEntityType::Unknown;

    return iter->second;
}

BlockEntity::BlockEntity(BlockEntityType type, Vector3i position) noexcept
    : m_Type(type), m_Position(position) {}

std::unique_ptr<BlockEntity> BlockEntity::CreateFromNBT(nbt::NBT* nbt) {
    auto idTag = nbt->GetTag<nbt::TagString>(L"id");
    auto xTag = nbt->GetTag<nbt::TagInt>(L"x");
    auto yTag = nbt->GetTag<nbt::TagInt>(L"y");
    auto zTag = nbt->GetTag<nbt::TagInt>(L"z");

    if (idTag == nullptr || xTag == nullptr || yTag == nullptr ||
        zTag == nullptr)
        return nullptr;

    std::wstring id = idTag->GetValue();
    auto x = xTag->GetValue();
    auto y = yTag->GetValue();
    auto z = zTag->GetValue();

    Vector3i position(x, y, z);
    BlockEntityType type = GetTypeFromString(id);

    std::unique_ptr<BlockEntity> entity;

    switch (type) {
        case BlockEntityType::Banner:
            entity = std::make_unique<Banner>(type, position);
            break;
        case BlockEntityType::Beacon:
            entity = std::make_unique<Beacon>(type, position);
            break;
        case BlockEntityType::Bed:
            entity = std::make_unique<Bed>(type, position);
            break;
        case BlockEntityType::BrewingStand:
            entity = std::make_unique<BrewingStand>(type, position);
            break;
        case BlockEntityType::Chest:
            entity = std::make_unique<Chest>(type, position);
            break;
        case BlockEntityType::Comparator:
            entity = std::make_unique<RedstoneComparator>(type, position);
            break;
        case BlockEntityType::Dispenser:
            entity = std::make_unique<Dispenser>(type, position);
            break;
        case BlockEntityType::Dropper:
            entity = std::make_unique<Dropper>(type, position);
            break;
        case BlockEntityType::EnchantingTable:
            entity = std::make_unique<EnchantmentTable>(type, position);
            break;
        case BlockEntityType::EndGateway:
            entity = std::make_unique<EndGateway>(type, position);
            break;
        case BlockEntityType::FlowerPot:
            entity = std::make_unique<FlowerPot>(type, position);
            break;
        case BlockEntityType::Furnace:
            entity = std::make_unique<Furnace>(type, position);
            break;
        case BlockEntityType::Hopper:
            entity = std::make_unique<Hopper>(type, position);
            break;
        case BlockEntityType::Jukebox:
            entity = std::make_unique<Jukebox>(type, position);
            break;
        case BlockEntityType::MonsterSpawner:
            entity = std::make_unique<MonsterSpawner>(type, position);
            break;
        case BlockEntityType::Noteblock:
            entity = std::make_unique<NoteBlock>(type, position);
            break;
        case BlockEntityType::Piston:
            entity = std::make_unique<Piston>(type, position);
            break;
        case BlockEntityType::ShulkerBox:
            entity = std::make_unique<ShulkerBox>(type, position);
            break;
        case BlockEntityType::Sign:
            entity = std::make_unique<Sign>(type, position);
            break;
        case BlockEntityType::Skull:
            entity = std::make_unique<Skull>(type, position);
            break;
        case BlockEntityType::TrappedChest:
            entity = std::make_unique<Chest>(type, position);
            break;

        default:
            entity = std::make_unique<BlockEntity>(type, position);
    }

    entity->ImportNBT(nbt);
    entity->m_NBT = *nbt;

    return entity;
}

}  // namespace block
}  // namespace mc
