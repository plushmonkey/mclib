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
    if (data.GetSize() != 5) return false;

    data >> m_Id;
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

        switch (action.GetInt()) {
            case 0: // Add player
            {
                MCString name;
                VarInt numProperties;

                data >> name;
                data >> numProperties;

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
                }

                VarInt gameMode, ping;
                data >> gameMode;
                data >> ping;

                u8 hasDisplayName;
                MCString displayName;

                data >> hasDisplayName;
                if (hasDisplayName)
                    data >> displayName;
            }
            break;
            case 1: // Update gamemode
            {
                VarInt gameMode;
                data >> gameMode;
            }
            break;
            case 2: // Update latency
            {
                VarInt ping;
                data >> ping;
            } 
            break;
            case 3: // Update display name
            {
                u8 hasDisplayName;
                MCString displayName;

                data >> hasDisplayName;
                if (hasDisplayName)
                    data >> displayName;
            }
            break;
            case 4: // Remove player
            {
                // no fields
            }
            break;
        }
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

} // ns Outbound

} // ns Packets
} // ns Minecraft
