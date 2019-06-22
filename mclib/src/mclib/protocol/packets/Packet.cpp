#include <mclib/protocol/packets/Packet.h>

#include <mclib/core/Connection.h>
#include <mclib/inventory/Slot.h>
#include <mclib/protocol/packets/PacketHandler.h>

#include <iostream>

namespace {

template <typename T>
class FixedPointNumber {
private:
    T m_IntRep;

public:
    FixedPointNumber() : m_IntRep(0) {}
    FixedPointNumber(T intRep) : m_IntRep(intRep) {}

    float GetFloat() const {
        return (float)(m_IntRep >> 5) + (m_IntRep & 31) / 32.0f;
    }

    friend mc::DataBuffer& operator>>(mc::DataBuffer& in,
                                      FixedPointNumber<s8>& fpn);
    friend mc::DataBuffer& operator>>(mc::DataBuffer& in,
                                      FixedPointNumber<s32>& fpn);
};

mc::DataBuffer& operator>>(mc::DataBuffer& in, FixedPointNumber<s8>& fpn) {
    return in >> fpn.m_IntRep;
}

mc::DataBuffer& operator>>(mc::DataBuffer& in, FixedPointNumber<s32>& fpn) {
    return in >> fpn.m_IntRep;
}

}  // namespace

namespace mc {
namespace protocol {
namespace packets {

void Packet::SetConnection(core::Connection* connection) {
    m_Connection = connection;
}
core::Connection* Packet::GetConnection() { return m_Connection; }

namespace in {

// Play packets
SpawnObjectPacket::SpawnObjectPacket() {}

bool SpawnObjectPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    m_EntityId = eid.GetInt();

    data >> m_UUID;
    data >> m_Type;

    double x, y, z;

    data >> x >> y >> z;

    m_Position = Vector3f((float)x, (float)y, (float)z);

    data >> m_Pitch >> m_Yaw;

    data >> m_Data;
    data >> m_Velocity.x >> m_Velocity.y >> m_Velocity.z;

    return true;
}

void SpawnObjectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnExperienceOrbPacket::SpawnExperienceOrbPacket() {}

bool SpawnExperienceOrbPacket::Deserialize(DataBuffer& data,
                                           std::size_t packetLength) {
    VarInt eid;
    double x, y, z;

    data >> eid >> x >> y >> z >> m_Count;

    m_EntityId = eid.GetInt();
    m_Position.x = x;
    m_Position.y = y;
    m_Position.z = z;

    return true;
}

void SpawnExperienceOrbPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnGlobalEntityPacket::SpawnGlobalEntityPacket() {}

bool SpawnGlobalEntityPacket::Deserialize(DataBuffer& data,
                                          std::size_t packetLength) {
    VarInt eid;
    double x, y, z;

    data >> eid >> m_Type;

    data >> x >> y >> z;
    m_Position = Vector3d(x, y, z);

    return true;
}

void SpawnGlobalEntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnMobPacket::SpawnMobPacket()
    : InboundPacket(), m_Metadata(m_ProtocolVersion) {}

bool SpawnMobPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt entityId, type;

    m_Metadata.SetProtocolVersion(m_ProtocolVersion);

    data >> entityId;
    m_EntityId = entityId.GetInt();
    data >> m_UUID;

    data >> type;
    m_Type = type.GetInt();

    double x, y, z;
    data >> x >> y >> z;
    m_Position = Vector3d(x, y, z);

    data >> m_Yaw;
    data >> m_Pitch;
    data >> m_HeadPitch;

    short vx, vy, vz;
    data >> vx >> vy >> vz;
    m_Velocity = Vector3s(vx, vy, vz);
    data >> m_Metadata;

    return true;
}

void SpawnMobPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnPaintingPacket::SpawnPaintingPacket() {}

bool SpawnPaintingPacket::Deserialize(DataBuffer& data,
                                      std::size_t packetLength) {
    VarInt eid;
    MCString title;
    Position position;
    u8 direction;

    data >> eid >> m_UUID;

    if (m_ProtocolVersion >= protocol::Version::Minecraft_1_13_2) {
        VarInt titleId;

        data >> titleId;

        static const std::string paintingNames[] = {"minecraft:kebab",
                                                    "minecraft:aztec",
                                                    "minecraft:alban",
                                                    "minecraft:aztec2",
                                                    "minecraft:bomb",
                                                    "minecraft:plant",
                                                    "minecraft:wasteland",
                                                    "minecraft:pool",
                                                    "minecraft:courbet",
                                                    "minecraft:sea",
                                                    "minecraft:sunset",
                                                    "minecraft:creebet",
                                                    "minecraft:wanderer",
                                                    "minecraft:graham",
                                                    "minecraft:match",
                                                    "minecraft:bust",
                                                    "minecraft:stage",
                                                    "minecraft:void",
                                                    "minecraft:skull_and_roses",
                                                    "minecraft:wither",
                                                    "minecraft:fighters",
                                                    "minecraft:pointer",
                                                    "minecraft:pigscene",
                                                    "minecraft:burning_skull",
                                                    "minecraft:skeleton",
                                                    "minecraft:donkey_kong"};

        title = paintingNames[titleId.GetInt()];
    } else {
        data >> title;
    }

    data >> position >> direction;

    m_EntityId = eid.GetInt();
    m_Title = title.GetUTF16();
    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();
    m_Direction = (Direction)direction;

    return true;
}

void SpawnPaintingPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnPlayerPacket::SpawnPlayerPacket()
    : InboundPacket(), m_Metadata(m_ProtocolVersion) {}

bool SpawnPlayerPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    VarInt eid;

    m_Metadata.SetProtocolVersion(m_ProtocolVersion);

    data >> eid;
    m_EntityId = eid.GetInt();

    data >> m_UUID;

    double x, y, z;
    data >> x >> y >> z;

    m_Position = Vector3d(x, y, z);

    data >> m_Yaw;
    data >> m_Pitch;

    data >> m_Metadata;
    return true;
}

void SpawnPlayerPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

AnimationPacket::AnimationPacket() {}

bool AnimationPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;

    m_EntityId = eid.GetInt();

    u8 anim;
    data >> anim;

    m_Animation = (Animation)anim;
    return true;
}

void AnimationPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

StatisticsPacket::StatisticsPacket() {}

bool StatisticsPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt count;
    data >> count;

    if (m_ProtocolVersion >= Version::Minecraft_1_13_2) {
        for (s32 i = 0; i < count.GetInt(); ++i) {
            VarInt categoryId;
            VarInt statisticId;
            VarInt value;

            data >> categoryId >> statisticId >> value;

            // Custom statistic
            if (statisticId.GetInt() == 8) {
                static const std::wstring statMap[] = {
                    L"minecraft.leave_game",
                    L"minecraft.play_one_minute",
                    L"minecraft.time_since_death",
                    L"minecraft.sneak_Time",
                    L"minecraft.walk_one_cm",
                    L"minecraft.crouch_one_cm",
                    L"minecraft.sprint_one_cm",
                    L"minecraft.swim_one_cm",
                    L"minecraft.fall_one_cm",
                    L"minecraft.climb_one_cm",
                    L"minecraft.fly_one_cm",
                    L"minecraft.dive_one_cm",
                    L"minecraft.minecart_one_cm",
                    L"minecraft.boat_one_cm",
                    L"minecraft.pig_one_cm",
                    L"minecraft.horse_one_cm",
                    L"minecraft.aviate_one_cm",
                    L"minecraft.jump",
                    L"minecraft.drop",
                    L"minecraft.damage_dealt",
                    L"minecraft.damage_taken",
                    L"minecraft.deaths",
                    L"minecraft.mob_kills",
                    L"minecraft.animals_bred",
                    L"minecraft.player_kills",
                    L"minecraft.fish_caught",
                    L"minecraft.talked_to_villager",
                    L"minecraft.traded_with_villager",
                    L"minecraft.eat_cake_slice",
                    L"minecraft.fill_cauldron",
                    L"minecraft.use_cauldron",
                    L"minecraft.clean_armor",
                    L"minecraft.clean_banner",
                    L"minecraft.interact_with_brewingstand",
                    L"minecraft.interact_with_beacon",
                    L"minecraft.inspect_dropper",
                    L"minecraft.inspect_hopper",
                    L"minecraft.inspect_dispenser",
                    L"minecraft.play_noteblock",
                    L"minecraft.tune_noteblock",
                    L"minecraft.pot_flower",
                    L"minecraft.trigger_trapped_chest",
                    L"minecraft.open_enderchest",
                    L"minecraft.enchant_item",
                    L"minecraft.play_record",
                    L"minecraft.interact_with_furnace",
                    L"minecraft.interact_with_crafting_table",
                    L"minecraft.open_chest",
                    L"minecraft.sleep_in_bed",
                    L"minecraft.open_shulker_box"};

                m_Statistics[statMap[statisticId.GetInt()]] = value.GetInt();
            }
        }
    } else {
        for (s32 i = 0; i < count.GetInt(); ++i) {
            MCString name;
            VarInt value;

            data >> name;
            data >> value;

            m_Statistics[name.GetUTF16()] = value.GetInt();
        }
    }

    return true;
}

void StatisticsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

AdvancementsPacket::AdvancementsPacket() {}

bool AdvancementsPacket::Deserialize(DataBuffer& data,
                                     std::size_t packetLength) {
    data >> m_Reset;

    VarInt mappingSize;

    data >> mappingSize;

    for (s32 i = 0; i < mappingSize.GetInt(); ++i) {
        MCString key;
        Advancement advancement;

        data >> key;

        bool hasParent;
        data >> hasParent;

        if (hasParent) {
            MCString parent;
            data >> parent;
            advancement.parentId = parent.GetUTF16();
        }

        bool hasDisplay;
        data >> hasDisplay;
        if (hasDisplay) {
            MCString title, description;
            data >> title >> description;

            advancement.display.title = title.GetUTF16();
            advancement.display.description = description.GetUTF16();

            advancement.display.icon.Deserialize(data, m_ProtocolVersion);

            VarInt frameType;
            data >> frameType;

            advancement.display.frameType = (FrameType)frameType.GetInt();
            data >> advancement.display.flags;

            if ((advancement.display.flags & (s32)Flags::BackgroundTexture) !=
                0) {
                MCString texture;
                data >> texture;

                advancement.display.backgroundTexture = texture.GetUTF16();
            }

            data >> advancement.display.x >> advancement.display.y;
        }

        VarInt criteriaSize;
        data >> criteriaSize;

        for (s32 j = 0; j < criteriaSize.GetInt(); ++j) {
            MCString identifier;
            data >> identifier;
        }

        VarInt requirementSize;
        data >> requirementSize;

        advancement.requirements.resize(requirementSize.GetInt());

        for (s32 j = 0; j < requirementSize.GetInt(); ++j) {
            VarInt size;
            data >> size;

            auto& requirementArray = advancement.requirements[j];
            requirementArray.resize(size.GetInt());

            for (s32 k = 0; k < size.GetInt(); ++k) {
                MCString requirement;

                data >> requirement;

                requirementArray[k] = requirement.GetUTF16();
            }
        }

        m_Advancements.insert(std::make_pair(key.GetUTF16(), advancement));
    }

    VarInt listSize;
    data >> listSize;

    for (s32 i = 0; i < listSize.GetInt(); ++i) {
        MCString identifier;
        data >> identifier;

        m_RemoveIdentifiers.push_back(identifier.GetUTF16());
    }

    VarInt progressSize;
    data >> progressSize;

    for (s32 i = 0; i < progressSize.GetInt(); ++i) {
        // The identifier of the advancement
        MCString key;

        data >> key;

        VarInt size;
        data >> size;

        AdvancementProgress progress;
        for (s32 j = 0; j < size.GetInt(); ++j) {
            MCString criterionIdentifier;
            CriterionProgress criterionProgress;

            data >> criterionIdentifier;

            data >> criterionProgress.achieved;
            if (criterionProgress.achieved) data >> criterionProgress.date;

            progress.insert(std::make_pair(criterionIdentifier.GetUTF16(),
                                           criterionProgress));
        }

        m_Progress.insert(std::make_pair(key.GetUTF16(), progress));
    }

    return true;
}

void AdvancementsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BlockBreakAnimationPacket::BlockBreakAnimationPacket() {}

bool BlockBreakAnimationPacket::Deserialize(DataBuffer& data,
                                            std::size_t packetLength) {
    VarInt eid;
    Position position;

    data >> eid >> position >> m_DestroyStage;

    m_EntityId = eid.GetInt();
    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();

    return true;
}

void BlockBreakAnimationPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateBlockEntityPacket::UpdateBlockEntityPacket() {}

bool UpdateBlockEntityPacket::Deserialize(DataBuffer& data,
                                          std::size_t packetLength) {
    Position pos;
    u8 action;

    data >> pos;
    data >> action;

    nbt::NBT nbt;
    data >> nbt;

    m_BlockEntity = block::BlockEntity::CreateFromNBT(&nbt);
    m_Position = Vector3i(pos.GetX(), pos.GetY(), pos.GetZ());
    m_Action = (Action)action;

    return true;
}

void UpdateBlockEntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BlockActionPacket::BlockActionPacket() {}

bool BlockActionPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    Position position;
    VarInt type;

    data >> position >> m_ActionId >> m_ActionParam >> type;

    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();
    m_BlockType = type.GetInt();

    return true;
}

void BlockActionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BlockChangePacket::BlockChangePacket() {}

bool BlockChangePacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    Position location;
    VarInt blockId;

    data >> location >> blockId;

    m_Position = Vector3i(location.GetX(), location.GetY(), location.GetZ());
    m_BlockId = blockId.GetInt();

    return true;
}
void BlockChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BossBarPacket::BossBarPacket() {}

bool BossBarPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    static int DivisionCounts[] = {0, 6, 10, 12, 20};
    VarInt action;

    data >> m_UUID >> action;
    m_Action = (Action)action.GetInt();

    switch (m_Action) {
        case Action::Add: {
            MCString title;
            VarInt color, divisions;

            data >> title >> m_Health >> color >> divisions >> m_Flags;

            m_Title = title.GetUTF16();
            m_Color = (Color)color.GetInt();
            m_Divisions = DivisionCounts[divisions.GetInt()];
        } break;
        case Action::Remove: {
        } break;
        case Action::UpdateHealth: {
            data >> m_Health;
        } break;
        case Action::UpdateTitle: {
            MCString title;

            data >> title;
            m_Title = title.GetUTF16();
        } break;
        case Action::UpdateStyle: {
            VarInt color, divisions;

            data >> color >> divisions;

            m_Color = (Color)color.GetInt();
            m_Divisions = DivisionCounts[divisions.GetInt()];
        } break;
        case Action::UpdateFlags: {
            data >> m_Flags;
        } break;
    }

    return true;
}
void BossBarPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ServerDifficultyPacket::ServerDifficultyPacket() {}

bool ServerDifficultyPacket::Deserialize(DataBuffer& data,
                                         std::size_t packetLength) {
    data >> m_Difficulty;
    return true;
}

void ServerDifficultyPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TabCompletePacket::TabCompletePacket() {}

bool TabCompletePacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    if (m_ProtocolVersion > protocol::Version::Minecraft_1_12_2) {
        VarInt id, start, length, count;

        data >> id >> start >> length >> count;

        for (s32 i = 0; i < count.GetInt(); ++i) {
            MCString match, tooltip;
            bool hasTooltip;

            data >> match >> hasTooltip;

            if (hasTooltip) {
                data >> tooltip;
            }

            m_Matches.push_back(match.GetUTF16());
        }

    } else {
        VarInt count;

        data >> count;

        for (s32 i = 0; i < count.GetInt(); ++i) {
            MCString match;

            data >> match;

            m_Matches.push_back(match.GetUTF16());
        }
    }

    return true;
}

void TabCompletePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChatPacket::ChatPacket() {}

bool ChatPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString chatData;
    u8 position;

    data >> chatData;
    data >> position;

    m_Position = (ChatPosition)position;

    try {
        m_ChatData = json::parse(chatData.GetUTF8());
    } catch (json::parse_error&) {
    }

    return true;
}

void ChatPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

MultiBlockChangePacket::MultiBlockChangePacket() {}

bool MultiBlockChangePacket::Deserialize(DataBuffer& data,
                                         std::size_t packetLength) {
    data >> m_ChunkX >> m_ChunkZ;
    VarInt count;
    data >> count;

    for (s32 i = 0; i < count.GetInt(); ++i) {
        u8 horizontal;
        u8 y;
        VarInt blockID;

        data >> horizontal >> y >> blockID;

        BlockChange change;
        change.x = horizontal >> 4;
        change.z = horizontal & 15;
        change.y = y;
        change.blockData = blockID.GetShort();

        m_BlockChanges.push_back(change);
    }
    return true;
}

void MultiBlockChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ConfirmTransactionPacket::ConfirmTransactionPacket() {}

bool ConfirmTransactionPacket::Deserialize(DataBuffer& data,
                                           std::size_t packetLength) {
    data >> m_WindowId >> m_Action >> m_Accepted;
    return true;
}

void ConfirmTransactionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CloseWindowPacket::CloseWindowPacket() {}

bool CloseWindowPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    data >> m_WindowId;
    return true;
}

void CloseWindowPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

OpenWindowPacket::OpenWindowPacket() {}

bool OpenWindowPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString type, title;

    data >> m_WindowId >> type >> title >> m_SlotCount;

    m_WindowType = type.GetUTF16();
    m_WindowTitle = title.GetUTF16();

    m_EntityId = 0;
    if (m_WindowType.compare(L"EntityHorse") == 0) data >> m_EntityId;

    return true;
}

void OpenWindowPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

WindowItemsPacket::WindowItemsPacket() {}

bool WindowItemsPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    data >> m_WindowId;
    s16 count;
    data >> count;

    for (s16 i = 0; i < count; ++i) {
        inventory::Slot slot;

        slot.Deserialize(data, m_ProtocolVersion);

        m_Slots.push_back(slot);
    }

    return true;
}

void WindowItemsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

WindowPropertyPacket::WindowPropertyPacket() {}

bool WindowPropertyPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    data >> m_WindowId >> m_Property >> m_Value;
    return true;
}

void WindowPropertyPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetSlotPacket::SetSlotPacket() {}

bool SetSlotPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId;
    data >> m_SlotIndex;
    m_Slot.Deserialize(data, m_ProtocolVersion);
    return true;
}

void SetSlotPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetCooldownPacket::SetCooldownPacket() {}

bool SetCooldownPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    VarInt item, ticks;
    data >> item >> ticks;

    m_ItemId = item.GetInt();
    m_Ticks = ticks.GetInt();

    return true;
}

void SetCooldownPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PluginMessagePacket::PluginMessagePacket() {}

bool PluginMessagePacket::Deserialize(DataBuffer& data,
                                      std::size_t packetLength) {
    std::size_t begin = data.GetReadOffset();

    data >> m_Channel;

    data.ReadSome(m_Data, data.GetSize() - (data.GetReadOffset() - begin) - 1);

    return true;
}

void PluginMessagePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

NamedSoundEffectPacket::NamedSoundEffectPacket() {}

bool NamedSoundEffectPacket::Deserialize(DataBuffer& data,
                                         std::size_t packetLength) {
    MCString name;
    VarInt category;

    data >> name >> category;

    m_Name = name.GetUTF16();
    m_Category = (SoundCategory)category.GetInt();

    FixedPointNumber<u32> x, y, z;

    data >> x >> y >> z;
    m_Position.x = (double)x.GetFloat();
    m_Position.y = (double)y.GetFloat();
    m_Position.z = (double)z.GetFloat();

    data >> m_Volume >> m_Pitch;

    return true;
}

void NamedSoundEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityStatusPacket::EntityStatusPacket() {}

bool EntityStatusPacket::Deserialize(DataBuffer& data,
                                     std::size_t packetLength) {
    data >> m_EntityId;
    data >> m_Status;
    return true;
}

void EntityStatusPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ExplosionPacket::ExplosionPacket() {}

bool ExplosionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    float posX, posY, posZ;
    s32 count;

    data >> posX >> posY >> posZ;

    m_Position = Vector3d(posX, posY, posZ);

    data >> m_Radius;
    data >> count;

    for (s32 i = 0; i < count; ++i) {
        s8 x, y, z;
        data >> x >> y >> z;
        m_AffectedBlocks.push_back(Vector3s(x, y, z));
    }

    float motionX, motionY, motionZ;

    data >> motionX >> motionY >> motionZ;

    m_PlayerMotion = Vector3d(motionX, motionY, motionZ);

    return true;
}

void ExplosionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UnloadChunkPacket::UnloadChunkPacket() {}

bool UnloadChunkPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    data >> m_ChunkX >> m_ChunkZ;
    return true;
}

void UnloadChunkPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChangeGameStatePacket::ChangeGameStatePacket() {}

bool ChangeGameStatePacket::Deserialize(DataBuffer& data,
                                        std::size_t packetLength) {
    u8 reason;
    data >> reason;

    m_Reason = (Reason)reason;

    data >> m_Value;

    return true;
}

void ChangeGameStatePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

KeepAlivePacket::KeepAlivePacket() {}

bool KeepAlivePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    if (this->GetProtocolVersion() < Version::Minecraft_1_12_2) {
        VarInt aliveId;

        data >> aliveId;

        m_AliveId = aliveId.GetLong();
    } else {
        data >> m_AliveId;
    }

    return true;
}

void KeepAlivePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChunkDataPacket::ChunkDataPacket() {}

bool ChunkDataPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    world::ChunkColumnMetadata metadata;

    data >> metadata.x;
    data >> metadata.z;
    data >> metadata.continuous;
    VarInt mask;
    data >> mask;

    metadata.sectionmask = mask.GetInt();

    if (m_Connection)
        metadata.skylight = m_Connection->GetDimension() == 0;
    else
        metadata.skylight = true;

    VarInt size;

    data >> size;

    m_ChunkColumn = std::make_shared<world::ChunkColumn>(metadata);

    data >> *m_ChunkColumn;

    // Skip biome information
    if (metadata.continuous) data.SetReadOffset(data.GetReadOffset() + 256);

    VarInt entities;
    data >> entities;

    s32 entityCount = entities.GetInt();

    for (s32 i = 0; i < entities.GetInt(); ++i) {
        nbt::NBT nbt;

        data >> nbt;

        block::BlockEntityPtr blockEntity =
            block::BlockEntity::CreateFromNBT(&nbt);

        if (blockEntity == nullptr) continue;

        m_BlockEntities.push_back(blockEntity);
        m_ChunkColumn->AddBlockEntity(blockEntity);
    }

    return true;
}

void ChunkDataPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EffectPacket::EffectPacket() {}

bool EffectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EffectId;
    Position pos;
    data >> pos;

    m_Position.x = pos.GetX();
    m_Position.y = pos.GetY();
    m_Position.z = pos.GetZ();

    data >> m_Data;
    data >> m_DisableRelativeVolume;

    return true;
}

void EffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ParticlePacket::ParticlePacket() {}

