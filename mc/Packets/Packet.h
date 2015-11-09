#ifndef PACKETS_PACKET_H_
#define PACKETS_PACKET_H_

#include "../DataBuffer.h"
#include "../MCString.h"
#include "../Position.h"
#include "../Protocol.h"
#include "../Chunk.h"
#include <map>
#include <json/json.h>

/**
 * Thanks to #mcdevs on irc.freenode.net (http://wiki.vg/Protocol) for protocol information.
 */

namespace Minecraft {
namespace Packets {

class PacketHandler;

class Packet {
protected:
    VarInt m_Id;
    Protocol::State m_ProtocolState;

public:
    Packet() : m_Id(0xFF), m_ProtocolState(Protocol::State::Play) { }
    virtual ~Packet() { }

    Protocol::State GetProtocolState() const { return m_ProtocolState; }
    VarInt GetId() const { return m_Id; }
    virtual DataBuffer Serialize() const = 0;
    virtual bool Deserialize(DataBuffer& data, std::size_t packetLength) = 0;
    virtual void Dispatch(PacketHandler* handler) = 0;
};

class InboundPacket : public Packet {
public:
    virtual ~InboundPacket() { }
    DataBuffer Serialize() const { return DataBuffer(); }
};

class OutboundPacket : public Packet {
public:
    virtual ~OutboundPacket() { }
    bool Deserialize(DataBuffer& data, std::size_t packetLength) { return false; }
    void Dispatch(PacketHandler* handler) {
        throw std::runtime_error("Cannot dispatch an outbound packet.");
    }
};


namespace Inbound {

// Login packets
class DisconnectPacket : public InboundPacket { // 0x00
private:
    MCString m_Reason;

public:
    DisconnectPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    std::wstring GetReason() const { return m_Reason.GetUTF16(); }
};

class EncryptionRequestPacket : public InboundPacket { // 0x01
private:
    MCString m_ServerId;
    std::string m_PublicKey;
    std::string m_VerifyToken;

public:
    EncryptionRequestPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    std::string GetPublicKey() const { return m_PublicKey; }
    std::string GetVerifyToken() const { return m_VerifyToken; }
    MCString GetServerId() const { return m_ServerId; }
};

class LoginSuccessPacket : public InboundPacket { // 0x02
private:
    MCString m_UUID;
    MCString m_Username;

public:
    LoginSuccessPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    std::wstring GetUUID() const { return m_UUID.GetUTF16(); }
    std::wstring GetUsername() const { return m_Username.GetUTF16(); }
};

class SetCompressionPacket : public InboundPacket { // 0x03
private:
    VarInt m_MaxPacketSize;

public:
    SetCompressionPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    // Packets of this size or higher may be compressed
    s64 GetMaxPacketSize() const { return m_MaxPacketSize.GetLong(); }
};

// Play packets

class KeepAlivePacket : public InboundPacket { // 0x00
private:
    s64 m_AliveId;

public:
    KeepAlivePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    s64 GetAliveId() const { return m_AliveId; }
};

class JoinGamePacket : public InboundPacket { // 0x01
private:
    s32 m_EntityId;
    u8 m_Gamemode;
    s8 m_Dimension;
    u8 m_Difficulty;
    u8 m_MaxPlayers;
    MCString m_LevelType;
    bool m_ReducedDebug;

public:
    JoinGamePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    s32 GetEntityId() const { return m_EntityId; }
    u8 GetGamemode() const { return m_Gamemode; }
    s8 GetDimension() const { return m_Dimension; }
    u8 GetDifficulty() const { return m_Difficulty; }
    u8 GetMaxPlayers() const { return m_MaxPlayers; }
    std::wstring GetLevelType() const { return m_LevelType.GetUTF16(); }
    bool GetReducedDebug() const { return m_ReducedDebug; }
};

class ChatPacket : public InboundPacket { // 0x02
public:
    enum ChatPosition { ChatBox, SystemMessage, Hotbar };

private:
    Json::Value m_ChatData;
    ChatPosition m_Position;

public:
    ChatPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    ChatPosition GetChatPosition() const { return m_Position; }
    const Json::Value& GetChatData() const { return m_ChatData; }
};

class TimeUpdatePacket : public InboundPacket { // 0x03
private:
    s64 m_WorldAge;
    s64 m_Time;

public:
    TimeUpdatePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    s64 GetWorldAge() const { return m_WorldAge; }
    s64 GetTime() const { return m_Time; }
};

class EntityEquipmentPacket : public InboundPacket { // 0x04
public:
    enum EquipmentSlot { Held, Boots, Leggings, Chestplate, Helmet };

private:
    EntityId m_EntityId;
    EquipmentSlot m_EquipmentSlot;
    Slot m_Item;

public:
    EntityEquipmentPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    EquipmentSlot GetEquipmentSlot() const { return m_EquipmentSlot; }
    Slot GetItem() const { return m_Item; }
};

class SpawnPositionPacket : public InboundPacket { // 0x05
private:
    Position m_Location;

public:
    SpawnPositionPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    Position GetLocation() const { return m_Location; }
};

class UpdateHealthPacket : public InboundPacket { // 0x06
private:
    float m_Health;
    s32 m_Food;
    float m_Saturation;

public:
    UpdateHealthPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    float GetHealth() const { return m_Health; }
    s32 GetFood() const { return m_Food; }
    float GetSaturation() const { return m_Saturation; }
};

class RespawnPacket : public InboundPacket { // 0x07
private:
    s32 m_Dimension;
    u8 m_Difficulty;
    u8 m_Gamemode;
    std::wstring m_Level;

public:
    RespawnPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    s32 GetDimension() const { return m_Dimension; }
    u8 GetDifficulty() const { return m_Difficulty; }
    u8 GetGamemode() const { return m_Gamemode; }
    const std::wstring& GetLevel() const { return m_Level; }
};

class PlayerPositionAndLookPacket : public InboundPacket { // 0x08
private:
    double m_X, m_Y, m_Z;
    float m_Yaw, m_Pitch;
    u8 m_Flags;

public:
    PlayerPositionAndLookPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    double GetX() const { return m_X; }
    double GetY() const { return m_Y; }
    double GetZ() const { return m_Z; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }
    u8 GetFlags() const { return m_Flags; }
};

class HeldItemChangePacket : public InboundPacket { // 0x09
private:
    u8 m_Slot;

public:
    HeldItemChangePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    // The new slot that the player selected (0-8)
    u8 GetSlot() const { return m_Slot; }
};

class AnimationPacket : public InboundPacket { // 0x0B
public:
    enum Animation { SwingArm, TakeDamage, LeaveBed, EatFood, CriticalEffect, MagicCriticalEffect };

private:
    EntityId m_EntityId;
    Animation m_Animation;

public:
    AnimationPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Animation GetAnimation() const { return m_Animation; }
};

class SpawnPlayerPacket : public InboundPacket { // 0x0C
private:
    EntityId m_EntityId;
    UUID m_UUID;
    float m_X;
    float m_Y;
    float m_Z;
    u8 m_Yaw;
    u8 m_Pitch;
    s16 m_CurrentItem;
    EntityMetadata m_Metadata;

public:
    SpawnPlayerPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    UUID GetUUID() const { return m_UUID; }
    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }
    float GetZ() const { return m_Z; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    s16 GetCurrentItem() const { return m_CurrentItem; }
    const EntityMetadata& GetMetadata() const { return m_Metadata; }
};

class SpawnMobPacket : public InboundPacket { // 0x0F
private:
    EntityId m_EntityId;
    u8 m_Type;
    float m_X;
    float m_Y;
    float m_Z;
    u8 m_Yaw;
    u8 m_Pitch;
    u8 m_HeadPitch;
    s16 m_VelocityX;
    s16 m_VelocityY;
    s16 m_VelocityZ;
    EntityMetadata m_Metadata;

public:
    SpawnMobPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetType() const { return m_Type; }
    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }
    float GetZ() const { return m_Z; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    u8 GetHeadPitch() const { return m_HeadPitch; }
    s16 GetVelocityX() const { return m_VelocityX; }
    s16 GetVelocityY() const { return m_VelocityY; }
    s16 GetVelocityZ() const { return m_VelocityZ; }
    const EntityMetadata& GetMetadata() const { return m_Metadata; }
};

class EntityVelocityPacket : public InboundPacket { // 0x12
private:
    EntityId m_EntityId;
    Vector3s m_Velocity;

public:
    EntityVelocityPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }

    // Units of 1/8000 of a block per tick
    Vector3s GetVelocity() const { return m_Velocity; }
};


class DestroyEntitiesPacket : public InboundPacket { // 0x13
private:
    std::vector<EntityId> m_EntityIds;

public:
    DestroyEntitiesPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    const std::vector<EntityId>& GetEntityIds() const { return m_EntityIds; }
};

class EntityPacket : public InboundPacket { // 0x14
private:
    EntityId m_EntityId;

public:
    EntityPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
};

class EntityRelativeMovePacket : public InboundPacket { // 0x15
private:
    EntityId m_EntityId;
    float m_DeltaX;
    float m_DeltaY;
    float m_DeltaZ;
    bool m_OnGround;

public:
    EntityRelativeMovePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    float GetDeltaX() const { return m_DeltaX; }
    float GetDeltaY() const { return m_DeltaY; }
    float GetDeltaZ() const { return m_DeltaZ; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityLookPacket : public InboundPacket { // 0x16
private:
    EntityId m_EntityId;
    u8 m_Yaw;
    u8 m_Pitch;
    bool m_OnGround;

public:
    EntityLookPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityLookAndRelativeMovePacket : public InboundPacket { // 0x17
private:
    EntityId m_EntityId;
    Vector3f m_DeltaPos;
    u8 m_Yaw;
    u8 m_Pitch;
    bool m_OnGround;

public:
    EntityLookAndRelativeMovePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Vector3f GetDeltaPosition() const { return m_DeltaPos; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityTeleportPacket : public InboundPacket { // 0x18
private:
    EntityId m_EntityId;
    Vector3f m_Position;
    u8 m_Yaw;
    u8 m_Pitch;
    bool m_OnGround;

public:
    EntityTeleportPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Vector3f GetPosition() const { return m_Position; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityHeadLookPacket : public InboundPacket { // 0x19
private:
    EntityId m_EntityId;
    u8 m_Yaw;

public:
    EntityHeadLookPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetYaw() const { return m_Yaw; }
};

class EntityStatusPacket : public InboundPacket { // 0x1A
private:
    EntityId m_EntityId;
    u8 m_Status;

public:
    EntityStatusPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetStatus() const { return m_Status; }
};

class EntityMetadataPacket : public InboundPacket { // 0x1C
private:
    EntityId m_EntityId;
    EntityMetadata m_Metadata;

public:
    EntityMetadataPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    const EntityMetadata& GetMetadata() const { return m_Metadata; }
};

class SetExperiencePacket : public InboundPacket { // 0x1F
private:
    float m_ExperienceBar;
    s32 m_Level;
    s32 m_TotalExperience;

public:
    SetExperiencePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    float GetExperienceBar() const { return m_ExperienceBar; }
    s32 GetLevel() const { return m_Level; }
    s32 GetTotalExperience() const { return m_TotalExperience; }
};

class EntityPropertiesPacket : public InboundPacket { // 0x20
public:
    struct Property {
        struct Modifier {
            UUID uuid;
            double amount;
            u8 operation;
        };

        double value;
        std::vector<Modifier> modifiers;
    };

private:
    EntityId m_EntityId;
    std::map<std::wstring, Property> m_Properties;

public:
    EntityPropertiesPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    const std::map<std::wstring, Property>& GetProperties() const { return m_Properties; }
};

class ChunkDataPacket : public InboundPacket { // 0x21
private:
    ChunkColumnPtr m_ChunkColumn;

public:
    ChunkDataPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    ChunkColumnPtr GetChunkColumn() const { return m_ChunkColumn; }
};

class MultiBlockChangePacket : public InboundPacket { // 0x22
public:
    struct BlockChange {
        // Relative to chunk
        s16 x;
        // Relative to chunk
        s16 y;
        // Relative to chunk
        s16 z;
        s16 blockData;
    };

private:
    s32 m_ChunkX;
    s32 m_ChunkZ;

    std::vector<BlockChange> m_BlockChanges;

public:
    MultiBlockChangePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    s32 GetChunkX() const { return m_ChunkX; }
    s32 GetChunkZ() const { return m_ChunkZ; }
    const std::vector<BlockChange>& GetBlockChanges() const { return m_BlockChanges; }
};

class BlockChangePacket : public InboundPacket { // 0x23
private:
    s16 m_BlockData;
    Vector3i m_Position;

public:
    BlockChangePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    s16 GetBlockData() const { return m_BlockData; }
    Vector3i GetPosition() const { return m_Position; }
};

class MapChunkBulkPacket : public InboundPacket { // 0x26
private:
    bool m_SkyLight;

    std::map<std::pair<s32, s32>, ChunkColumnPtr> m_ChunkColumns;

public:
    MapChunkBulkPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    const std::map<std::pair<s32, s32>, ChunkColumnPtr>& GetChunkColumns() const { return m_ChunkColumns; }
};

class EffectPacket : public InboundPacket { // 0x28
private:
    s32 m_EffectId;
    Vector3i m_Position;
    s32 m_Data;
    bool m_DisableRelativeVolume;

public:
    EffectPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    s32 GetEffectId() const { return m_EffectId; }
    Vector3i GetPosition() const { return m_Position; }
    s32 GetData() const { return m_Data; }
    bool GetDisableRelativeVolume() const { return m_DisableRelativeVolume; }
};

class SoundEffectPacket : public InboundPacket { // 0x29
private:
    std::wstring m_SoundName;
    Vector3i m_Position;
    float m_Volume;
    u8 m_Pitch;

public:
    SoundEffectPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    const std::wstring& GetSoundName() const { return m_SoundName; }
    Vector3i GetPosition() const { return m_Position; }
    float GetVolume() const { return m_Volume; }
    u8 GetPitch() const { return m_Pitch; }
};

class ChangeGameStatePacket : public InboundPacket { // 0x2B
public:
    enum Reason {
        InvalidBed = 0, EndRaining, BeginRaining, ChangeGameMode, EnterCredits, 
        DemoMessage, ArrowHit, FadeValue, FadeTime, PlayMobAppearance = 10
    };

private:
    Reason m_Reason;
    float m_Value;

public:
    ChangeGameStatePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    Reason GetReason() const { return m_Reason; }
    float GetValue() const { return m_Value; }
};

class SetSlotPacket : public InboundPacket { // 0x2F
private:
    u8 m_WindowId;
    s16 m_SlotIndex;
    Slot m_Slot;

public:
    SetSlotPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    // 0 is inventory window
    u8 GetWindowId() const { return m_WindowId; }

    /**
     * 0-3 are armor slots,
     * 4-7 are crafting area slots
     * 8 is crafting result
     * 9 is start of top row of inventory, each row has 9 slots
     * 36 (9*3 + 9) is start of hotbar
     */
    s16 GetSlotIndex() const { return m_SlotIndex; }

    Slot GetSlot() const { return m_Slot; }
};

class WindowItemsPacket : public InboundPacket { // 0x30
private:
    u8 m_WindowId;
    std::vector<Slot> m_Slots;

public:
    WindowItemsPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);
    u8 GetWindowId() const { return m_WindowId; }

    // Contains every slot for the window, even empty ones (-1 itemId in Slot)
    const std::vector<Slot>& GetSlots() const { return m_Slots; }
};

class UpdateBlockEntityPacket : public InboundPacket { // 0x37
public:
    enum Action { 
        SpawnPotentials, 
        CommandBlockText,
        BeaconPowers,
        MobHead,
        FlowerPot,
        BannerData
    };

private:
    Vector3i m_Position;
    Action m_Action;
    NBT::NBT m_NBT;

public:
    UpdateBlockEntityPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    Vector3i GetPosition() const { return m_Position; }
    Action GetAction() const { return m_Action; }
    const NBT::NBT& GetNBT() const { return m_NBT; }
};

class StatisticsPacket : public InboundPacket { // 0x37
public:
    typedef std::map<std::wstring, s32> Statistics;

private:
    Statistics m_Statistics;

public:
    StatisticsPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    const Statistics& GetStatistics() const { return m_Statistics; }
};

class PlayerListItemPacket : public InboundPacket { // 0x38
public:
    enum Action { AddPlayer = 0, UpdateGamemode, UpdateLatency, UpdateDisplay, RemovePlayer };

    struct ActionData {
        UUID uuid;
        std::wstring name;
        std::map<std::wstring, std::wstring> properties;
        s32 gamemode;
        s32 ping;
        std::wstring displayName;
    };
    typedef std::shared_ptr<ActionData> ActionDataPtr;

private:
    Action m_Action;
    std::vector<ActionDataPtr> m_Data;

public:
    PlayerListItemPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    Action GetAction() const { return m_Action; }

    const std::vector<ActionDataPtr>& GetActionData() const { return m_Data; }
};

class PlayerAbilitiesPacket : public InboundPacket { // 0x39
private:
    u8 m_Flags;
    float m_FlyingSpeed;
    float m_WalkingSpeed;

public:
    PlayerAbilitiesPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    u8 GetFlags() const { return m_Flags; }
    float GetFlyingSpeed() const { return m_FlyingSpeed; }
    float GetWalkingSpeed() const { return m_WalkingSpeed; }
};

class PluginMessagePacket : public InboundPacket { // 0x3F
private:
    MCString m_Channel;
    std::string m_Data;

public:
    PluginMessagePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    std::wstring GetChannel() const { return m_Channel.GetUTF16(); }
    std::string GetData() const { return m_Data; }
};

// Same as login disconnectpacket
// class DisconnectPacket : public InboundPacket { // 0x40

class ServerDifficultyPacket : public InboundPacket { // 0x41
private:
    u8 m_Difficulty;

public:
    ServerDifficultyPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    u8 GetDifficulty() const { return m_Difficulty; }
};

class CombatEventPacket : public InboundPacket { // 0x42
public:
    enum Event { EnterCombat, EndCombat, EntityDead };

private:
    Event m_Event;
    s32 m_Duration; // EndCombat only
    EntityId m_PlayerId; // EntityDead only
    EntityId m_EntityId; // EndCombat and EntityDead only
    std::wstring m_Message; // EntityDead only

public:
    CombatEventPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    Event GetEvent() const { return m_Event; }
    s32 GetDuration() const { return m_Duration; }
    EntityId GetPlayerId() const { return m_PlayerId; }
    EntityId GetEntityId() const { return m_EntityId; }
    const std::wstring& GetMessage() const { return m_Message; }
};


class WorldBorderPacket : public InboundPacket { // 0x44
public:
    enum Action { SetSize, LerpSize, SetCenter, Initialize, SetWarningTime, SetWarningBlocks };

private:
    double m_Radius;
    double m_OldRadius;
    double m_X;
    double m_Z;
    s64 m_Speed;
    s32 m_PortalTeleportBoundary;
    s32 m_WarningTime;
    s32 m_WarningBlocks;

    Action m_Action;

public:
    WorldBorderPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    // Radius from center to world border
    double GetRadius() const { return m_Radius; };

    // Old radius from center to world border
    double GetOldRadius() const { return m_OldRadius; };

    // Center X coord
    double GetX() const { return m_X; }

    // Center Y coord
    double GetZ() const { return m_Z; }

    /**
     * Number of real-time ticks/seconds (?) until New Radius is reached. 
     * From experiments, it appears that Notchian server does not sync 
     * world border speed to game ticks, so it gets out of sync with 
     * server lag 
     */
    s64 GetSpeed() const { return m_Speed; }

    /**
     * Resulting coordinates from a portal teleport are limited to +-value. 
     * Usually 29999984.
     */
    s32 GetPortalTeleportBoundary() const { return m_PortalTeleportBoundary; }
    /** 
     * Causes the screen to be tinted red when a contracting world border will reach 
     * the player within the specified time. The default is 15 seconds. 
     * The tint will not display if the user is using fast graphics. 
     * Unit: seconds
     */
    s32 GetWarningTime() const { return m_WarningTime; }
    /**
     * Causes the screen to be tinted red when the player is within 
     * the specified number of blocks from the world border. 
     * The default is 5 blocks. 
     * The tint will not display if the user is using fast graphics.
     */
    s32 GetWarningBlocks() const { return m_WarningBlocks; };

    // Different values are set depending on which action is happening
    Action GetAction() const { return m_Action; }
};


} // ns Inbound

namespace Outbound {

// Handshake packets

class HandshakePacket : public OutboundPacket {
private:
    VarInt m_ProtocolVersion;
    MCString m_Server;
    u16 m_Port;
    VarInt m_NewState;

public:
    HandshakePacket(s32 protocol, std::string server, u16 port, Protocol::State state);
    DataBuffer Serialize() const;
};

// Login packets

class LoginStartPacket : public OutboundPacket {
private:
    MCString m_Name;

public:
    LoginStartPacket(const std::string& name);
    DataBuffer Serialize() const;
};

class EncryptionResponsePacket : public OutboundPacket {
private:
    std::string m_SharedSecret;
    std::string m_VerifyToken;

public:
    EncryptionResponsePacket(const std::string& sharedSecret, const std::string& verifyToken);
    DataBuffer Serialize() const;

    std::string GetSharedSecret() const { return m_SharedSecret; }
    std::string GetVerifyToken() const { return m_VerifyToken; }
};

// Play packets

class KeepAlivePacket : public OutboundPacket { // 0x00
private:
    s64 m_KeepAliveId;

public:
    KeepAlivePacket(s64 id);
    DataBuffer Serialize() const;

    s64 GetKeepAliveId() const { return m_KeepAliveId; }
};

class ChatPacket : public OutboundPacket { // 0x00
private:
    std::wstring m_Message;

public:
    ChatPacket(const std::wstring& message);
    ChatPacket(const std::string& message);
    DataBuffer Serialize() const;

    const std::wstring& GetChatMessage() const { return m_Message; }
};

class UseEntityPacket : public OutboundPacket { // 0x02
public:
    enum Action { Interact, Attack, InteractAt };
private:
    EntityId m_Target;
    Action m_Action;
    Vector3f m_Position;

public:
    UseEntityPacket(EntityId target, Action action, Vector3f position = Vector3f(0, 0, 0));
    DataBuffer Serialize() const;


};


class PlayerPositionAndLookPacket : public OutboundPacket { // 0x06
private:
    double m_X;
    // Foot position
    double m_Y;
    double m_Z;
    float m_Yaw;
    float m_Pitch;

    bool m_OnGround;

public:
    PlayerPositionAndLookPacket(double x, double y, double z, float yaw, float pitch, bool onGround);
    DataBuffer Serialize() const;
    
    double GetX() const { return m_X; }
    double GetY() const { return m_Y; }
    double GetZ() const { return m_Z; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }
    bool IsOnGround() const { return m_OnGround; }
};

class ClientStatusPacket : public OutboundPacket { // 0x16
public:
    enum Action { PerformRespawn, RequestStats, TakingInventoryAchievement };

private:
    Action m_Action;

public:
    ClientStatusPacket(Action action);
    DataBuffer Serialize() const;

    Action GetAction() const { return m_Action; }
};



} // ns Outbound

} // ns Packets
} // ns Minecraft

#endif
