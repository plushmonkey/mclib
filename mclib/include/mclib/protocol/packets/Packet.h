#ifndef MCLIB_PROTOCOL_PACKETS_PACKET_H_
#define MCLIB_PROTOCOL_PACKETS_PACKET_H_

#include <mclib/block/BlockEntity.h>
#include <mclib/common/DataBuffer.h>
#include <mclib/common/Json.h>
#include <mclib/common/MCString.h>
#include <mclib/common/Position.h>
#include <mclib/entity/Attribute.h>
#include <mclib/entity/Metadata.h>
#include <mclib/inventory/Slot.h>
#include <mclib/protocol/ProtocolState.h>
#include <mclib/world/Chunk.h>

#include <map>
#include <unordered_map>

/**
 * Thanks to #mcdevs on irc.freenode.net (http://wiki.vg/Protocol) for protocol
 * information.
 */

namespace mc {

enum class SoundCategory {
    Master,
    Music,
    Record,
    Weather,
    Block,
    Hostile,
    Neutral,
    Player,
    Ambient,
    Voice
};

namespace core {

class Connection;

}  // namespace core

namespace protocol {
namespace packets {

class PacketHandler;

class Packet {
protected:
    VarInt m_Id;
    protocol::State m_ProtocolState;
    protocol::Version m_ProtocolVersion;
    // The connection that is processing this packet.
    core::Connection* m_Connection;

public:
    Packet() noexcept
        : m_Id(0xFF),
          m_ProtocolState(protocol::State::Play),
          m_Connection(nullptr),
          m_ProtocolVersion(protocol::Version::Minecraft_1_11_2) {}

    virtual ~Packet() {}

    Packet(const Packet& rhs) = default;
    Packet& operator=(const Packet& rhs) = default;
    Packet(Packet&& rhs) = default;
    Packet& operator=(Packet&& rhs) = default;

    protocol::State GetProtocolState() const noexcept {
        return m_ProtocolState;
    }
    protocol::Version GetProtocolVersion() const noexcept {
        return m_ProtocolVersion;
    }
    VarInt GetId() const noexcept { return m_Id; }

    virtual DataBuffer Serialize() const = 0;
    virtual bool Deserialize(DataBuffer& data, std::size_t packetLength) = 0;
    virtual void Dispatch(PacketHandler* handler) = 0;

    void SetId(s32 id) { m_Id = id; }
    void SetProtocolVersion(protocol::Version version) noexcept {
        m_ProtocolVersion = version;
    }
    MCLIB_API void SetConnection(core::Connection* connection);
    MCLIB_API core::Connection* GetConnection();
};

class InboundPacket : public Packet {
public:
    virtual ~InboundPacket() {}
    DataBuffer Serialize() const { return DataBuffer(); }
};

class OutboundPacket : public Packet {
public:
    virtual ~OutboundPacket() {}

    bool Deserialize(DataBuffer& data, std::size_t packetLength) {
        return false;
    }
    void Dispatch(PacketHandler* handler) {
        throw std::runtime_error("Cannot dispatch an outbound packet.");
    }
};

namespace in {

// Login packets
class DisconnectPacket : public InboundPacket {  // 0x00
private:
    MCString m_Reason;

public:
    MCLIB_API DisconnectPacket();

    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    std::wstring GetReason() const { return m_Reason.GetUTF16(); }
};

class EncryptionRequestPacket : public InboundPacket {  // 0x01
private:
    MCString m_ServerId;
    std::string m_PublicKey;
    std::string m_VerifyToken;

public:
    MCLIB_API EncryptionRequestPacket();

    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    std::string GetPublicKey() const { return m_PublicKey; }
    std::string GetVerifyToken() const { return m_VerifyToken; }
    MCString GetServerId() const { return m_ServerId; }
};

class LoginSuccessPacket : public InboundPacket {  // 0x02
private:
    MCString m_UUID;
    MCString m_Username;

public:
    MCLIB_API LoginSuccessPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    std::wstring GetUUID() const { return m_UUID.GetUTF16(); }
    std::wstring GetUsername() const { return m_Username.GetUTF16(); }
};

class SetCompressionPacket : public InboundPacket {  // 0x03
private:
    VarInt m_MaxPacketSize;

public:
    MCLIB_API SetCompressionPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    // Packets of this size or higher may be compressed
    s64 GetMaxPacketSize() const { return m_MaxPacketSize.GetLong(); }
};

// Play packets

class SpawnObjectPacket : public InboundPacket {  // 0x00
private:
    EntityId m_EntityId;
    UUID m_UUID;
    u8 m_Type;
    Vector3f m_Position;
    u8 m_Pitch;
    u8 m_Yaw;
    s32 m_Data;
    Vector3s m_Velocity;

public:
    MCLIB_API SpawnObjectPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    UUID GetUUID() const { return m_UUID; }
    u8 GetType() const { return m_Type; }
    Vector3f GetPosition() const { return m_Position; }
    u8 GetPitch() const { return m_Pitch; }
    u8 GetYaw() const { return m_Yaw; }

