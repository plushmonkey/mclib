#include "Packet.h"
#include "PacketHandler.h"

namespace {

template <typename T>
class FixedPointNumber {
private:
    T m_IntRep;
    
public:
    FixedPointNumber() : m_IntRep(0) { }
    FixedPointNumber(T intRep) : m_IntRep(intRep) { }
    
    float GetFloat() const { 
        return (float)(m_IntRep >> 5) + (m_IntRep & 31) / 32.0f;
    }

    friend Minecraft::DataBuffer& operator>>(Minecraft::DataBuffer& in, FixedPointNumber<s8>& fpn);
    friend Minecraft::DataBuffer& operator>>(Minecraft::DataBuffer& in, FixedPointNumber<s32>& fpn);
};

Minecraft::DataBuffer& operator>>(Minecraft::DataBuffer& in, FixedPointNumber<s8>& fpn) {
    return in >> fpn.m_IntRep;
}

Minecraft::DataBuffer& operator>>(Minecraft::DataBuffer& in, FixedPointNumber<s32>& fpn) {
    return in >> fpn.m_IntRep;
}

}

namespace Minecraft {
namespace Packets {

namespace Inbound {

// Play packets
KeepAlivePacket::KeepAlivePacket() {
    m_Id = 0x00;
}

bool KeepAlivePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt aliveId;

    data >> aliveId;

    m_AliveId = aliveId.GetLong();

    return true;
}

void KeepAlivePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

JoinGamePacket::JoinGamePacket() {
    m_Id = 0x01;
}

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

ChatPacket::ChatPacket() {
    m_Id = 0x02;
}

bool ChatPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString chatData;
    u8 position;

    data >> chatData;
    data >> position;

    m_Position = (ChatPosition)position;

    Json::Reader reader;

    reader.parse(chatData.GetUTF8(), m_ChatData);

    return true;
}

void ChatPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TimeUpdatePacket::TimeUpdatePacket() {
    m_Id = 0x03;
}

bool TimeUpdatePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WorldAge;
    data >> m_Time;
    return true;
}

void TimeUpdatePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityEquipmentPacket::EntityEquipmentPacket() {
    m_Id = 0x04;
}

bool EntityEquipmentPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    s16 equipmentSlot;

    data >> eid;
    data >> equipmentSlot;
    data >> m_Item;

    m_EntityId = eid.GetInt();
    m_EquipmentSlot = (EquipmentSlot)equipmentSlot;

    return true;
}

void EntityEquipmentPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnPositionPacket::SpawnPositionPacket() {
    m_Id = 0x05;
}

bool SpawnPositionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Location;
    return true;
}

void SpawnPositionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateHealthPacket::UpdateHealthPacket() {
    m_Id = 0x06;
}

bool UpdateHealthPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt food;
    data >> m_Health >> food >> m_Saturation;
    m_Food = food.GetInt();
    return true;
}

void UpdateHealthPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

RespawnPacket::RespawnPacket() {
    m_Id = 0x07;
}

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

PlayerPositionAndLookPacket::PlayerPositionAndLookPacket() {
    m_Id = 0x08;
}

bool PlayerPositionAndLookPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_X >> m_Y >> m_Z;
    data >> m_Yaw >> m_Pitch;
    data >> m_Flags;
    return true;
}

void PlayerPositionAndLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

HeldItemChangePacket::HeldItemChangePacket() {
    m_Id = 0x09;
}

bool HeldItemChangePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Slot;
    return true;
}

void HeldItemChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

AnimationPacket::AnimationPacket() {
    m_Id = 0x0B;
}

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

SpawnPlayerPacket::SpawnPlayerPacket() {
    m_Id = 0x0C;
}

bool SpawnPlayerPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    data >> m_UUID;

    FixedPointNumber<s32> x, y, z;
    data >> x >> y >> z;

    m_X = x.GetFloat();
    m_Y = y.GetFloat();
    m_Z = z.GetFloat();
    m_EntityId = eid.GetInt();

    data >> m_Yaw;
    data >> m_Pitch;
    data >> m_CurrentItem;
    data >> m_Metadata;
    return true;
}

void SpawnPlayerPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnMobPacket::SpawnMobPacket() {
    m_Id = 0x0F;
}