bool ParticlePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_ParticleId >> m_LongDistance;

    float x, y, z;
    data >> x >> y >> z;
    m_Position = Vector3d(x, y, z);

    float offsetX, offsetY, offsetZ;
    data >> offsetX >> offsetY >> offsetZ;
    m_MaxOffset = Vector3d(offsetX, offsetY, offsetZ);

    data >> m_ParticleData >> m_Count;

    if (m_ParticleId == 36) {  // iconcrack
        for (s32 i = 0; i < 2; ++i) {
            VarInt varData;
            data >> varData;
            m_Data.push_back(varData.GetInt());
        }
    } else if (m_ParticleId == 37 ||
               m_ParticleId == 38) {  // blockcrack || blockdust
        VarInt varData;
        data >> varData;
        m_Data.push_back(varData.GetInt());
    }

    return true;
}

void ParticlePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

JoinGamePacket::JoinGamePacket() {}

bool JoinGamePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EntityId;
    data >> m_Gamemode;
    data >> m_Dimension;
    data >> m_Difficulty;
    data >> m_MaxPlayers;
    data >> m_LevelType;
    data >> m_ReducedDebug;
    return true;
}

void JoinGamePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

MapPacket::MapPacket() {}

bool MapPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt mapId, count;

    data >> mapId >> m_Scale >> m_TrackPosition >> count;
    m_MapId = mapId.GetInt();

    for (s32 i = 0; i < count.GetInt(); ++i) {
        Icon icon;
        data >> icon.direction >> icon.x >> icon.z;

        icon.type = icon.direction & 0x0F;
        icon.direction = (icon.direction & 0xF0) >> 4;

        m_Icons.push_back(icon);
    }

    data >> m_Columns;

    if (m_Columns > 0) {
        VarInt length;

        data >> m_Rows >> m_X >> m_Z >> length;
        data.ReadSome(m_Data, length.GetInt());
    }

    return true;
}

void MapPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityRelativeMovePacket::EntityRelativeMovePacket() {}

bool EntityRelativeMovePacket::Deserialize(DataBuffer& data,
                                           std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    data >> m_Delta.x >> m_Delta.y >> m_Delta.z;
    data >> m_OnGround;

    m_EntityId = eid.GetInt();

    return true;
}

void EntityRelativeMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityLookAndRelativeMovePacket::EntityLookAndRelativeMovePacket() {}

bool EntityLookAndRelativeMovePacket::Deserialize(DataBuffer& data,
                                                  std::size_t packetLength) {
    VarInt eid;

    data >> eid >> m_Delta.x >> m_Delta.y >> m_Delta.z;
    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;

    m_EntityId = eid.GetInt();

    return true;
}

void EntityLookAndRelativeMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityLookPacket::EntityLookPacket() {}

bool EntityLookPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;

    m_EntityId = eid.GetInt();

    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;
    return true;
}

void EntityLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityPacket::EntityPacket() {}

bool EntityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

VehicleMovePacket::VehicleMovePacket() {}

bool VehicleMovePacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    data >> m_Position.x >> m_Position.y >> m_Position.z >> m_Yaw >> m_Pitch;
    return true;
}

void VehicleMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

OpenSignEditorPacket::OpenSignEditorPacket() {}

bool OpenSignEditorPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    Position position;

    data >> position;

    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();
    return true;
}

void OpenSignEditorPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerAbilitiesPacket::PlayerAbilitiesPacket() {}

bool PlayerAbilitiesPacket::Deserialize(DataBuffer& data,
                                        std::size_t packetLength) {
    data >> m_Flags;
    data >> m_FlyingSpeed;
    data >> m_FOVModifier;

    return true;
}

void PlayerAbilitiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CombatEventPacket::CombatEventPacket() {}

bool CombatEventPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    VarInt event;

    data >> event;

    m_Event = (Event)event.GetInt();

    if (m_Event == Event::EndCombat) {
        VarInt duration;

        data >> duration;

        m_Duration = duration.GetInt();
        data >> m_EntityId;
    } else if (m_Event == Event::EntityDead) {
        VarInt pid;
        data >> pid;

        m_PlayerId = pid.GetInt();
        data >> m_EntityId;

        MCString message;
        data >> message;

        m_Message = message.GetUTF16();
    }

    return true;
}

void CombatEventPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerListItemPacket::PlayerListItemPacket() {}

bool PlayerListItemPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    VarInt action;
    VarInt numPlayers;

    data >> action;
    data >> numPlayers;

    m_Action = (Action)action.GetInt();

    for (s32 i = 0; i < numPlayers.GetInt(); ++i) {
        UUID uuid;
        data >> uuid;

        ActionDataPtr actionData = std::make_shared<ActionData>();
        actionData->uuid = uuid;

        switch (m_Action) {
            case Action::AddPlayer: {
                MCString name;
                VarInt numProperties;

                data >> name;
                data >> numProperties;

                actionData->name = name.GetUTF16();

                for (s32 j = 0; j < numProperties.GetInt(); ++j) {
                    MCString propertyName;
                    MCString propertyValue;
                    u8 isSigned;
                    MCString signature;

                    data >> propertyName;
                    data >> propertyValue;
                    data >> isSigned;
                    if (isSigned) data >> signature;

                    actionData->properties[propertyName.GetUTF16()] =
                        propertyValue.GetUTF16();
                }

                VarInt gameMode, ping;
                data >> gameMode;
                data >> ping;

                u8 hasDisplayName;
                MCString displayName;

                data >> hasDisplayName;
                if (hasDisplayName) data >> displayName;

                actionData->gamemode = gameMode.GetInt();
                actionData->ping = ping.GetInt();
                actionData->displayName = displayName.GetUTF16();
            } break;
            case Action::UpdateGamemode: {
                VarInt gameMode;
                data >> gameMode;

                actionData->gamemode = gameMode.GetInt();
            } break;
            case Action::UpdateLatency: {
                VarInt ping;
                data >> ping;

                actionData->ping = ping.GetInt();
            } break;
            case Action::UpdateDisplay: {
                u8 hasDisplayName;
                MCString displayName;

                data >> hasDisplayName;
                if (hasDisplayName) data >> displayName;

                actionData->displayName = displayName.GetUTF16();
            } break;
            case Action::RemovePlayer: {
                std::shared_ptr<ActionData> actionData =
                    std::make_shared<ActionData>();
                actionData->uuid = uuid;
            } break;
        }

        m_Data.push_back(actionData);
    }

    return true;
}

void PlayerListItemPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerPositionAndLookPacket::PlayerPositionAndLookPacket() {}

bool PlayerPositionAndLookPacket::Deserialize(DataBuffer& data,
                                              std::size_t packetLength) {
    data >> m_Position.x >> m_Position.y >> m_Position.z;
    data >> m_Yaw >> m_Pitch;
    data >> m_Flags;
    VarInt tid;
    data >> tid;

    m_TeleportId = tid.GetInt();
    return true;
}

void PlayerPositionAndLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UseBedPacket::UseBedPacket() {}

bool UseBedPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    Position location;

    data >> eid >> location;

    m_EntityId = eid.GetInt();
    m_Position.x = location.GetX();
    m_Position.y = location.GetY();
    m_Position.z = location.GetZ();
    return true;
}

void UseBedPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

DestroyEntitiesPacket::DestroyEntitiesPacket() {}

bool DestroyEntitiesPacket::Deserialize(DataBuffer& data,
                                        std::size_t packetLength) {
    VarInt count;

    data >> count;

    for (s32 i = 0; i < count.GetInt(); ++i) {
        VarInt eid;

        data >> eid;

        m_EntityIds.push_back(eid.GetInt());
    }
    return true;
}

void DestroyEntitiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UnlockRecipesPacket::UnlockRecipesPacket() {}

bool UnlockRecipesPacket::Deserialize(DataBuffer& data,
                                      std::size_t packetLength) {
    s16 action;

    data >> action;
    m_Action = (Action)action;

    data >> m_OpenCraftingBook;
    data >> m_Filter;

    VarInt size1;
    data >> size1;

    m_Array1.resize(size1.GetInt());

    for (s32 i = 0; i < size1.GetInt(); ++i) {
        VarInt id;

        data >> id;

        m_Array1[i] = id.GetInt();
    }

    if (m_Action == Action::Initialize) {
        VarInt size2;
        data >> size2;

        m_Array2.resize(size2.GetInt());

        for (s32 i = 0; i < size2.GetInt(); ++i) {
            VarInt id;

            data >> id;

            m_Array2[i] = id.GetInt();
        }
    }

    return true;
}

void UnlockRecipesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

RemoveEntityEffectPacket::RemoveEntityEffectPacket() {}

bool RemoveEntityEffectPacket::Deserialize(DataBuffer& data,
                                           std::size_t packetLength) {
    VarInt eid;
    data >> eid >> m_EffectId;
    m_EntityId = eid.GetInt();
    return true;
}

void RemoveEntityEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ResourcePackSendPacket::ResourcePackSendPacket() {}

bool ResourcePackSendPacket::Deserialize(DataBuffer& data,
                                         std::size_t packetLength) {
    MCString url, hash;

    data >> url >> hash;

    m_Url = url.GetUTF16();
    m_Hash = hash.GetUTF8();

    return true;
}

void ResourcePackSendPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

RespawnPacket::RespawnPacket() {}

bool RespawnPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Dimension;
    data >> m_Difficulty;
    data >> m_Gamemode;

    MCString level;
    data >> level;

    m_Level = level.GetUTF16();

    return true;
}

void RespawnPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityHeadLookPacket::EntityHeadLookPacket() {}

bool EntityHeadLookPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    data >> m_Yaw;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityHeadLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

WorldBorderPacket::WorldBorderPacket() {}

bool WorldBorderPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    VarInt action;

    data >> action;

    m_Action = (Action)action.GetInt();

    switch (m_Action) {
        case Action::SetSize: {
            data >> m_Diameter;
        } break;
        case Action::LerpSize: {
            data >> m_OldDiameter;
            data >> m_Diameter;
            VarLong speed;

            data >> speed;
            m_Speed = speed.GetLong();
        } break;
        case Action::SetCenter: {
            data >> m_X >> m_Z;
        } break;
        case Action::Initialize: {
            data >> m_X >> m_Z;

            data >> m_OldDiameter >> m_Diameter;

            VarLong speed;
            data >> speed;
            m_Speed = speed.GetLong();

            VarInt portalTeleportBoundary, warningTime, warningBlocks;

            data >> portalTeleportBoundary >> warningTime >> warningBlocks;

            m_PortalTeleportBoundary = portalTeleportBoundary.GetInt();
            m_WarningTime = warningTime.GetInt();
            m_WarningBlocks = warningBlocks.GetInt();
        } break;
        case Action::SetWarningTime: {
            VarInt warningTime;
            data >> warningTime;
            m_WarningTime = warningTime.GetInt();
        } break;
        case Action::SetWarningBlocks: {
            VarInt warningBlocks;
            data >> warningBlocks;
            m_WarningBlocks = warningBlocks.GetInt();
        } break;
    }
    return true;
}

void WorldBorderPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CameraPacket::CameraPacket() {}

bool CameraPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt id;
    data >> id;
    m_EntityId = id.GetInt();
    return true;
}

void CameraPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

HeldItemChangePacket::HeldItemChangePacket() {}

bool HeldItemChangePacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    data >> m_Slot;
    return true;
}

void HeldItemChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

DisplayScoreboardPacket::DisplayScoreboardPacket() {}

bool DisplayScoreboardPacket::Deserialize(DataBuffer& data,
                                          std::size_t packetLength) {
    u8 pos;
    MCString name;

    data >> pos >> name;

    m_Position = (ScoreboardPosition)pos;
    m_Name = name.GetUTF16();

    return true;
}

void DisplayScoreboardPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityMetadataPacket::EntityMetadataPacket()
    : InboundPacket(), m_Metadata(m_ProtocolVersion) {}

bool EntityMetadataPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    VarInt eid;

    m_Metadata.SetProtocolVersion(m_ProtocolVersion);

    data >> eid;
    data >> m_Metadata;

    m_EntityId = eid.GetInt();

    return true;
}

void EntityMetadataPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

AttachEntityPacket::AttachEntityPacket() {}

bool AttachEntityPacket::Deserialize(DataBuffer& data,
                                     std::size_t packetLength) {
    data >> m_EntityId;
    data >> m_VehicleId;

    return true;
}

void AttachEntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityVelocityPacket::EntityVelocityPacket() {}

bool EntityVelocityPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    data >> m_Velocity.x;
    data >> m_Velocity.y;
    data >> m_Velocity.z;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityVelocityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityEquipmentPacket::EntityEquipmentPacket() {}

bool EntityEquipmentPacket::Deserialize(DataBuffer& data,
                                        std::size_t packetLength) {
    VarInt eid;
    VarInt equipmentSlot;

    data >> eid;
    data >> equipmentSlot;
    m_Item.Deserialize(data, m_ProtocolVersion);

    m_EntityId = eid.GetInt();
    m_EquipmentSlot = (EquipmentSlot)equipmentSlot.GetInt();

    return true;
}

void EntityEquipmentPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetExperiencePacket::SetExperiencePacket() {}

bool SetExperiencePacket::Deserialize(DataBuffer& data,
                                      std::size_t packetLength) {
    VarInt level, total;
    data >> m_ExperienceBar >> level >> total;
    m_Level = level.GetInt();
    m_TotalExperience = level.GetInt();
    return true;
}

void SetExperiencePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateHealthPacket::UpdateHealthPacket() {}

bool UpdateHealthPacket::Deserialize(DataBuffer& data,
                                     std::size_t packetLength) {
    VarInt food;
    data >> m_Health >> food >> m_Saturation;
    m_Food = food.GetInt();
    return true;
}

void UpdateHealthPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ScoreboardObjectivePacket::ScoreboardObjectivePacket() {}

bool ScoreboardObjectivePacket::Deserialize(DataBuffer& data,
                                            std::size_t packetLength) {
    MCString objective, value, type;
    u8 mode;

    data >> objective >> mode >> value >> type;

    m_Objective = objective.GetUTF16();
    m_Mode = (Mode)mode;
    m_Value = value.GetUTF16();
    m_Type = type.GetUTF16();

    return true;
}

void ScoreboardObjectivePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetPassengersPacket::SetPassengersPacket() {}

bool SetPassengersPacket::Deserialize(DataBuffer& data,
                                      std::size_t packetLength) {
    VarInt eid, count;

    data >> eid >> count;
    m_EntityId = eid.GetInt();

    for (s32 i = 0; i < count.GetInt(); ++i) {
        VarInt peid;

        data >> peid;
        m_Passengers.push_back(peid.GetInt());
    }

    return true;
}

void SetPassengersPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TeamsPacket::TeamsPacket() {}