    s32 GetData() const { return m_Data; }
    Vector3s GetVelocity() const { return m_Velocity; }
};

class SpawnExperienceOrbPacket : public InboundPacket {  // 0x01
private:
    EntityId m_EntityId;
    Vector3d m_Position;
    u16 m_Count;

public:
    MCLIB_API SpawnExperienceOrbPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Vector3d GetPosition() const { return m_Position; }
    u16 GetCount() const { return m_Count; }
};

class SpawnGlobalEntityPacket : public InboundPacket {  // 0x02
private:
    EntityId m_EntityId;
    u8 m_Type;
    Vector3d m_Position;

public:
    MCLIB_API SpawnGlobalEntityPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    // Always 1 (thunderbolt)
    u8 GetType() const { return m_Type; }
    Vector3d GetPosition() const { return m_Position; }
};

class SpawnMobPacket : public InboundPacket {  // 0x03
private:
    EntityId m_EntityId;
    UUID m_UUID;
    u32 m_Type;
    Vector3d m_Position;
    u8 m_Yaw;
    u8 m_Pitch;
    u8 m_HeadPitch;
    Vector3s m_Velocity;
    entity::EntityMetadata m_Metadata;

public:
    MCLIB_API SpawnMobPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    UUID GetUUID() const { return m_UUID; }
    u32 GetType() const { return m_Type; }
    Vector3d GetPosition() const { return m_Position; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    u8 GetHeadPitch() const { return m_HeadPitch; }
    Vector3s GetVelocity() const { return m_Velocity; }
    const entity::EntityMetadata& GetMetadata() const { return m_Metadata; }
};

class SpawnPaintingPacket : public InboundPacket {  // 0x04
private:
    enum class Direction { North, West, South, East };
    EntityId m_EntityId;
    UUID m_UUID;
    std::wstring m_Title;
    Vector3i m_Position;
    Direction m_Direction;

public:
    MCLIB_API SpawnPaintingPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    UUID GetUUID() const { return m_UUID; }
    const std::wstring& GetTitle() const { return m_Title; }
    Vector3i GetPosition() const { return m_Position; }
    Direction GetDirection() const { return m_Direction; }
};

class SpawnPlayerPacket : public InboundPacket {  // 0x05
private:
    EntityId m_EntityId;
    UUID m_UUID;
    Vector3d m_Position;
    u8 m_Yaw;
    u8 m_Pitch;
    entity::EntityMetadata m_Metadata;

public:
    MCLIB_API SpawnPlayerPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    UUID GetUUID() const { return m_UUID; }
    Vector3d GetPosition() const { return m_Position; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    const entity::EntityMetadata& GetMetadata() const { return m_Metadata; }
};

class AnimationPacket : public InboundPacket {  // 0x06
public:
    enum class Animation {
        SwingMainArm,
        TakeDamage,
        LeaveBed,
        SwingOffhand,
        CriticalEffect,
        MagicCriticalEffect
    };

private:
    EntityId m_EntityId;
    Animation m_Animation;

public:
    MCLIB_API AnimationPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Animation GetAnimation() const { return m_Animation; }
};

class StatisticsPacket : public InboundPacket {  // 0x07
public:
    typedef std::map<std::wstring, s32> Statistics;

private:
    Statistics m_Statistics;

public:
    MCLIB_API StatisticsPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const Statistics& GetStatistics() const { return m_Statistics; }
};

class AdvancementsPacket : public InboundPacket {
public:
    enum class FrameType { Task, Challenge, Goal };
    enum class Flags {
        BackgroundTexture = 0x01,
        ShowToast = 0x02,
        Hidden = 0x04
    };

    struct AdvancementDisplay {
        std::wstring title;
        std::wstring description;
        inventory::Slot icon;
        FrameType frameType;
        s32 flags;
        std::wstring backgroundTexture;
        float x;
        float y;
    };

    struct CriterionProgress {
        bool achieved;
        s64 date;
    };

    struct Advancement {
        std::wstring parentId;
        AdvancementDisplay display;
        std::vector<std::vector<std::wstring>> requirements;
    };

    using AdvancementProgress = std::map<std::wstring, CriterionProgress>;

private:
    std::map<std::wstring, Advancement> m_Advancements;
    std::vector<std::wstring> m_RemoveIdentifiers;

    std::map<std::wstring, AdvancementProgress> m_Progress;
    bool m_Reset;

public:
    MCLIB_API AdvancementsPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);
};

class BlockBreakAnimationPacket : public InboundPacket {  // 0x08
private:
    EntityId m_EntityId;
    Vector3i m_Position;
    u8 m_DestroyStage;

public:
    MCLIB_API BlockBreakAnimationPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    // EntityId for the break animation
    EntityId GetEntityId() const { return m_EntityId; }
    Vector3i GetPosition() const { return m_Position; }
    // 0-9
    u8 GetDestroyStage() const { return m_DestroyStage; }
};

class UpdateBlockEntityPacket : public InboundPacket {  // 0x09
public:
    enum class Action {
        MobSpawner = 1,
        CommandBlockText,
        BeaconPowers,
        MobHead,
        FlowerPot,
        BannerData,
        StructureData,
        GatewayDestination,
        SignText,
        ShulkerBox,
        BedColor
    };

private:
    Vector3i m_Position;
    Action m_Action;
    block::BlockEntityPtr m_BlockEntity;

public:
    MCLIB_API UpdateBlockEntityPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Vector3i GetPosition() const { return m_Position; }
    Action GetAction() const { return m_Action; }
    block::BlockEntityPtr GetBlockEntity() { return m_BlockEntity; }
};

class BlockActionPacket : public InboundPacket {  // 0x0A
private:
    Vector3i m_Position;
    u8 m_ActionId;
    u8 m_ActionParam;
    s32 m_BlockType;

public:
    MCLIB_API BlockActionPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Vector3i GetPosition() const { return m_Position; }
    u8 GetActionId() const { return m_ActionId; }
    u8 GetActionParam() const { return m_ActionParam; }
    // The block type ID for the block, not including metadata/damage value
    s32 GetBlockType() const { return m_BlockType; }
};

class BlockChangePacket : public InboundPacket {  // 0x0B
private:
    Vector3i m_Position;
    s32 m_BlockId;

public:
    MCLIB_API BlockChangePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Vector3i GetPosition() const { return m_Position; }
    s32 GetBlockId() const { return m_BlockId; }
};

class BossBarPacket : public InboundPacket {  // 0x0C
public:
    enum class Action {
        Add,
        Remove,
        UpdateHealth,
        UpdateTitle,
        UpdateStyle,
        UpdateFlags
    };
    enum class Color { Pink, Blue, Red, Green, Yellow, Purple, White };
    enum class Flag { DarkenSky = 0x01, DragonBar = 0x02 };

private:
    UUID m_UUID;
    Action m_Action;

    std::wstring m_Title;
    float m_Health;
    Color m_Color;
    u32 m_Divisions;
    u8 m_Flags;

public:
    MCLIB_API BossBarPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    UUID GetUUID() const { return m_UUID; }
    Action GetAction() const { return m_Action; }

    const std::wstring& GetTitle() const { return m_Title; }
    float GetHealth() const { return m_Health; }
    Color GetColor() const { return m_Color; }
    u32 GetDivisions() const { return m_Divisions; }

