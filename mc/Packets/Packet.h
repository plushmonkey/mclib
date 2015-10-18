#ifndef PACKETS_PACKET_H_
#define PACKETS_PACKET_H_

#include "../DataBuffer.h"
#include "../MCString.h"
#include "../Protocol.h"

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