bool TeamsPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString name;
    u8 mode;

    data >> name >> mode;

    m_TeamName = name.GetUTF16();
    m_Mode = (Mode)mode;

    switch (m_Mode) {
        case Mode::Create: {
            MCString display, prefix, suffix, visbility, collision;
            VarInt count;

            if (m_ProtocolVersion <= protocol::Version::Minecraft_1_12_2) {
                data >> display >> prefix >> suffix >> m_FriendlyFlags >>
                    visbility >> collision >> m_Color >> count;
            } else {
                VarInt formatting;
                u8 friendlyFlags;

                data >> display >> friendlyFlags >> visbility >> collision >>
                    formatting >> prefix >> suffix >> count;
            }

            m_TeamDisplayName = display.GetUTF16();
            m_TeamPrefix = prefix.GetUTF16();
            m_TeamSuffix = suffix.GetUTF16();
            m_TagVisbility = visbility.GetUTF16();
            m_CollisionRule = collision.GetUTF16();

            for (s32 i = 0; i < count.GetInt(); ++i) {
                MCString player;
                data >> player;

                m_Players.push_back(player.GetUTF16());
            }
        } break;
        case Mode::Remove: {
        } break;
        case Mode::Update: {
            MCString display, prefix, suffix, visbility, collision;

            if (m_ProtocolVersion <= protocol::Version::Minecraft_1_12_2) {
                data >> display >> prefix >> suffix >> m_FriendlyFlags >>
                    visbility >> collision >> m_Color;
            } else {
                u8 friendlyFlags;
                VarInt formatting;
                data >> display >> friendlyFlags >> visbility >> collision >>
                    formatting >> prefix >> suffix;
                ;
            }

            m_TeamDisplayName = display.GetUTF16();
            m_TeamPrefix = prefix.GetUTF16();
            m_TeamSuffix = suffix.GetUTF16();
            m_TagVisbility = visbility.GetUTF16();
            m_CollisionRule = collision.GetUTF16();
        } break;
        case Mode::AddPlayer:
        case Mode::RemovePlayer: {
            VarInt count;

            data >> count;

            for (s32 i = 0; i < count.GetInt(); ++i) {
                MCString player;
                data >> player;

                m_Players.push_back(player.GetUTF16());
            }
        } break;
    }

    return true;
}

void TeamsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateScorePacket::UpdateScorePacket() {}

bool UpdateScorePacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    MCString name, objective;
    u8 action;

    data >> name >> action >> objective;

    m_ScoreName = name.GetUTF16();
    m_Action = (Action)action;
    m_Objective = objective.GetUTF16();

    if (m_Action != Action::Remove) {
        VarInt value;
        data >> value;
        m_Value = value.GetInt();
    }
    return true;
}

void UpdateScorePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnPositionPacket::SpawnPositionPacket() {}

bool SpawnPositionPacket::Deserialize(DataBuffer& data,
                                      std::size_t packetLength) {
    data >> m_Location;
    return true;
}

void SpawnPositionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TimeUpdatePacket::TimeUpdatePacket() {}

bool TimeUpdatePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WorldAge;
    data >> m_Time;
    return true;
}

void TimeUpdatePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TitlePacket::TitlePacket() {}

bool TitlePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt action;

    data >> action;

    m_Action = (Action)action.GetInt();

    switch (m_Action) {
        case Action::SetTitle:
        case Action::SetSubtitle:
        case Action::SetActionBar: {
            MCString text;
            data >> text;

            m_Text = text.GetUTF16();
        } break;
        case Action::SetDisplay: {
            data >> m_FadeIn >> m_Stay >> m_FadeOut;
        } break;
        default:
            break;
    }

    return true;
}

void TitlePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SoundEffectPacket::SoundEffectPacket() {}

bool SoundEffectPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    VarInt id, category;

    data >> id >> category;

    m_SoundId = id.GetInt();
    m_Category = (SoundCategory)category.GetInt();

    FixedPointNumber<int> x, y, z;
    data >> x >> y >> z;

    m_Position.x = (double)x.GetFloat();
    m_Position.y = (double)y.GetFloat();
    m_Position.z = (double)z.GetFloat();
    data >> m_Volume;
    data >> m_Pitch;

    return true;
}

void SoundEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerListHeaderAndFooterPacket::PlayerListHeaderAndFooterPacket() {}

bool PlayerListHeaderAndFooterPacket::Deserialize(DataBuffer& data,
                                                  std::size_t packetLength) {
    MCString header, footer;

    data >> header >> footer;

    m_Header = header.GetUTF16();
    m_Footer = footer.GetUTF16();

    return true;
}

void PlayerListHeaderAndFooterPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CollectItemPacket::CollectItemPacket() {}

bool CollectItemPacket::Deserialize(DataBuffer& data,
                                    std::size_t packetLength) {
    VarInt collected, collector, count;

    data >> collected >> collector >> count;

    m_Collected = collected.GetInt();
    m_Collector = collector.GetInt();
    m_PickupCount = count.GetInt();

    return true;
}

void CollectItemPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityTeleportPacket::EntityTeleportPacket() {}

bool EntityTeleportPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    VarInt eid;

    data >> eid;

    m_EntityId = eid.GetInt();

    data >> m_Position.x >> m_Position.y >> m_Position.z;

    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;

    return true;
}

void EntityTeleportPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityPropertiesPacket::EntityPropertiesPacket() {}

bool EntityPropertiesPacket::Deserialize(DataBuffer& data,
                                         std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    m_EntityId = eid.GetInt();

    s32 propertyCount;
    data >> propertyCount;

    for (s32 i = 0; i < propertyCount; ++i) {
        MCString key;
        double value;

        data >> key;
        data >> value;

        mc::entity::Attribute attribute(key.GetUTF16(), value);

        VarInt modifierCount;
        data >> modifierCount;

        for (s32 j = 0; j < modifierCount.GetInt(); ++j) {
            UUID uuid;
            double amount;
            u8 operation;

            data >> uuid;
            data >> amount;
            data >> operation;

            mc::entity::Modifier modifier(
                uuid, amount, (mc::entity::ModifierOperation)operation);

            attribute.AddModifier(modifier);
        }

        m_Properties.insert(std::make_pair(attribute.GetKey(), attribute));
    }
    return true;
}

void EntityPropertiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityEffectPacket::EntityEffectPacket() {}

bool EntityEffectPacket::Deserialize(DataBuffer& data,
                                     std::size_t packetLength) {
    VarInt eid, duration;
    data >> eid >> m_EffectId >> m_Amplifier >> duration >> m_Flags;
    m_EntityId = eid.GetInt();
    m_Duration = duration.GetInt();
    return true;
}

void EntityEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

AdvancementProgressPacket::AdvancementProgressPacket() {}

bool AdvancementProgressPacket::Deserialize(DataBuffer& data,
                                            std::size_t packetLength) {
    bool hasId;
    data >> hasId;

    if (hasId) {
        MCString id;

        data >> id;

        m_Id = id.GetUTF16();
    }

    return true;
}

void AdvancementProgressPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CraftRecipeResponsePacket::CraftRecipeResponsePacket() {}

bool CraftRecipeResponsePacket::Deserialize(DataBuffer& data,
                                            std::size_t packetLength) {
    VarInt recipeId;

    data >> m_WindowId >> recipeId;

    m_RecipeId = recipeId.GetInt();

    return true;
}

void CraftRecipeResponsePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

// Login packets
DisconnectPacket::DisconnectPacket() {
    m_ProtocolState = protocol::State::Login;
}

bool DisconnectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    // Update the protocol state so the login and play versions of this are
    // handled correctly.
    if (m_Connection) m_ProtocolState = m_Connection->GetProtocolState();

    data >> m_Reason;
    return true;
}

void DisconnectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EncryptionRequestPacket::EncryptionRequestPacket() {
    m_ProtocolState = protocol::State::Login;
}

bool EncryptionRequestPacket::Deserialize(DataBuffer& data,
                                          std::size_t packetLength) {
    VarInt pubKeyLen;
    VarInt verifyTokenLen;

    data >> m_ServerId;

    data >> pubKeyLen;
    data.ReadSome(m_PublicKey, pubKeyLen.GetInt());

    data >> verifyTokenLen;
    data.ReadSome(m_VerifyToken, verifyTokenLen.GetInt());

    return true;
}

void EncryptionRequestPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

LoginSuccessPacket::LoginSuccessPacket() {
    m_ProtocolState = protocol::State::Login;
}

bool LoginSuccessPacket::Deserialize(DataBuffer& data,
                                     std::size_t packetLength) {
    data >> m_UUID;
    data >> m_Username;
    return true;
}

void LoginSuccessPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetCompressionPacket::SetCompressionPacket() {
    m_ProtocolState = protocol::State::Login;
}

bool SetCompressionPacket::Deserialize(DataBuffer& data,
                                       std::size_t packetLength) {
    data >> m_MaxPacketSize;
    return true;
}

void SetCompressionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