    bool HasFlag(Flag flag) const { return (m_Flags & (int)flag) != 0; }
    u8 GetFlags() const { return m_Flags; }
};

class ServerDifficultyPacket : public InboundPacket {  // 0x0D
private:
    u8 m_Difficulty;

public:
    MCLIB_API ServerDifficultyPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    u8 GetDifficulty() const { return m_Difficulty; }
};

class TabCompletePacket : public InboundPacket {  // 0x0E
private:
    std::vector<std::wstring> m_Matches;

public:
    MCLIB_API TabCompletePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::vector<std::wstring>& GetMatches() const { return m_Matches; }
};

class ChatPacket : public InboundPacket {  // 0x0F
public:
    enum class ChatPosition { ChatBox, SystemMessage, Hotbar };

private:
    json m_ChatData;
    ChatPosition m_Position;

public:
    MCLIB_API ChatPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    ChatPosition GetChatPosition() const { return m_Position; }
    const nlohmann::json& GetChatData() const { return m_ChatData; }
};

class MultiBlockChangePacket : public InboundPacket {  // 0x10
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
    MCLIB_API MultiBlockChangePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetChunkX() const { return m_ChunkX; }
    s32 GetChunkZ() const { return m_ChunkZ; }
    const std::vector<BlockChange>& GetBlockChanges() const {
        return m_BlockChanges;
    }
};

class ConfirmTransactionPacket : public InboundPacket {  // 0x11
private:
    u8 m_WindowId;
    s16 m_Action;
    bool m_Accepted;

public:
    MCLIB_API ConfirmTransactionPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    u8 GetWindowId() const { return m_WindowId; }
    s16 GetAction() const { return m_Action; }
    bool IsAccepted() const { return m_Accepted; }
};

class CloseWindowPacket : public InboundPacket {  // 0x12
private:
    u8 m_WindowId;

public:
    MCLIB_API CloseWindowPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    u8 GetWindowId() const { return m_WindowId; }
};

class OpenWindowPacket : public InboundPacket {  // 0x13
private:
    u8 m_WindowId;
    std::wstring m_WindowType;
    std::wstring m_WindowTitle;
    u8 m_SlotCount;
    // Optional. Only used when window type is "EntityHorse"
    EntityId m_EntityId;

public:
    MCLIB_API OpenWindowPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    u8 GetWindowId() const { return m_WindowId; }
    const std::wstring& GetWindowType() const { return m_WindowType; }
    const std::wstring& GetWindowTitle() const { return m_WindowTitle; }
    u8 GetSlotCount() const { return m_SlotCount; }
    EntityId GetEntityId() const { return m_EntityId; }
};

class WindowItemsPacket : public InboundPacket {  // 0x14
private:
    u8 m_WindowId;
    std::vector<inventory::Slot> m_Slots;

public:
    MCLIB_API WindowItemsPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    u8 GetWindowId() const { return m_WindowId; }

    // Contains every slot for the window, even empty ones (-1 itemId in Slot)
    const std::vector<inventory::Slot>& GetSlots() const { return m_Slots; }
};

class WindowPropertyPacket : public InboundPacket {  // 0x15
private:
    u8 m_WindowId;
    s16 m_Property;
    s16 m_Value;

public:
    MCLIB_API WindowPropertyPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    u8 GetWindowId() const { return m_WindowId; }
    s16 GetProperty() const { return m_Property; }
    s16 GetValue() const { return m_Value; }
};

class SetSlotPacket : public InboundPacket {  // 0x16
private:
    u8 m_WindowId;
    s16 m_SlotIndex;
    inventory::Slot m_Slot;

public:
    MCLIB_API SetSlotPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

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

    inventory::Slot GetSlot() const { return m_Slot; }
};

class SetCooldownPacket : public InboundPacket {  // 0x17
private:
    s32 m_ItemId;
    s32 m_Ticks;

public:
    MCLIB_API SetCooldownPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetItemId() const { return m_ItemId; }
    s32 GetTicks() const { return m_Ticks; }
};

class PluginMessagePacket : public InboundPacket {  // 0x18
private:
    MCString m_Channel;
    std::string m_Data;

public:
    MCLIB_API PluginMessagePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    std::wstring GetChannel() const { return m_Channel.GetUTF16(); }
    std::string GetData() const { return m_Data; }
};

class NamedSoundEffectPacket : public InboundPacket {  // 0x19
private:
    std::wstring m_Name;
    SoundCategory m_Category;
    Vector3d m_Position;
    float m_Volume;
    float m_Pitch;

public:
    MCLIB_API NamedSoundEffectPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::wstring& GetName() const { return m_Name; }
    SoundCategory GetCategory() const { return m_Category; }
    Vector3d GetPosition() const { return m_Position; }
    float GetVolume() const { return m_Volume; }
    float GetPitch() const { return m_Pitch; }
};

// Same as login disconnectpacket
// class DisconnectPacket : public InboundPacket { // 0x1A

class EntityStatusPacket : public InboundPacket {  // 0x1B
private:
    EntityId m_EntityId;
    u8 m_Status;

public:
    MCLIB_API EntityStatusPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetStatus() const { return m_Status; }
};

class ExplosionPacket : public InboundPacket {  // 0x1C
private:
    Vector3d m_Position;
    float m_Radius;
    std::vector<Vector3s> m_AffectedBlocks;
    Vector3d m_PlayerMotion;

public:
    MCLIB_API ExplosionPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    // Position of the center of the explosion
    Vector3d GetPosition() const { return m_Position; }
    float GetRadius() const { return m_Radius; }
    // Offsets of affected blocks. All of the affected blocks should be set to
    // air.
    const std::vector<Vector3s>& GetAffectedBlocks() const {
        return m_AffectedBlocks;
    }
    // velocity of the player being pushed by the explosion
    Vector3d GetPlayerMotion() const { return m_PlayerMotion; }
};

class UnloadChunkPacket : public InboundPacket {  // 0x1D
private:
    s32 m_ChunkX;
    s32 m_ChunkZ;

public:
    MCLIB_API UnloadChunkPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetChunkX() const { return m_ChunkX; }
    s32 GetChunkZ() const { return m_ChunkZ; }
};

class ChangeGameStatePacket : public InboundPacket {  // 0x1E
public:
    enum class Reason {
        InvalidBed = 0,
        EndRaining,
        BeginRaining,
        ChangeGameMode,
        EnterCredits,
        DemoMessage,
        ArrowHit,
        FadeValue,
        FadeTime,
        PlayMobAppearance = 10
    };

private:
    Reason m_Reason;
    float m_Value;

public:
    MCLIB_API ChangeGameStatePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Reason GetReason() const { return m_Reason; }
    float GetValue() const { return m_Value; }
};

class KeepAlivePacket : public InboundPacket {  // 0x1F
private:
    s64 m_AliveId;

public:
    MCLIB_API KeepAlivePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s64 GetAliveId() const { return m_AliveId; }
};

class ChunkDataPacket : public InboundPacket {  // 0x20
private:
    world::ChunkColumnPtr m_ChunkColumn;
    std::vector<block::BlockEntityPtr> m_BlockEntities;

public:
    MCLIB_API ChunkDataPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    world::ChunkColumnPtr GetChunkColumn() const { return m_ChunkColumn; }
    const std::vector<block::BlockEntityPtr>& GetBlockEntities() const {
        return m_BlockEntities;
    }
};

class EffectPacket : public InboundPacket {  // 0x21
private:
    s32 m_EffectId;
    Vector3i m_Position;
    s32 m_Data;
    bool m_DisableRelativeVolume;

public:
    MCLIB_API EffectPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetEffectId() const { return m_EffectId; }
    Vector3i GetPosition() const { return m_Position; }
    s32 GetData() const { return m_Data; }
    bool GetDisableRelativeVolume() const { return m_DisableRelativeVolume; }
};

class ParticlePacket : public InboundPacket {  // 0x22
private:
    s32 m_ParticleId;
    bool m_LongDistance;
    Vector3d m_Position;
    Vector3d m_MaxOffset;
    float m_ParticleData;
    s32 m_Count;
    std::vector<s32> m_Data;

public:
    MCLIB_API ParticlePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetParticleId() const { return m_ParticleId; }
    bool IsLongDistance() const { return m_LongDistance; }
    Vector3d GetPosition() const { return m_Position; }
    Vector3d GetMaxOffset() const { return m_MaxOffset; }
    float GetParticleData() const { return m_ParticleData; }
    s32 GetCount() const { return m_Count; }
    const std::vector<s32>& GetData() const { return m_Data; }
};

class JoinGamePacket : public InboundPacket {  // 0x23
private:
    s32 m_EntityId;
    u8 m_Gamemode;
    s32 m_Dimension;
    u8 m_Difficulty;
    u8 m_MaxPlayers;
    MCString m_LevelType;
    bool m_ReducedDebug;

public:
    MCLIB_API JoinGamePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetEntityId() const { return m_EntityId; }
    u8 GetGamemode() const { return m_Gamemode; }
    s32 GetDimension() const { return m_Dimension; }
    u8 GetDifficulty() const { return m_Difficulty; }
    u8 GetMaxPlayers() const { return m_MaxPlayers; }
    std::wstring GetLevelType() const { return m_LevelType.GetUTF16(); }
    bool GetReducedDebug() const { return m_ReducedDebug; }
};

class MapPacket : public InboundPacket {  // 0x24
public:
    struct Icon {
        u8 direction;
        u8 type;
        u8 x;
        u8 z;
    };

private:
    s32 m_MapId;
    u8 m_Scale;
    bool m_TrackPosition;
    std::vector<Icon> m_Icons;
    u8 m_Columns;

