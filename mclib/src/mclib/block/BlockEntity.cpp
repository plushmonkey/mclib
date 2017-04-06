#include <mclib/block/BlockEntity.h>

#include <map>

namespace mc {
namespace block {

BlockEntityType GetTypeFromString(const std::wstring& str) {
    static const std::map<std::wstring, BlockEntityType> types =
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

    auto iter = types.find(str);
    if (iter == types.end()) return BlockEntityType::Unknown;

    return iter->second;
}

BlockEntity::BlockEntity(BlockEntityType type, Vector3i position)
    : m_Type(type), m_Position(position)
{

}

std::shared_ptr<BlockEntity> BlockEntity::CreateFromNBT(nbt::NBT* nbt) {
    auto& root = nbt->GetRoot();

    std::wstring id;
    s32 x, y, z;
    s32 tagCount = 0;
    for (auto iter = root.begin(); iter != root.end(); ++iter) {
        nbt::Tag* tag = iter->get();

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

    std::shared_ptr<BlockEntity> entity;

    switch (type) {
    case BlockEntityType::Sign:
        entity = std::make_shared<SignBlockEntity>(type, position);
        break;
    default:
        entity = std::make_shared<BlockEntity>(type, position);
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
        if ((*iter)->GetName().find(L"Text") == 0 && (*iter)->GetType() == nbt::TagType::String) {
            int index = (*iter)->GetName()[4] - L'0' - 1;
            m_Text[index] = ((nbt::TagString*)iter->get())->GetValue();
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