namespace status {

ResponsePacket::ResponsePacket() { m_ProtocolState = protocol::State::Status; }

bool ResponsePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString response;

    data >> response;

    m_Response = response.GetUTF16();

    return true;
}

void ResponsePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PongPacket::PongPacket() { m_ProtocolState = protocol::State::Status; }

bool PongPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Payload;

    return true;
}

void PongPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

}  // namespace status

}  // namespace in

namespace out {

// Handshake packets
HandshakePacket::HandshakePacket(s32 protocol, std::string server, u16 port,
                                 protocol::State state)
    : m_ProtocolVersion(protocol),
      m_Server(server),
      m_Port(port),
      m_NewState((s32)state) {}

DataBuffer HandshakePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_ProtocolVersion << m_Server << m_Port << m_NewState;

    return buffer;
}

// Login packets

LoginStartPacket::LoginStartPacket(const std::string& name) : m_Name(name) {}

DataBuffer LoginStartPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_Name;
    return buffer;
}

EncryptionResponsePacket::EncryptionResponsePacket(
    const std::string& sharedSecret, const std::string& verifyToken)
    : m_SharedSecret(sharedSecret), m_VerifyToken(verifyToken) {}

DataBuffer EncryptionResponsePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;

    VarInt sharedLength = (s32)m_SharedSecret.length();
    VarInt verifyLength = (s32)m_VerifyToken.length();
    buffer << sharedLength;
    buffer << m_SharedSecret;
    buffer << verifyLength;
    buffer << m_VerifyToken;
    return buffer;
}

// Play packets

TeleportConfirmPacket::TeleportConfirmPacket(s32 teleportId)
    : m_TeleportId(teleportId) {}

DataBuffer TeleportConfirmPacket::Serialize() const {
    DataBuffer buffer;
    VarInt teleportId(m_TeleportId);

    buffer << m_Id;
    buffer << teleportId;

    return buffer;
}

PrepareCraftingGridPacket::PrepareCraftingGridPacket(
    u8 windowId, s16 actionNumber, const std::vector<Entry>& returnEntries,
    const std::vector<Entry>& prepareEntries)
    : m_WindowId(windowId),
      m_ActionNumber(actionNumber),
      m_ReturnEntries(returnEntries),
      m_PrepareEntries(prepareEntries) {}

DataBuffer PrepareCraftingGridPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_WindowId;
    buffer << m_ActionNumber;

    s16 returnSize = static_cast<s16>(m_ReturnEntries.size());

    buffer << returnSize;
    for (auto&& entry : m_ReturnEntries) {
        buffer << entry.item.Serialize(m_ProtocolVersion);
        buffer << entry.craftingSlot;
        buffer << entry.playerSlot;
    }

    s16 prepareSize = static_cast<s16>(m_PrepareEntries.size());

    buffer << prepareSize;
    for (auto&& entry : m_PrepareEntries) {
        buffer << entry.item.Serialize(m_ProtocolVersion);
        buffer << entry.craftingSlot;
        buffer << entry.playerSlot;
    }

    return buffer;
}

CraftRecipeRequestPacket::CraftRecipeRequestPacket(u8 windowId, s32 recipeId,
                                                   bool makeAll)
    : m_WindowId(windowId), m_RecipeId(recipeId), m_MakeAll(makeAll) {}

DataBuffer CraftRecipeRequestPacket::Serialize() const {
    VarInt recipeId(m_RecipeId);
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_WindowId;
    buffer << recipeId;
    buffer << m_MakeAll;

    return buffer;
}

TabCompletePacket::TabCompletePacket(const std::wstring& text,
                                     bool assumeCommand)
    : m_Text(text), m_AssumeCommand(assumeCommand), m_HasPosition(false) {}

TabCompletePacket::TabCompletePacket(const std::wstring& text,
                                     bool assumeCommand, bool hasPosition,
                                     Position lookingAt)
    : m_Text(text),
      m_AssumeCommand(assumeCommand),
      m_HasPosition(hasPosition),
      m_LookingAt(lookingAt) {}

DataBuffer TabCompletePacket::Serialize() const {
    DataBuffer buffer;
    MCString text(m_Text);

    buffer << m_Id << text;

    if (m_ProtocolVersion <= protocol::Version::Minecraft_1_12_2) {
        buffer << m_AssumeCommand << m_HasPosition;

        if (m_HasPosition) buffer << m_LookingAt;
    }

    return buffer;
}

ChatPacket::ChatPacket(const std::wstring& message) : m_Message(message) {}

ChatPacket::ChatPacket(const std::string& message)
    : m_Message(message.begin(), message.end()) {}

DataBuffer ChatPacket::Serialize() const {
    MCString out(m_Message);
    DataBuffer buffer;

    buffer << m_Id;
    buffer << out;

    return buffer;
}

ClientStatusPacket::ClientStatusPacket(Action action) : m_Action(action) {}

DataBuffer ClientStatusPacket::Serialize() const {
    VarInt action(m_Action);
    DataBuffer buffer;

    buffer << m_Id;
    buffer << action;

    return buffer;
}

ClientSettingsPacket::ClientSettingsPacket(const std::wstring& locale,
                                           u8 viewDistance, ChatMode chatMode,
                                           bool chatColors, u8 skinFlags,
                                           MainHand hand)
    : m_Locale(locale),
      m_ViewDistance(viewDistance),
      m_ChatMode(chatMode),
      m_ChatColors(chatColors),
      m_SkinFlags(skinFlags),
      m_MainHand(hand) {}

DataBuffer ClientSettingsPacket::Serialize() const {
    MCString locale(m_Locale);
    DataBuffer buffer;
    VarInt chatMode((int)m_ChatMode);
    VarInt hand((int)m_MainHand);

    buffer << m_Id;
    buffer << locale;
    buffer << m_ViewDistance;
    buffer << chatMode;
    buffer << m_ChatColors;
    buffer << m_SkinFlags;
    buffer << hand;

    return buffer;
}

ConfirmTransactionPacket::ConfirmTransactionPacket(u8 windowId, s16 action,
                                                   bool accepted)
    : m_WindowId(windowId), m_Action(action), m_Accepted(accepted) {}

DataBuffer ConfirmTransactionPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_WindowId;
    buffer << m_Action;
    buffer << m_Accepted;

    return buffer;
}

EnchantItemPacket::EnchantItemPacket(u8 windowId, u8 enchantmentIndex) {}

DataBuffer EnchantItemPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_WindowId << m_EnchantmentIndex;

    return buffer;
}

ClickWindowPacket::ClickWindowPacket(u8 windowId, u16 slotIndex, u8 button,
                                     u16 action, s32 mode,
                                     inventory::Slot clickedItem)
    : m_WindowId(windowId),
      m_SlotIndex(slotIndex),
      m_Button(button),
      m_Action(action),
      m_Mode(mode),
      m_ClickedItem(clickedItem) {}

DataBuffer ClickWindowPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_WindowId << m_SlotIndex << m_Button << m_Action;
    VarInt mode(m_Mode);
    buffer << mode << m_ClickedItem.Serialize(m_ProtocolVersion);

    return buffer;
}

CloseWindowPacket::CloseWindowPacket(u8 windowId) : m_WindowId(windowId) {}

DataBuffer CloseWindowPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_WindowId;

    return buffer;
}

PluginMessagePacket::PluginMessagePacket(const std::wstring& channel,
                                         const std::string& data)
    : m_Channel(channel), m_Data(data) {}

DataBuffer PluginMessagePacket::Serialize() const {
    DataBuffer buffer;
    MCString channel(m_Channel);

    buffer << m_Id << channel;
    buffer << m_Data;

    return buffer;
}

UseEntityPacket::UseEntityPacket(EntityId target, Action action, Hand hand,
                                 Vector3f position)
    : m_Target(target), m_Action(action), m_Hand(hand), m_Position(position) {}

