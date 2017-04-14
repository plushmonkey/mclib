#include <mclib/block/BlockEntity.h>

#include <unordered_map>

namespace mc {
namespace block {

static const std::unordered_map<std::wstring, BlockEntityType> blockEntityTypes =
{
    { L"minecraft:banner", BlockEntityType::Banner },
    { L"minecraft:beacon", BlockEntityType::Beacon },
    { L"minecraft:cauldron", BlockEntityType::Cauldron },
    { L"minecraft:brewing_stand", BlockEntityType::BrewingStand },
    { L"minecraft:chest", BlockEntityType::Chest },
    { L"minecraft:comparator", BlockEntityType::Comparator },
    { L"minecraft:command_block", BlockEntityType::CommandBlock },
    { L"minecraft:daylight_detector", BlockEntityType::DaylightSensor },
    { L"minecraft:dispenser", BlockEntityType::Dispenser },
    { L"minecraft:dropper", BlockEntityType::Dropper },
    { L"minecraft:enchanting_table", BlockEntityType::EnchantingTable },
    { L"minecraft:ender_chest", BlockEntityType::EnderChest },
    { L"minecraft:end_gateway", BlockEntityType::EndGateway },
    { L"minecraft:end_portal", BlockEntityType::EndPortal },
    { L"minecraft:flower_pot", BlockEntityType::FlowerPot },
    { L"minecraft:furnace", BlockEntityType::Furnace },
    { L"minecraft:hopper", BlockEntityType::Hopper },
    { L"minecraft:jukebox", BlockEntityType::Jukebox },
    { L"minecraft:mob_spawner", BlockEntityType::MobSpawner },
    { L"minecraft:noteblock", BlockEntityType::Noteblock },
    { L"minecraft:piston", BlockEntityType::Piston },
    { L"minecraft:sign", BlockEntityType::Sign },
    { L"minecraft:skull", BlockEntityType::Skull },
    { L"minecraft:structure_block", BlockEntityType::StructureBlock }
};

BlockEntityType GetTypeFromString(const std::wstring& str) {
    auto iter = blockEntityTypes.find(str);
    if (iter == blockEntityTypes.end()) return BlockEntityType::Unknown;

    return iter->second;
}

BlockEntity::BlockEntity(BlockEntityType type, Vector3i position) noexcept
    : m_Type(type), m_Position(position)
{

}

std::unique_ptr<BlockEntity> BlockEntity::CreateFromNBT(nbt::NBT* nbt) {
    auto& root = nbt->GetRoot();

    std::wstring id;
    s32 x, y, z;
    s32 tagCount = 0;
    for (auto iter = root.begin(); iter != root.end(); ++iter) {
        nbt::Tag* tag = iter->second.get();

        if (tag->GetName().compare(L"id") == 0 && tag->GetType() == nbt::TagType::String) {
            id = ((nbt::TagString*)tag)->GetValue();
            tagCount++;
        }

        if (tag->GetName().compare(L"x") == 0 && tag->GetType() == nbt::TagType::Int) {
            x = ((nbt::TagInt*)tag)->GetValue();
            tagCount++;
        }

        if (tag->GetName().compare(L"y") == 0 && tag->GetType() == nbt::TagType::Int) {
            y = ((nbt::TagInt*)tag)->GetValue();
            tagCount++;
        }

        if (tag->GetName().compare(L"z") == 0 && tag->GetType() == nbt::TagType::Int) {
            z = ((nbt::TagInt*)tag)->GetValue();
            tagCount++;
        }
    }

    if (tagCount != 4) return nullptr;

    Vector3i position(x, y, z);
    BlockEntityType type = GetTypeFromString(id);

    std::unique_ptr<BlockEntity> entity;

    switch (type) {
        case BlockEntityType::Sign:
            entity = std::make_unique<SignBlockEntity>(type, position);
            break;
        default:
            entity = std::make_unique<BlockEntity>(type, position);
    }

    if (!entity->ImportNBT(nbt)) {
        // The nbt data didn't contain enough information to construct the block entity
        return nullptr;
    }
    entity->m_NBT = *nbt;
    return entity;
}


bool SignBlockEntity::ImportNBT(nbt::NBT* nbt) {
    auto& root = nbt->GetRoot();
    s32 textCount = 0;

    for (auto iter = root.begin(); iter != root.end(); ++iter) {
        if (iter->second->GetName().find(L"Text") == 0 && iter->second->GetType() == nbt::TagType::String) {
            int index = iter->second->GetName()[4] - L'0' - 1;
            m_Text[index] = ((nbt::TagString*)iter->second.get())->GetValue();
            textCount++;
        }
    }

    return textCount == 4;
}

const std::wstring& SignBlockEntity::GetText(std::size_t index) const {
    return m_Text[index];
}

} // ns block
} // ns mc
