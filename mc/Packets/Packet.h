#ifndef PACKETS_PACKET_H_
#define PACKETS_PACKET_H_

#include "../DataBuffer.h"
#include "../MCString.h"
#include "../Position.h"
#include "../Protocol.h"
#include <map>
#include <json/json.h>

namespace Minecraft {
namespace Packets {

class PacketHandler;

class Packet {
private:
   // Packet& operator=(const Packet&);

protected:
    VarInt m_Id;
    Minecraft::ProtocolState m_ProtocolState;

public:
    Packet() : m_Id(0xFF) { }
    virtual ~Packet() { }

    Minecraft::ProtocolState GetProtocolState() const { return m_ProtocolState; }
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
    u8 m_GameMode;
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
    u8 GetGameMode() const { return m_GameMode; }
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

class SpawnPositionPacket : public InboundPacket { // 0x05
private:
    Position m_Location;

public:
    SpawnPositionPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    Position GetLocation() const { return m_Location; }
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

class SpawnMobPacket : public InboundPacket { // 0x0F
private:
    EntityId m_EntityId;
    u8 m_Type;
    s32 m_X;
    s32 m_Y;
    s32 m_Z;
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
    s32 GetX() const { return m_X; }
    s32 GetY() const { return m_Y; }
    s32 GetZ() const { return m_Z; }
    u8 GetYaw() const { return m_Yaw; }
    u8 GetPitch() const { return m_Pitch; }
    u8 GetHeadPitch() const { return m_HeadPitch; }
    s16 GetVelocityX() const { return m_VelocityX; }
    s16 GetVelocityY() const { return m_VelocityY; }
    s16 GetVelocityZ() const { return m_VelocityZ; }
    const EntityMetadata& GetMetadata() const { return m_Metadata; }
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

class MapChunkBulkPacket : public InboundPacket { // 0x26
private:
    bool m_SkyLight;

public:
    MapChunkBulkPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);
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

class ServerDifficultyPacket : public InboundPacket { // 0x41
private:
    u8 m_Difficulty;

public:
    ServerDifficultyPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    u8 GetDifficulty() const { return m_Difficulty; }
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
    HandshakePacket(s32 protocol, std::string server, u16 port, s32 state);
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