DataBuffer UseEntityPacket::Serialize() const {
    DataBuffer buffer;
    VarInt target(m_Target);
    VarInt type((int)m_Action);

    buffer << m_Id;
    buffer << target;
    buffer << type;

    if (m_Action == Action::InteractAt) {
        buffer << m_Position.x;
        buffer << m_Position.y;
        buffer << m_Position.z;
    }

    if (m_Action == Action::Interact || m_Action == Action::InteractAt) {
        VarInt hand((int)m_Hand);
        buffer << hand;
    }

    return buffer;
}

KeepAlivePacket::KeepAlivePacket(s64 id) : m_KeepAliveId(id) {}

DataBuffer KeepAlivePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;

    if (m_ProtocolVersion < Version::Minecraft_1_12_2) {
        VarInt aliveId(m_KeepAliveId);

        buffer << aliveId;
    } else {
        buffer << m_KeepAliveId;
    }

    return buffer;
}

PlayerPositionPacket::PlayerPositionPacket(Vector3d position, bool onGround)
    : m_Position(position), m_OnGround(onGround) {}

DataBuffer PlayerPositionPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_Position.x << m_Position.y << m_Position.z;
    buffer << m_OnGround;
    return buffer;
}

PlayerPositionAndLookPacket::PlayerPositionAndLookPacket(Vector3d position,
                                                         float yaw, float pitch,
                                                         bool onGround)
    : m_Position(position), m_Yaw(yaw), m_Pitch(pitch), m_OnGround(onGround) {}

DataBuffer PlayerPositionAndLookPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Position.x << m_Position.y << m_Position.z;
    buffer << m_Yaw << m_Pitch;
    buffer << m_OnGround;

    return buffer;
}

PlayerLookPacket::PlayerLookPacket(float yaw, float pitch, bool onGround)
    : m_Yaw(yaw), m_Pitch(pitch), m_OnGround(onGround) {}

DataBuffer PlayerLookPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_Yaw << m_Pitch;
    buffer << m_OnGround;
    return buffer;
}

PlayerPacket::PlayerPacket(bool onGround) : m_OnGround(onGround) {}

DataBuffer PlayerPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_OnGround;
    return buffer;
}

VehicleMovePacket::VehicleMovePacket(Vector3d position, float yaw, float pitch)
    : m_Position(position), m_Yaw(yaw), m_Pitch(pitch) {}

DataBuffer VehicleMovePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Position.x << m_Position.y << m_Position.z;
    buffer << m_Yaw << m_Pitch;

    return buffer;
}

SteerBoatPacket::SteerBoatPacket(bool rightPaddle, bool leftPaddle)
    : m_RightPaddle(rightPaddle), m_LeftPaddle(leftPaddle) {}

DataBuffer SteerBoatPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_RightPaddle << m_LeftPaddle;

    return buffer;
}

PlayerAbilitiesPacket::PlayerAbilitiesPacket(bool isFlying)
    : m_IsFlying(isFlying) {}

DataBuffer PlayerAbilitiesPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;

    u8 flags = (u8)m_IsFlying << 1;
    float flyingSpeed = 0.0f;
    float walkingSpeed = 0.0f;

    buffer << flags << flyingSpeed << walkingSpeed;

    return buffer;
}

PlayerDiggingPacket::PlayerDiggingPacket(Status status, Vector3i position,
                                         Face face)
    : m_Status(status), m_Position(position), m_Face(face) {}

DataBuffer PlayerDiggingPacket::Serialize() const {
    DataBuffer buffer;
    Position location((s32)m_Position.x, (s32)m_Position.y, (s32)m_Position.z);

    buffer << m_Id;
    buffer << (u8)m_Status;
    buffer << location;
    buffer << (u8)m_Face;

    return buffer;
}

EntityActionPacket::EntityActionPacket(EntityId eid, Action action,
                                       s32 actionData)
    : m_EntityId(eid), m_Action(action), m_ActionData(actionData) {}

DataBuffer EntityActionPacket::Serialize() const {
    DataBuffer buffer;
    VarInt eid(m_EntityId);
    VarInt action((s32)m_Action);
    VarInt actionData(m_ActionData);

    buffer << m_Id;
    buffer << eid << action << actionData;

    return buffer;
}

SteerVehiclePacket::SteerVehiclePacket(float sideways, float forward, u8 flags)
    : m_Sideways(sideways), m_Forward(forward), m_Flags(flags) {}

DataBuffer SteerVehiclePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Sideways << m_Forward << m_Flags;

    return buffer;
}

ResourcePackStatusPacket::ResourcePackStatusPacket(Result result)
    : m_Result(result) {}

DataBuffer ResourcePackStatusPacket::Serialize() const {
    DataBuffer buffer;
    VarInt result((int)m_Result);

    buffer << m_Id << result;

    return buffer;
}

HeldItemChangePacket::HeldItemChangePacket(u16 slot) : m_Slot(slot) {}

DataBuffer HeldItemChangePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Slot;

    return buffer;
}

CreativeInventoryActionPacket::CreativeInventoryActionPacket(
    s16 slot, inventory::Slot item)
    : m_Slot(slot), m_Item(item) {}

DataBuffer CreativeInventoryActionPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Slot;
    buffer << m_Item.Serialize(m_ProtocolVersion);

    return buffer;
}

UpdateSignPacket::UpdateSignPacket(Vector3d position, const std::wstring& line1,
                                   const std::wstring& line2,
                                   const std::wstring& line3,
                                   const std::wstring& line4)
    : m_Line1(line1), m_Line2(line2), m_Line3(line3), m_Line4(line4) {
    m_Position = Position((s32)position.x, (s32)position.y, (s32)position.z);
}

DataBuffer UpdateSignPacket::Serialize() const {
    DataBuffer buffer;
    MCString line1(m_Line1);
    MCString line2(m_Line2);
    MCString line3(m_Line3);
    MCString line4(m_Line4);

    buffer << m_Id;
    buffer << m_Position;
    buffer << line1 << line2;
    buffer << line3 << line4;

    return buffer;
}

AnimationPacket::AnimationPacket(Hand hand) : m_Hand(hand) {}

DataBuffer AnimationPacket::Serialize() const {
    DataBuffer buffer;
    VarInt hand((int)m_Hand);

    buffer << m_Id << hand;

    return buffer;
}

SpectatePacket::SpectatePacket(UUID uuid) : m_UUID(uuid) {}

DataBuffer SpectatePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_UUID;

    return buffer;
}

PlayerBlockPlacementPacket::PlayerBlockPlacementPacket(Vector3i position,
                                                       Face face, Hand hand,
                                                       Vector3f cursorPos)
    : m_Position(position),
      m_Face(face),
      m_Hand(hand),
      m_CursorPos(cursorPos) {}

DataBuffer PlayerBlockPlacementPacket::Serialize() const {
    DataBuffer buffer;
    Position location((s32)m_Position.x, (s32)m_Position.y, (s32)m_Position.z);
    VarInt face((u8)m_Face), hand((int)m_Hand);

    buffer << m_Id;
    buffer << location;
    buffer << face;
    buffer << hand;
    buffer << m_CursorPos.x;
    buffer << m_CursorPos.y;
    buffer << m_CursorPos.z;

    return buffer;
}

UseItemPacket::UseItemPacket(Hand hand) : m_Hand(hand) {}

DataBuffer UseItemPacket::Serialize() const {
    DataBuffer buffer;
    VarInt hand((int)m_Hand);

    buffer << m_Id << hand;

    return buffer;
}

namespace status {

RequestPacket::RequestPacket() { m_ProtocolState = protocol::State::Status; }

DataBuffer RequestPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;

    return buffer;
}

PingPacket::PingPacket(s64 payload) : m_Payload(payload) {
    m_ProtocolState = protocol::State::Status;
}

DataBuffer PingPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_Payload;

    return buffer;
}

}  // namespace status

}  // namespace out
}  // namespace packets
}  // namespace protocol
}  // namespace mc
