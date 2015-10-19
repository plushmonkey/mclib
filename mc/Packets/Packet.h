#ifndef PACKETS_PACKET_H_
#define PACKETS_PACKET_H_

#include "../DataBuffer.h"
#include "../MCString.h"
#include "../Position.h"
#include "../Protocol.h"
#include <map>

namespace Minecraft {
namespace Packets {

class PacketHandler;

class Packet {
private:
    Packet& operator=(const Packet&);

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

    s64 GetMaxPacketSize() const { return m_MaxPacketSize.GetLong(); }
};

// Play packets

class KeepAlivePacket : public InboundPacket { // 0x00
private:
    VarInt m_AliveId;

public:
    KeepAlivePacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);

    VarInt GetAliveId() const { return m_AliveId; }
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

    u8 GetSlot() const { return m_Slot; }
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

    u8 GetWindowId() const { return m_WindowId; }
    s16 GetSlotIndex() const { return m_SlotIndex; }
    Slot GetSlot() const { return m_Slot; }
};

class WindowItemsPacket : public InboundPacket { // 0x30
private:
    u8 m_WindowId;

public:
    WindowItemsPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);
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
private:
    double m_Radius;

public:
    WorldBorderPacket();
    bool Deserialize(DataBuffer& data, std::size_t packetLength);
    void Dispatch(PacketHandler* handler);
    
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

} // ns Outbound

} // ns Packets
} // ns Minecraft

#endif