bool SpawnMobPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt entityId;

    data >> entityId;
    m_EntityId = entityId.GetInt();
    data >> m_Type;

    FixedPointNumber<s32> x, y, z;
    data >> x >> y >> z;

    m_X = x.GetFloat();
    m_Y = y.GetFloat();
    m_Z = z.GetFloat();
    
    data >> m_Yaw;
    data >> m_Pitch;
    data >> m_HeadPitch;
    data >> m_VelocityX;
    data >> m_VelocityY;
    data >> m_VelocityZ;
    data >> m_Metadata;

    return true;
}

void SpawnMobPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityVelocityPacket::EntityVelocityPacket() {
    m_Id = 0x12;
}
bool EntityVelocityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
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

DestroyEntitiesPacket::DestroyEntitiesPacket() {
    m_Id = 0x13;
}
bool DestroyEntitiesPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
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

EntityPacket::EntityPacket() {
    m_Id = 0x14;
}

bool EntityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityRelativeMovePacket::EntityRelativeMovePacket() {
    m_Id = 0x15;
}

bool EntityRelativeMovePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    FixedPointNumber<s8> dx, dy, dz;

    data >> eid;
    data >> dx >> dy >> dz;
    data >> m_OnGround;

    m_DeltaX = dx.GetFloat();
    m_DeltaY = dy.GetFloat();
    m_DeltaZ = dz.GetFloat();

    m_EntityId = eid.GetInt();

    return true;
}

void EntityRelativeMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityLookPacket::EntityLookPacket() {
    m_Id = 0x16;
}

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

EntityLookAndRelativeMovePacket::EntityLookAndRelativeMovePacket() {
    m_Id = 0x17;
}

bool EntityLookAndRelativeMovePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    FixedPointNumber<s8> dx, dy, dz;

    data >> eid >> dx >> dy >> dz;
    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;

    m_EntityId = eid.GetInt();
    m_DeltaPos = Vector3f(dx.GetFloat(), dy.GetFloat(), dz.GetFloat());

    return true;
}

void EntityLookAndRelativeMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityTeleportPacket::EntityTeleportPacket() {
    m_Id = 0x18;
}

bool EntityTeleportPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;

    m_EntityId = eid.GetInt();

    FixedPointNumber<s32> x, y, z;
    data >> x >> y >> z;

    m_Position.x = x.GetFloat();
    m_Position.y = y.GetFloat();
    m_Position.z = z.GetFloat();

    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;

    return true;
}

void EntityTeleportPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityHeadLookPacket::EntityHeadLookPacket() {
    m_Id = 0x19;
}

bool EntityHeadLookPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    data >> m_Yaw;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityHeadLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityStatusPacket::EntityStatusPacket() {
    m_Id = 0x1A;
}

bool EntityStatusPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EntityId; // Should this be VarInt?
    data >> m_Status;
    return true;
}

void EntityStatusPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityMetadataPacket::EntityMetadataPacket() {
    m_Id = 0x1C;
}

bool EntityMetadataPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    
    data >> eid;
    data >> m_Metadata;

    m_EntityId = eid.GetInt();

    return true;
}

void EntityMetadataPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetExperiencePacket::SetExperiencePacket() {
    m_Id = 0x1F;
}

bool SetExperiencePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt level, total;
    data >> m_ExperienceBar >> level >> total;
    m_Level = level.GetInt();
    m_TotalExperience = level.GetInt();
    return true;
}

void SetExperiencePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityPropertiesPacket::EntityPropertiesPacket() {
    m_Id = 0x20;
}

bool EntityPropertiesPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    m_EntityId = eid.GetInt();

    s32 propertyCount;
    data >> propertyCount;

    for (s32 i = 0; i < propertyCount; ++i) {
        Property property;
        MCString key;

        data >> key;
        data >> property.value;

        VarInt modifierCount;
        data >> modifierCount;

        for (s32 j = 0; j < modifierCount.GetInt(); ++j) {
            Property::Modifier modifier;
            data >> modifier.uuid;
            data >> modifier.amount;
            data >> modifier.operation;

            property.modifiers.push_back(modifier);
        }

        m_Properties[key.GetUTF16()] = property;
    }
    return true;
}

void EntityPropertiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChunkDataPacket::ChunkDataPacket() {
    m_Id = 0x21;
}
bool ChunkDataPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    ChunkColumnMetadata metadata;

    data >> metadata.x;
    data >> metadata.z;
    data >> metadata.continuous;
    data >> metadata.sectionmask;

    VarInt size;

    data >> size;

    m_ChunkColumn = std::make_shared<ChunkColumn>(metadata);

    data >> *m_ChunkColumn;

    return true;
}

void ChunkDataPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

MultiBlockChangePacket::MultiBlockChangePacket() {
    m_Id = 0x22;
}

bool MultiBlockChangePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
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

BlockChangePacket::BlockChangePacket() {
    m_Id = 0x23;
}

bool BlockChangePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    Position location;
    VarInt blockData;

    data >> location >> blockData;

    m_Position = Vector3i(location.GetX(), location.GetY(), location.GetZ());
    m_BlockData = blockData.GetShort();

    return true;
}
void BlockChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

MapChunkBulkPacket::MapChunkBulkPacket() {
    m_Id = 0x26;
}

bool MapChunkBulkPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_SkyLight;
    VarInt count;
    data >> count;


    std::vector<ChunkColumnMetadata> metalist;

    // Array of meta
    for (s32 i = 0; i < count.GetInt(); ++i) {
        ChunkColumnMetadata meta;
        data >> meta.x;
        data >> meta.z;
        data >> meta.sectionmask;
        meta.continuous = true;
        meta.skylight = m_SkyLight;
        metalist.push_back(meta);
    }

    // Array of chunk columns
    for (s32 i = 0; i < count.GetInt(); ++i) {
        std::pair<s32, s32> key(metalist[i].x, metalist[i].z);

        m_ChunkColumns[key] = std::make_shared<ChunkColumn>(metalist[i]);

        data >> *m_ChunkColumns[key];
    }

    return true;
}

void MapChunkBulkPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EffectPacket::EffectPacket() {
    m_Id = 0x28;
}

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

SoundEffectPacket::SoundEffectPacket() {
    m_Id = 0x29;
}

bool SoundEffectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString name;

    data >> name;

    m_SoundName = name.GetUTF16();

    data >> m_Position.x;
    data >> m_Position.y;
    data >> m_Position.z;

    m_Position /= 8;

    data >> m_Volume;
    data >> m_Pitch;

    return true;
}

void SoundEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChangeGameStatePacket::ChangeGameStatePacket() {
    m_Id = 0x2B;
}

bool ChangeGameStatePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    u8 reason;
    data >> reason;

    m_Reason = (Reason)reason;

    data >> m_Value;

    return true;
}

void ChangeGameStatePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetSlotPacket::SetSlotPacket() {
    m_Id = 0x2F;
}

bool SetSlotPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId;
    data >> m_SlotIndex;
    data >> m_Slot;
    return true;
}

void SetSlotPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


WindowItemsPacket::WindowItemsPacket() {
    m_Id = 0x30;
}

bool WindowItemsPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId;
    s16 count;
    data >> count;

    for (s16 i = 0; i < count; ++i) {
        Slot slot;
        data >> slot;

        m_Slots.push_back(slot);
    }

    return true;
}

void WindowItemsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateBlockEntityPacket::UpdateBlockEntityPacket() {
    m_Id = 0x35;
}

bool UpdateBlockEntityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    Position pos;
    u8 action;

    data >> pos;
    data >> action;
    data >> m_NBT;

    m_Position = Vector3i(pos.GetX(), pos.GetY(), pos.GetZ());
    m_Action = (Action)action;

    return true;
}

void UpdateBlockEntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

StatisticsPacket::StatisticsPacket() {
    m_Id = 0x37;
}

bool StatisticsPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt count;
    data >> count;

    for (s32 i = 0; i < count.GetInt(); ++i) {
        MCString str;
        VarInt value;

        data >> str;
        data >> value;

        m_Statistics[str.GetUTF16()] = value.GetInt();
    }

    return true;
}

void StatisticsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerListItemPacket::PlayerListItemPacket() {
    m_Id = 0x38;
}

