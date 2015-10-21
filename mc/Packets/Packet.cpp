#include "Packet.h"
#include "PacketHandler.h"

namespace Minecraft {
namespace Packets {

namespace Inbound {

// Play packets
KeepAlivePacket::KeepAlivePacket() {
    m_Id = 0x00;
    m_ProtocolState = Minecraft::ProtocolState::Play;
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
}

bool JoinGamePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EntityId;
    data >> m_GameMode;
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
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

SpawnPositionPacket::SpawnPositionPacket() {
    m_Id = 0x05;
    m_ProtocolState = Minecraft::ProtocolState::Play;
}

bool SpawnPositionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Location;
    return true;
}

void SpawnPositionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerPositionAndLookPacket::PlayerPositionAndLookPacket() {
    m_Id = 0x08;
    m_ProtocolState = Minecraft::ProtocolState::Play;
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
}

bool HeldItemChangePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Slot;
    return true;
}

void HeldItemChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnMobPacket::SpawnMobPacket() {
    m_Id = 0x0F;
    m_ProtocolState = Minecraft::ProtocolState::Play;
}

bool SpawnMobPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt entityId;

    data >> entityId;
    m_EntityId = entityId.GetInt();
    data >> m_Type;
    data >> m_X;
    data >> m_Y;
    data >> m_Z;
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

EntityMetadataPacket::EntityMetadataPacket() {
    m_Id = 0x1C;
    m_ProtocolState = Minecraft::ProtocolState::Play;
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

MapChunkBulkPacket::MapChunkBulkPacket() {
    m_Id = 0x26;
    m_ProtocolState = Minecraft::ProtocolState::Play;
}

bool MapChunkBulkPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_SkyLight;
    VarInt count;
    data >> count;

    struct ChunkMeta {
        s32 x;
        s32 z;
        u16 bitmask;
    };

    // Array of meta
    for (s32 i = 0; i < count.GetInt(); ++i) {
        ChunkMeta meta;
        data >> meta.x;
        data >> meta.z;
        data >> meta.bitmask;

    }

    // Array of chunk
    for (s32 i = 0; i < count.GetInt(); ++i) {
        
    }

    return true;
}

void MapChunkBulkPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


SetSlotPacket::SetSlotPacket() {
    m_Id = 0x2F;
    m_ProtocolState = Minecraft::ProtocolState::Play;
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
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

StatisticsPacket::StatisticsPacket() {
    m_Id = 0x37;
    m_ProtocolState = Minecraft::ProtocolState::Play;
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
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

                m_Data.push_back(actionData);
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
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
    m_ProtocolState = Minecraft::ProtocolState::Play;
}

bool ServerDifficultyPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Difficulty;
    return true;
}

void ServerDifficultyPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

WorldBorderPacket::WorldBorderPacket() {
    m_Id = 0x44;
    m_ProtocolState = Minecraft::ProtocolState::Play;
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
    m_ProtocolState = Minecraft::ProtocolState::Login;
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
    m_ProtocolState = Minecraft::ProtocolState::Login;
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
    m_Id = 0x01;
    m_ProtocolState = Minecraft::ProtocolState::Login;
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
    m_ProtocolState = Minecraft::ProtocolState::Login;
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
HandshakePacket::HandshakePacket(s32 protocol, std::string server, u16 port, s32 state) {
    m_Id = 0x00;

    m_ProtocolVersion = protocol;
    m_Server = server;
    m_Port = port;
    m_NewState = state;
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

EncryptionResponsePacket::EncryptionResponsePacket(const std::string& sharedSecret, const std::string& verifyToken) {
    m_Id = 0x01;
    m_SharedSecret = sharedSecret;
    m_VerifyToken = verifyToken;
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