    u8 m_Rows;
    u8 m_X;
    u8 m_Z;
    s32 m_Length;
    std::string m_Data;

public:
    MCLIB_API MapPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetMapId() const { return m_MapId; }
    u8 GetScale() const { return m_Scale; }
    bool IsTrackingPosition() const { return m_TrackPosition; }
    const std::vector<Icon>& GetIcons() const { return m_Icons; }
    u8 GetColumns() const { return m_Columns; }
    u8 GetRows() const { return m_Rows; }
    u8 GetX() const { return m_X; }
    u8 GetZ() const { return m_Z; }
    s32 GetLength() const { return m_Length; }
    const std::string& GetData() const { return m_Data; }
};

// This packet allows at most 8 blocks movement in any direction,
// because short range is from -32768 to 32767. And 32768 / (128 * 32) = 8.
class EntityRelativeMovePacket : public InboundPacket {  // 0x25
private:
    EntityId m_EntityId;
    Vector3s m_Delta;
    bool m_OnGround;

public:
    MCLIB_API EntityRelativeMovePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    // Change in position as (current * 32 - prev * 32) * 128
    Vector3s GetDelta() const { return m_Delta; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityLookAndRelativeMovePacket : public InboundPacket {  // 0x26
private:
    EntityId m_EntityId;
    Vector3s m_Delta;
    u8 m_Yaw;
    u8 m_Pitch;
    bool m_OnGround;

public:
    MCLIB_API EntityLookAndRelativeMovePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Vector3s GetDelta() const { return m_Delta; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityLookPacket : public InboundPacket {  // 0x27
private:
    EntityId m_EntityId;
    u8 m_Yaw;
    u8 m_Pitch;
    bool m_OnGround;

public:
    MCLIB_API EntityLookPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityPacket : public InboundPacket {  // 0x28
private:
    EntityId m_EntityId;

public:
    MCLIB_API EntityPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
};

class VehicleMovePacket : public InboundPacket {  // 0x29
private:
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;

public:
    MCLIB_API VehicleMovePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Vector3d GetPosition() const { return m_Position; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }
};

class OpenSignEditorPacket : public InboundPacket {  // 0x2A
private:
    Vector3i m_Position;

public:
    MCLIB_API OpenSignEditorPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Vector3i GetPosition() const { return m_Position; }
};

class PlayerAbilitiesPacket : public InboundPacket {  // 0x2B
public:
    enum class Flag { Invulnerable, Flying, AllowFlying, Creative };

private:
    u8 m_Flags;
    float m_FlyingSpeed;
    float m_FOVModifier;

public:
    MCLIB_API PlayerAbilitiesPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    u8 GetFlags() const { return m_Flags; }
    float GetFlyingSpeed() const { return m_FlyingSpeed; }
    float GetFOVModifier() const { return m_FOVModifier; }

    bool HasFlag(Flag flag) const { return (m_Flags & (int)flag) != 0; }
};

class CombatEventPacket : public InboundPacket {  // 0x2C
public:
    enum class Event { EnterCombat, EndCombat, EntityDead };

private:
    Event m_Event;
    s32 m_Duration;          // EndCombat only
    EntityId m_PlayerId;     // EntityDead only
    EntityId m_EntityId;     // EndCombat and EntityDead only
    std::wstring m_Message;  // EntityDead only

public:
    MCLIB_API CombatEventPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Event GetEvent() const { return m_Event; }
    s32 GetDuration() const { return m_Duration; }
    EntityId GetPlayerId() const { return m_PlayerId; }
    EntityId GetEntityId() const { return m_EntityId; }
    const std::wstring& GetMessage() const { return m_Message; }
};

class PlayerListItemPacket : public InboundPacket {  // 0x2D
public:
    enum class Action {
        AddPlayer = 0,
        UpdateGamemode,
        UpdateLatency,
        UpdateDisplay,
        RemovePlayer
    };

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
    MCLIB_API PlayerListItemPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Action GetAction() const { return m_Action; }

    const std::vector<ActionDataPtr>& GetActionData() const { return m_Data; }
};

class PlayerPositionAndLookPacket : public InboundPacket {  // 0x2E
private:
    Vector3d m_Position;
    float m_Yaw, m_Pitch;
    u8 m_Flags;
    s32 m_TeleportId;

public:
    MCLIB_API PlayerPositionAndLookPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Vector3d GetPosition() const { return m_Position; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }
    u8 GetFlags() const { return m_Flags; }
    s32 GetTeleportId() const { return m_TeleportId; }
};

class UseBedPacket : public InboundPacket {  // 0x2F
private:
    EntityId m_EntityId;
    Vector3i m_Position;

public:
    MCLIB_API UseBedPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Vector3i GetPosition() const { return m_Position; }
};

class DestroyEntitiesPacket : public InboundPacket {  // 0x30
private:
    std::vector<EntityId> m_EntityIds;

public:
    MCLIB_API DestroyEntitiesPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::vector<EntityId>& GetEntityIds() const { return m_EntityIds; }
};

class UnlockRecipesPacket : public InboundPacket {
public:
    enum class Action { Initialize, Add, Remove };

private:
    Action m_Action;
    bool m_OpenCraftingBook;
    bool m_Filter;

    std::vector<s32> m_Array1;
    std::vector<s32> m_Array2;

public:
    MCLIB_API UnlockRecipesPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);
};

class RemoveEntityEffectPacket : public InboundPacket {  // 0x31
private:
    EntityId m_EntityId;
    u8 m_EffectId;

public:
    MCLIB_API RemoveEntityEffectPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetEffectId() const { return m_EffectId; }
};

class ResourcePackSendPacket : public InboundPacket {  // 0x32
private:
    std::wstring m_Url;
    std::string m_Hash;

public:
    MCLIB_API ResourcePackSendPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::wstring& GetURL() const { return m_Url; }
    const std::string& GetHash() const { return m_Hash; }
};

class RespawnPacket : public InboundPacket {  // 0x33
private:
    s32 m_Dimension;
    u8 m_Difficulty;
    u8 m_Gamemode;
    std::wstring m_Level;

public:
    MCLIB_API RespawnPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetDimension() const { return m_Dimension; }
    u8 GetDifficulty() const { return m_Difficulty; }
    u8 GetGamemode() const { return m_Gamemode; }
    const std::wstring& GetLevel() const { return m_Level; }
};

class EntityHeadLookPacket : public InboundPacket {  // 0x34
private:
    EntityId m_EntityId;
    u8 m_Yaw;

public:
    MCLIB_API EntityHeadLookPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetYaw() const { return m_Yaw; }
};

class WorldBorderPacket : public InboundPacket {  // 0x35
public:
    enum class Action {
        SetSize,
        LerpSize,
        SetCenter,
        Initialize,
        SetWarningTime,
        SetWarningBlocks
    };

private:
    double m_Diameter;
    double m_OldDiameter;
    double m_X;
    double m_Z;
    s64 m_Speed;
    s32 m_PortalTeleportBoundary;
    s32 m_WarningTime;
    s32 m_WarningBlocks;

    Action m_Action;

public:
    MCLIB_API WorldBorderPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    double GetDiameter() const { return m_Diameter; };
    double GetOldDiameter() const { return m_OldDiameter; };

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
     * Causes the screen to be tinted red when a contracting world border will
     * reach the player within the specified time. The default is 15 seconds.
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

class CameraPacket : public InboundPacket {  // 0x36
private:
    EntityId m_EntityId;

public:
    MCLIB_API CameraPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
};

class HeldItemChangePacket : public InboundPacket {  // 0x37
private:
    u8 m_Slot;

public:
    MCLIB_API HeldItemChangePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    // The new slot that the player selected (0-8)
    u8 GetSlot() const { return m_Slot; }
};

class DisplayScoreboardPacket : public InboundPacket {  // 0x38
public:
    enum class ScoreboardPosition { List, Sidebar, BelowName };

private:
    ScoreboardPosition m_Position;
    std::wstring m_Name;

public:
    MCLIB_API DisplayScoreboardPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    ScoreboardPosition GetPosition() const { return m_Position; }
    const std::wstring& GetName() const { return m_Name; }
};

class EntityMetadataPacket : public InboundPacket {  // 0x39
private:
    EntityId m_EntityId;
    entity::EntityMetadata m_Metadata;

public:
    MCLIB_API EntityMetadataPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    const entity::EntityMetadata& GetMetadata() const { return m_Metadata; }
};

class AttachEntityPacket : public InboundPacket {  // 0x3A
private:
    EntityId m_EntityId;
    EntityId m_VehicleId;

public:
    MCLIB_API AttachEntityPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    EntityId GetVehicleId() const { return m_VehicleId; }
};

class EntityVelocityPacket : public InboundPacket {  // 0x3B
private:
    EntityId m_EntityId;
    Vector3s m_Velocity;

public:
    MCLIB_API EntityVelocityPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }

    // Units of 1/8000 of a block per tick
    Vector3s GetVelocity() const { return m_Velocity; }
};

class EntityEquipmentPacket : public InboundPacket {  // 0x3C
public:
    enum class EquipmentSlot {
        MainHand,
        OffHand,
        Boots,
        Leggings,
        Chestplate,
        Helmet
    };

private:
    EntityId m_EntityId;
    EquipmentSlot m_EquipmentSlot;
    inventory::Slot m_Item;

public:
    MCLIB_API EntityEquipmentPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    EquipmentSlot GetEquipmentSlot() const { return m_EquipmentSlot; }
    inventory::Slot GetItem() const { return m_Item; }
};

class SetExperiencePacket : public InboundPacket {  // 0x3D
private:
    float m_ExperienceBar;
    s32 m_Level;
    s32 m_TotalExperience;

public:
    MCLIB_API SetExperiencePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    float GetExperienceBar() const { return m_ExperienceBar; }
    s32 GetLevel() const { return m_Level; }
    s32 GetTotalExperience() const { return m_TotalExperience; }
};

class UpdateHealthPacket : public InboundPacket {  // 0x3E
private:
    float m_Health;
    s32 m_Food;
    float m_Saturation;

public:
    MCLIB_API UpdateHealthPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    float GetHealth() const { return m_Health; }
    s32 GetFood() const { return m_Food; }
    float GetSaturation() const { return m_Saturation; }
};

class ScoreboardObjectivePacket : public InboundPacket {  // 0x3F
public:
    enum class Mode { Create, Remove, Update };

private:
    std::wstring m_Objective;
    Mode m_Mode;
    std::wstring m_Value;
    std::wstring m_Type;

public:
    MCLIB_API ScoreboardObjectivePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::wstring& GetObjective() const { return m_Objective; }
    Mode GetMode() const { return m_Mode; }
    const std::wstring& GetValue() const { return m_Value; }
    const std::wstring& GetType() const { return m_Type; }
};

class SetPassengersPacket : public InboundPacket {  // 0x40
private:
    EntityId m_EntityId;
    std::vector<EntityId> m_Passengers;

public:
    MCLIB_API SetPassengersPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    const std::vector<EntityId>& GetPassengers() const { return m_Passengers; }
};

class TeamsPacket : public InboundPacket {  // 0x41
public:
    enum class Mode { Create, Remove, Update, AddPlayer, RemovePlayer };

private:
    std::wstring m_TeamName;
    Mode m_Mode;
    std::wstring m_TeamDisplayName;
    std::wstring m_TeamPrefix;
    std::wstring m_TeamSuffix;
    u8 m_FriendlyFlags;
    std::wstring m_TagVisbility;
    std::wstring m_CollisionRule;
    u8 m_Color;
    std::vector<std::wstring> m_Players;

public:
    MCLIB_API TeamsPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::wstring& GetTeamName() const { return m_TeamName; }
    Mode GetMode() const { return m_Mode; }
    const std::wstring& GetTeamDisplayName() const { return m_TeamDisplayName; }
    const std::wstring& GetTeamPrefix() const { return m_TeamPrefix; }
    const std::wstring& GetTeamSuffix() const { return m_TeamSuffix; }
    u8 GetFriendlyFlags() const { return m_FriendlyFlags; }
    const std::wstring& GetTagVisibility() const { return m_TagVisbility; }
    const std::wstring& GetCollisionRule() const { return m_CollisionRule; }
    u8 GetColor() const { return m_Color; }
    const std::vector<std::wstring>& GetPlayers() const { return m_Players; }
};

class UpdateScorePacket : public InboundPacket {  // 0x42
public:
    enum class Action { CreateUpdate, Remove };

private:
    std::wstring m_ScoreName;
    Action m_Action;
    std::wstring m_Objective;
    s32 m_Value;

public:
    MCLIB_API UpdateScorePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::wstring& GetScoreName() const { return m_ScoreName; }
    Action GetAction() const { return m_Action; }
    const std::wstring& GetObjective() const { return m_Objective; }
    s32 GetValue() const { return m_Value; }
};

class SpawnPositionPacket : public InboundPacket {  // 0x43
private:
    Position m_Location;

public:
    MCLIB_API SpawnPositionPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Position GetLocation() const { return m_Location; }
};

class TimeUpdatePacket : public InboundPacket {  // 0x44
private:
    s64 m_WorldAge;
    s64 m_Time;

public:
    MCLIB_API TimeUpdatePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s64 GetWorldAge() const { return m_WorldAge; }
    s64 GetTime() const { return m_Time; }
};

class TitlePacket : public InboundPacket {  // 0x45
public:
    enum class Action {
        SetTitle,
        SetSubtitle,
        SetActionBar,
        SetDisplay,
        Hide,
        Reset
    };

private:
    Action m_Action;
    std::wstring m_Text;
    s32 m_FadeIn;
    s32 m_Stay;
    s32 m_FadeOut;

public:
    MCLIB_API TitlePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    Action GetAction() const { return m_Action; }
    const std::wstring& GetText() const { return m_Text; }
    s32 GetFadeIn() const { return m_FadeIn; }
    s32 GetStay() const { return m_Stay; }
    s32 GetFadeOut() const { return m_FadeOut; }
};

class SoundEffectPacket : public InboundPacket {  // 0x46
private:
    s32 m_SoundId;
    SoundCategory m_Category;
    Vector3d m_Position;
    float m_Volume;
    float m_Pitch;

public:
    MCLIB_API SoundEffectPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s32 GetSoundId() const { return m_SoundId; }
    SoundCategory GetCategory() const { return m_Category; }
    Vector3d GetPosition() const { return m_Position; }
    float GetVolume() const { return m_Volume; }
    float GetPitch() const { return m_Pitch; }
};

class PlayerListHeaderAndFooterPacket : public InboundPacket {  // 0x47
private:
    std::wstring m_Header;
    std::wstring m_Footer;

public:
    MCLIB_API PlayerListHeaderAndFooterPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::wstring& GetHeader() const { return m_Header; }
    const std::wstring& GetFooter() const { return m_Footer; }
};

class CollectItemPacket : public InboundPacket {  // 0x48
private:
    EntityId m_Collector;
    EntityId m_Collected;
    s32 m_PickupCount;

public:
    MCLIB_API CollectItemPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetCollectorId() const { return m_Collector; }
    EntityId GetCollectedId() const { return m_Collected; }
    s32 GetPickupCount() const { return m_PickupCount; }
};

class EntityTeleportPacket : public InboundPacket {  // 0x49
private:
    EntityId m_EntityId;
    Vector3d m_Position;
    u8 m_Yaw;
    u8 m_Pitch;
    bool m_OnGround;

public:
    MCLIB_API EntityTeleportPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    Vector3d GetPosition() const { return m_Position; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    float IsOnGround() const { return m_OnGround; }
};

class EntityPropertiesPacket : public InboundPacket {  // 0x4A
private:
    EntityId m_EntityId;
    std::map<std::wstring, mc::entity::Attribute> m_Properties;

public:
    MCLIB_API EntityPropertiesPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    const std::map<std::wstring, mc::entity::Attribute>& GetProperties() const {
        return m_Properties;
    }
};

class EntityEffectPacket : public InboundPacket {  // 0x4B
public:
    enum class Flag { Ambient, ShowParticles };

private:
    EntityId m_EntityId;
    u8 m_EffectId;
    u8 m_Amplifier;
    s32 m_Duration;
    u8 m_Flags;

public:
    MCLIB_API EntityEffectPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    EntityId GetEntityId() const { return m_EntityId; }
    u8 GetEffectId() const { return m_EffectId; }
    u8 GetAmplifier() const { return m_Amplifier; }
    s32 GetDuration() const { return m_Duration; }
    u8 GetFlags() const { return m_Flags; }

    bool HasFlag(Flag flag) const { return (m_Flags & (int)flag) != 0; }
};

class AdvancementProgressPacket : public InboundPacket {
private:
    std::wstring m_Id;

public:
    MCLIB_API AdvancementProgressPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);
};

class CraftRecipeResponsePacket : public InboundPacket {
private:
    u8 m_WindowId;
    s32 m_RecipeId;

public:
    MCLIB_API CraftRecipeResponsePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);
};

namespace status {

class ResponsePacket : public InboundPacket {  // 0x00
private:
    std::wstring m_Response;

public:
    MCLIB_API ResponsePacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    const std::wstring& GetResponse() const { return m_Response; }
};

class PongPacket : public InboundPacket {  // 0x01
private:
    s64 m_Payload;

public:
    MCLIB_API PongPacket();
    bool MCLIB_API Deserialize(DataBuffer& data, std::size_t packetLength);
    void MCLIB_API Dispatch(PacketHandler* handler);

    s64 GetPayload() const { return m_Payload; }
};

}  // namespace status
}  // namespace in

namespace out {

// Handshake packets

class HandshakePacket : public OutboundPacket {  // 0x00
private:
    VarInt m_ProtocolVersion;
    MCString m_Server;
    u16 m_Port;
    VarInt m_NewState;

public:
    MCLIB_API HandshakePacket(s32 protocol, std::string server, u16 port,
                              State state);
    DataBuffer MCLIB_API Serialize() const;
};

// Login packets

class LoginStartPacket : public OutboundPacket {  // 0x00
private:
    MCString m_Name;

public:
    MCLIB_API LoginStartPacket(const std::string& name);
    DataBuffer MCLIB_API Serialize() const;
};

class EncryptionResponsePacket : public OutboundPacket {  // 0x01
private:
    std::string m_SharedSecret;
    std::string m_VerifyToken;

public:
    MCLIB_API EncryptionResponsePacket(const std::string& sharedSecret,
                                       const std::string& verifyToken);
    DataBuffer MCLIB_API Serialize() const;

    std::string GetSharedSecret() const { return m_SharedSecret; }
    std::string GetVerifyToken() const { return m_VerifyToken; }
};

// Play packets
class TeleportConfirmPacket : public OutboundPacket {  // 0x00
private:
    s32 m_TeleportId;

public:
    MCLIB_API TeleportConfirmPacket(s32 teleportId);
    DataBuffer MCLIB_API Serialize() const;
};

class PrepareCraftingGridPacket : public OutboundPacket {
public:
    struct Entry {
        inventory::Slot item;
        u8 craftingSlot;
        u8 playerSlot;
    };

private:
    u8 m_WindowId;
    s16 m_ActionNumber;
    std::vector<Entry> m_ReturnEntries;
    std::vector<Entry> m_PrepareEntries;

public:
    MCLIB_API PrepareCraftingGridPacket(
        u8 windowId, s16 actionNumber, const std::vector<Entry>& returnEntries,
        const std::vector<Entry>& prepareEntries);
    DataBuffer MCLIB_API Serialize() const;
};

class CraftRecipeRequestPacket : public OutboundPacket {
private:
    u8 m_WindowId;
    s32 m_RecipeId;
    bool m_MakeAll;

public:
    MCLIB_API CraftRecipeRequestPacket(u8 windowId, s32 recipeId, bool makeAll);
    DataBuffer MCLIB_API Serialize() const;
};

class TabCompletePacket : public OutboundPacket {  // 0x01
private:
    std::wstring m_Text;
    bool m_AssumeCommand;
    bool m_HasPosition;
    Position m_LookingAt;

public:
    MCLIB_API TabCompletePacket(const std::wstring& text, bool assumeCommand);
    MCLIB_API TabCompletePacket(const std::wstring& text, bool assumeCommand,
                                bool hasPosition, Position lookingAt);
    DataBuffer MCLIB_API Serialize() const;
};

class ChatPacket : public OutboundPacket {  // 0x02
private:
    std::wstring m_Message;

public:
    MCLIB_API ChatPacket(const std::wstring& message);
    MCLIB_API ChatPacket(const std::string& message);
    DataBuffer MCLIB_API Serialize() const;

    const std::wstring& GetChatMessage() const { return m_Message; }
};

class ClientStatusPacket : public OutboundPacket {  // 0x03
public:
    enum Action { PerformRespawn, RequestStats, TakingInventoryAchievement };

private:
    Action m_Action;

public:
    MCLIB_API ClientStatusPacket(Action action);
    DataBuffer MCLIB_API Serialize() const;

    Action GetAction() const { return m_Action; }
};

class ClientSettingsPacket : public OutboundPacket {  // 0x04
private:
    std::wstring m_Locale;
    u8 m_ViewDistance;
    ChatMode m_ChatMode;
    bool m_ChatColors;
    u8 m_SkinFlags;
    MainHand m_MainHand;

public:
    MCLIB_API ClientSettingsPacket(const std::wstring& locale, u8 viewDistance,
                                   ChatMode chatMode, bool chatColors,
                                   u8 skinFlags, MainHand hand);
    DataBuffer MCLIB_API Serialize() const;
};

class ConfirmTransactionPacket : public OutboundPacket {  // 0x05
private:
    u8 m_WindowId;
    s16 m_Action;
    bool m_Accepted;

public:
    MCLIB_API ConfirmTransactionPacket(u8 windowId, s16 action, bool accepted);
    DataBuffer MCLIB_API Serialize() const;
};

class EnchantItemPacket : public OutboundPacket {  // 0x06
private:
    u8 m_WindowId;
    u8 m_EnchantmentIndex;

public:
    MCLIB_API EnchantItemPacket(u8 windowId, u8 enchantmentIndex);
    DataBuffer MCLIB_API Serialize() const;
};

class ClickWindowPacket : public OutboundPacket {  // 0x07
private:
    u8 m_WindowId;
    u16 m_SlotIndex;
    u8 m_Button;
    u16 m_Action;
    s32 m_Mode;
    inventory::Slot m_ClickedItem;

public:
    MCLIB_API ClickWindowPacket(u8 windowId, u16 slotIndex, u8 button,
                                u16 action, s32 mode,
                                inventory::Slot clickedItem);
    DataBuffer MCLIB_API Serialize() const;
};

class CloseWindowPacket : public OutboundPacket {  // 0x08
private:
    u8 m_WindowId;

public:
    MCLIB_API CloseWindowPacket(u8 windowId);
    DataBuffer MCLIB_API Serialize() const;
};

class PluginMessagePacket : public OutboundPacket {  // 0x09
private:
    std::wstring m_Channel;
    std::string m_Data;

public:
    MCLIB_API PluginMessagePacket(const std::wstring& channel,
                                  const std::string& data);
    DataBuffer MCLIB_API Serialize() const;
};

class UseEntityPacket : public OutboundPacket {  // 0x0A
public:
    enum class Action { Interact, Attack, InteractAt };

private:
    EntityId m_Target;
    Action m_Action;
    Vector3f m_Position;
    Hand m_Hand;

public:
    MCLIB_API UseEntityPacket(EntityId target, Action action,
                              Hand hand = Hand::Main,
                              Vector3f position = Vector3f(0, 0, 0));
    DataBuffer MCLIB_API Serialize() const;
};

class KeepAlivePacket : public OutboundPacket {  // 0x0B
private:
    s64 m_KeepAliveId;

public:
    MCLIB_API KeepAlivePacket(s64 id);
    DataBuffer MCLIB_API Serialize() const;

    s64 GetKeepAliveId() const { return m_KeepAliveId; }
};

class PlayerPositionPacket : public OutboundPacket {  // 0x0C
private:
    Vector3d m_Position;
    bool m_OnGround;

public:
    MCLIB_API PlayerPositionPacket(Vector3d position, bool onGround);
    DataBuffer MCLIB_API Serialize() const;
};

class PlayerPositionAndLookPacket : public OutboundPacket {  // 0x0D
private:
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;
    bool m_OnGround;

public:
    MCLIB_API PlayerPositionAndLookPacket(Vector3d position, float yaw,
                                          float pitch, bool onGround);
    DataBuffer MCLIB_API Serialize() const;
};

class PlayerLookPacket : public OutboundPacket {  // 0x0E
private:
    float m_Yaw;
    float m_Pitch;
    bool m_OnGround;

public:
    MCLIB_API PlayerLookPacket(float yaw, float pitch, bool onGround);
    DataBuffer MCLIB_API Serialize() const;
};

class PlayerPacket : public OutboundPacket {  // 0x0F
private:
    bool m_OnGround;

public:
    MCLIB_API PlayerPacket(bool onGround);
    DataBuffer MCLIB_API Serialize() const;
};

class VehicleMovePacket : public OutboundPacket {  // 0x10
private:
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;

public:
    MCLIB_API VehicleMovePacket(Vector3d position, float yaw, float pitch);
    DataBuffer MCLIB_API Serialize() const;
};

class SteerBoatPacket : public OutboundPacket {  // 0x11
private:
    bool m_RightPaddle;
    bool m_LeftPaddle;

public:
    MCLIB_API SteerBoatPacket(bool rightPaddle, bool leftPaddle);
    DataBuffer MCLIB_API Serialize() const;
};

class PlayerAbilitiesPacket : public OutboundPacket {  // 0x12
private:
    bool m_IsFlying;

public:
    MCLIB_API PlayerAbilitiesPacket(bool isFlying);
    DataBuffer MCLIB_API Serialize() const;
};

class PlayerDiggingPacket : public OutboundPacket {  // 0x13
public:
    enum Status {
        StartedDigging,
        CancelledDigging,
        FinishedDigging,
        DropItemStack,
        DropItem,
        ShootArrow,
        SwapHandItem
    };

private:
    Status m_Status;
    Vector3i m_Position;
    Face m_Face;

public:
    MCLIB_API PlayerDiggingPacket(Status status, Vector3i position, Face face);
    DataBuffer MCLIB_API Serialize() const;
};

class EntityActionPacket : public OutboundPacket {  // 0x14
public:
    enum class Action {
        StartSneak,
        StopSneak,
        LeaveBed,
        StartSprint,
        StopSprint,
        StartHorseJump,
        StopHorseJump,
        OpenRiddenHorseInventory,
        StartElytraFlying
    };

private:
    EntityId m_EntityId;
    Action m_Action;
    s32 m_ActionData;

public:
    // Action data is only used for HorseJump (0 to 100), 0 otherwise.
    MCLIB_API EntityActionPacket(EntityId eid, Action action,
                                 s32 actionData = 0);
    DataBuffer MCLIB_API Serialize() const;
};

class SteerVehiclePacket : public OutboundPacket {  // 0x15
private:
    float m_Sideways;
    float m_Forward;
    u8 m_Flags;

public:
    // Flags: 0x01 = Jump, 0x02 = Unmount
    MCLIB_API SteerVehiclePacket(float sideways, float forward, u8 flags);
    DataBuffer MCLIB_API Serialize() const;
};

class ResourcePackStatusPacket : public OutboundPacket {  // 0x16
public:
    enum class Result { Loaded, Declined, Failed, Accepted };

private:
    Result m_Result;

public:
    MCLIB_API ResourcePackStatusPacket(Result result);
    DataBuffer MCLIB_API Serialize() const;
};

class CraftingBookDataPacket : public OutboundPacket {
private:
public:
    MCLIB_API CraftingBookDataPacket();
    DataBuffer MCLIB_API Serialize() const;
};

class HeldItemChangePacket : public OutboundPacket {  // 0x17
private:
    u16 m_Slot;

public:
    // Slot should be between 0 and 8, representing hot bar left to right
    MCLIB_API HeldItemChangePacket(u16 slot);
    DataBuffer MCLIB_API Serialize() const;
};

class CreativeInventoryActionPacket : public OutboundPacket {  // 0x18
private:
    s16 m_Slot;
    inventory::Slot m_Item;

public:
    MCLIB_API CreativeInventoryActionPacket(s16 slot, inventory::Slot item);
    DataBuffer MCLIB_API Serialize() const;
};

class UpdateSignPacket : public OutboundPacket {  // 0x19
private:
    Position m_Position;
    std::wstring m_Line1;
    std::wstring m_Line2;
    std::wstring m_Line3;
    std::wstring m_Line4;

public:
    MCLIB_API UpdateSignPacket(Vector3d position, const std::wstring& line1,
                               const std::wstring& line2,
                               const std::wstring& line3,
                               const std::wstring& line4);
    DataBuffer MCLIB_API Serialize() const;
};

// Send when the player's arm swings
class AnimationPacket : public OutboundPacket {  // 0x1A
private:
    Hand m_Hand;

public:
    MCLIB_API AnimationPacket(Hand hand = Hand::Main);
    DataBuffer MCLIB_API Serialize() const;
};

class SpectatePacket : public OutboundPacket {  // 0x1B
private:
    UUID m_UUID;

public:
    MCLIB_API SpectatePacket(UUID uuid);
    DataBuffer MCLIB_API Serialize() const;
};

class PlayerBlockPlacementPacket : public OutboundPacket {  // 0x1C
private:
    Vector3i m_Position;
    Face m_Face;
    Hand m_Hand;
    Vector3f m_CursorPos;

public:
    // Cursor position is the position of the crosshair on the block
    MCLIB_API PlayerBlockPlacementPacket(Vector3i position, Face face,
                                         Hand hand, Vector3f cursorPos);
    DataBuffer MCLIB_API Serialize() const;
};

class UseItemPacket : public OutboundPacket {  // 0x1D
private:
    Hand m_Hand;

public:
    MCLIB_API UseItemPacket(Hand hand);
    DataBuffer MCLIB_API Serialize() const;
};

class AdvancementTabPacket : public OutboundPacket {
private:
public:
    MCLIB_API AdvancementTabPacket();
    DataBuffer MCLIB_API Serialize() const;
};

namespace status {

class RequestPacket : public OutboundPacket {  // 0x00
public:
    MCLIB_API RequestPacket();
    DataBuffer MCLIB_API Serialize() const;
};

class PingPacket : public OutboundPacket {  // 0x01
private:
    s64 m_Payload;

public:
    MCLIB_API PingPacket(s64 payload);
    DataBuffer MCLIB_API Serialize() const;
};

}  // namespace status
}  // namespace out

}  // namespace packets
}  // namespace protocol
}  // namespace mc

#endif