bool PlayerListItemPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt action;
    VarInt numPlayers;

    data >> action;
    data >> numPlayers;

    for (s32 i = 0; i < numPlayers.GetInt(); ++i) {
        UUID uuid;
        data >> uuid;

        ActionDataPtr actionData = std::make_shared<ActionData>();
        actionData->uuid = uuid;

        switch (action.GetInt()) {
            case AddPlayer:
            {
                m_Action = AddPlayer;

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
                    if (isSigned)
                        data >> signature;

                    actionData->properties[propertyName.GetUTF16()] = propertyValue.GetUTF16();
                }

                VarInt gameMode, ping;
                data >> gameMode;
                data >> ping;

                u8 hasDisplayName;
                MCString displayName;

                data >> hasDisplayName;
                if (hasDisplayName)
                    data >> displayName;
                
                actionData->gamemode = gameMode.GetInt();
                actionData->ping = ping.GetInt();
                actionData->displayName = displayName.GetUTF16();
            }
            break;
            case UpdateGamemode:
            {
                m_Action = UpdateGamemode;

                VarInt gameMode;
                data >> gameMode;

                actionData->gamemode = gameMode.GetInt();
            }
            break;
            case UpdateLatency:
            {
                m_Action = UpdateLatency;

                VarInt ping;
                data >> ping;

                actionData->ping = ping.GetInt();
            } 
            break;
            case UpdateDisplay:
            {
                m_Action = UpdateDisplay;

                u8 hasDisplayName;
                MCString displayName;

                data >> hasDisplayName;
                if (hasDisplayName)
                    data >> displayName;

                actionData->displayName = displayName.GetUTF16();
            }
            break;
            case RemovePlayer:
            {
                m_Action = RemovePlayer;

                std::shared_ptr<ActionData> actionData = std::make_shared<ActionData>();
                actionData->uuid = uuid;
            }
            break;
        }

        m_Data.push_back(actionData);
    }

    return true;
}

void PlayerListItemPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerAbilitiesPacket::PlayerAbilitiesPacket() {
    m_Id = 0x39;
}

bool PlayerAbilitiesPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Flags;
    data >> m_FlyingSpeed;
    data >> m_WalkingSpeed;

    return true;
}

void PlayerAbilitiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PluginMessagePacket::PluginMessagePacket() {
    m_Id = 0x3F;
}

bool PluginMessagePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    std::size_t begin = data.GetReadOffset();

    data >> m_Channel;

    data.ReadSome(m_Data, packetLength - (data.GetReadOffset() - begin));

    return true;
}

void PluginMessagePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ServerDifficultyPacket::ServerDifficultyPacket() {
    m_Id = 0x41;
}

bool ServerDifficultyPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Difficulty;
    return true;
}

void ServerDifficultyPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CombatEventPacket::CombatEventPacket() {
    m_Id = 0x42;
}

bool CombatEventPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
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

WorldBorderPacket::WorldBorderPacket() {
    m_Id = 0x44;
}

bool WorldBorderPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt action;

    data >> action;

    m_Action = (Action)action.GetInt();

    switch (m_Action) {
        case SetSize:
        {
            data >> m_Radius;
        }
        break;
        case LerpSize:
        {
            data >> m_OldRadius;
            data >> m_Radius;
            VarLong speed;

            data >> speed;
            m_Speed = speed.GetLong();
        }
        break;
        case SetCenter:
        {
            data >> m_X >> m_Z;
        }
        break;
        case Initialize:
        {
            data >> m_X >> m_Z;

            data >> m_OldRadius >> m_Radius;

            VarLong speed;
            data >> speed;
            m_Speed = speed.GetLong();

            VarInt portalTeleportBoundary, warningTime, warningBlocks;

            data >> portalTeleportBoundary >> warningBlocks >> warningTime;

            m_PortalTeleportBoundary = portalTeleportBoundary.GetInt();
            m_WarningTime = warningTime.GetInt();
            m_WarningBlocks = warningBlocks.GetInt();
        }
        break;
        case SetWarningTime:
        {
            VarInt warningTime;
            data >> warningTime;
            m_WarningTime = warningTime.GetInt();
        }
        break;
        case SetWarningBlocks:
        {
            VarInt warningBlocks;
            data >> warningBlocks;
            m_WarningBlocks = warningBlocks.GetInt();
        }
        break;
    }
    return true;
}

void WorldBorderPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

// Login packets
DisconnectPacket::DisconnectPacket() {
    m_Id = 0x00;
    m_ProtocolState = Minecraft::Protocol::State::Login;
}

bool DisconnectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Reason;
    return true;
}

void DisconnectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EncryptionRequestPacket::EncryptionRequestPacket() {
    m_Id = 0x01;
    m_ProtocolState = Minecraft::Protocol::State::Login;
}

bool EncryptionRequestPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
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
    m_Id = 0x02;
    m_ProtocolState = Minecraft::Protocol::State::Login;
}

bool LoginSuccessPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_UUID;
    data >> m_Username;
    return true;
}

void LoginSuccessPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetCompressionPacket::SetCompressionPacket() {
    m_Id = 0x03;
    m_ProtocolState = Minecraft::Protocol::State::Login;
}

bool SetCompressionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_MaxPacketSize;
    return true;
}

void SetCompressionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


} // ns Inbound

namespace Outbound {

// Handshake packets
HandshakePacket::HandshakePacket(s32 protocol, std::string server, u16 port, Protocol::State state) 
    : m_ProtocolVersion(protocol), m_Server(server), m_Port(port), m_NewState((s32)state)
{
    m_Id = 0x00;
}

DataBuffer HandshakePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_ProtocolVersion << m_Server << m_Port << m_NewState;

    return buffer;
}

// Login packets

LoginStartPacket::LoginStartPacket(const std::string& name) {
    m_Id = 0x00;
    m_Name = name;
}

DataBuffer LoginStartPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_Name;
    return buffer;
}

EncryptionResponsePacket::EncryptionResponsePacket(const std::string& sharedSecret, const std::string& verifyToken)
    : m_SharedSecret(sharedSecret), m_VerifyToken(verifyToken)
{
    m_Id = 0x01;
}

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

KeepAlivePacket::KeepAlivePacket(s64 id) : m_KeepAliveId(id) {
    m_Id = 0x00;
}

DataBuffer KeepAlivePacket::Serialize() const {
    DataBuffer buffer;
    VarInt aliveId(m_KeepAliveId);

    buffer << m_Id;
    buffer << aliveId;

    return buffer;
}

ChatPacket::ChatPacket(const std::wstring& message) : m_Message(message) {
    m_Id = 0x01;
}

ChatPacket::ChatPacket(const std::string& message) : m_Message(message.begin(), message.end()) {
    m_Id = 0x01;
}

DataBuffer ChatPacket::Serialize() const {
    MCString out(m_Message);
    DataBuffer buffer;

    buffer << m_Id;
    buffer << out;

    return buffer;
}

UseEntityPacket::UseEntityPacket(EntityId target, Action action, Vector3f position)
    : m_Target(target), m_Action(action), m_Position(position)
{
    m_Id = 0x02;
}

DataBuffer UseEntityPacket::Serialize() const {
    DataBuffer buffer;
    VarInt target(m_Target);
    VarInt type(m_Action);

    buffer << m_Id;
    buffer << target;
    buffer << type;

    if (m_Action == Action::InteractAt) {
        buffer << m_Position.x;
        buffer << m_Position.y;
        buffer << m_Position.z;
    }

    return buffer;
}

PlayerPositionAndLookPacket::PlayerPositionAndLookPacket(double x, double y, double z, float yaw, float pitch, bool onGround)
    : m_X(x), m_Y(y), m_Z(z), m_Yaw(yaw), m_Pitch(pitch), m_OnGround(onGround)
{
    m_Id = 0x06;
}

DataBuffer PlayerPositionAndLookPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_X << m_Y << m_Z;
    buffer << m_Yaw << m_Pitch;
    buffer << m_OnGround;

    return buffer;
}

ClientStatusPacket::ClientStatusPacket(Action action) {
    m_Id = 0x16;
    m_Action = action;
}

DataBuffer ClientStatusPacket::Serialize() const {
    VarInt action(m_Action);
    DataBuffer buffer;

    buffer << m_Id;
    buffer << action;

    return buffer;
}

} // ns Outbound

} // ns Packets
} // ns Minecraft
