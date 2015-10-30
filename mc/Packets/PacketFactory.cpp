#include "PacketFactory.h"
#include <exception>
#include <string>

namespace Minecraft {
namespace Packets {


class LoginPacketFactory {
public:
    static Packet* CreatePacket(DataBuffer& data, std::size_t length);
};

class PlayPacketFactory {
public:
    static Packet* CreatePacket(DataBuffer& data, std::size_t length);
};

Packet* LoginPacketFactory::CreatePacket(DataBuffer& data, std::size_t length) {
    if (data.GetSize() == 0) return nullptr;
    u8 id;
    data >> id;

    InboundPacket* packet = nullptr;

    switch (id) {
    case Protocol::Login::Disconnect:
        packet = new Inbound::DisconnectPacket();
        break;
    case Protocol::Login::EncryptionRequest:
        packet = new Inbound::EncryptionRequestPacket();
        break;
    case Protocol::Login::LoginSuccess:
        packet = new Inbound::LoginSuccessPacket();
        break;
    case Protocol::Login::SetCompression:
        packet = new Inbound::SetCompressionPacket();
        break;
    default:
        throw std::runtime_error("Unknown packet type " + std::to_string(id) + " received during login protocol state.");
    }

    if (packet)
        packet->Deserialize(data, length);

    return packet;
}

Packet* PlayPacketFactory::CreatePacket(DataBuffer& data, std::size_t length) {
    if (data.GetSize() == 0) return nullptr;
    u8 id;
    data >> id;

    InboundPacket* packet = nullptr;

    switch (id) {
    case Protocol::Play::KeepAlive:
        packet = new Inbound::KeepAlivePacket();
        break;
    case Protocol::Play::JoinGame:
        packet = new Inbound::JoinGamePacket();
        break;
    case Protocol::Play::Chat:
        packet = new Inbound::ChatPacket();
        break;
    case Protocol::Play::TimeUpdate:
        packet = new Inbound::TimeUpdatePacket();
        break;
    case Protocol::Play::EntityEquipment:
        packet = new Inbound::EntityEquipmentPacket();
        break;
    case Protocol::Play::SpawnPosition:
        packet = new Inbound::SpawnPositionPacket();
        break;
    case Protocol::Play::UpdateHealth:
        packet = new Inbound::UpdateHealthPacket();
        break;
    case Protocol::Play::Respawn:
        packet = new Inbound::RespawnPacket();
        break;
    case Protocol::Play::PlayerPositionAndLook:
        packet = new Inbound::PlayerPositionAndLookPacket();
        break;
    case Protocol::Play::HeldItemChange:
        packet = new Inbound::HeldItemChangePacket();
        break;
    case Protocol::Play::Animation:
        packet = new Inbound::AnimationPacket();
        break;
    case Protocol::Play::SpawnPlayer:
        packet = new Inbound::SpawnPlayerPacket();
        break;
    case Protocol::Play::SpawnMob:
        packet = new Inbound::SpawnMobPacket();
        break;
    case Protocol::Play::EntityVelocity:
        packet = new Inbound::EntityVelocityPacket();
        break;
    case Protocol::Play::DestroyEntities:
        packet = new Inbound::DestroyEntitiesPacket();
        break;
    case Protocol::Play::Entity:
        packet = new Inbound::EntityPacket();
        break;
    case Protocol::Play::EntityRelativeMove:
        packet = new Inbound::EntityRelativeMovePacket();
        break;
    case Protocol::Play::EntityLook:
        packet = new Inbound::EntityLookPacket();
        break;
    case Protocol::Play::EntityLookAndRelativeMove:
        packet = new Inbound::EntityLookAndRelativeMovePacket();
        break;
    case Protocol::Play::EntityTeleport:
        packet = new Inbound::EntityTeleportPacket();
        break;
    case Protocol::Play::EntityHeadLook:
        packet = new Inbound::EntityHeadLookPacket();
        break;
    case Protocol::Play::EntityStatus:
        packet = new Inbound::EntityStatusPacket();
        break;
    case Protocol::Play::EntityMetadata:
        packet = new Inbound::EntityMetadataPacket();
        break;
    case Protocol::Play::SetExperience:
        packet = new Inbound::SetExperiencePacket();
        break;
    case Protocol::Play::EntityProperties:
        packet = new Inbound::EntityPropertiesPacket();
        break;
    case Protocol::Play::ChunkData:
        packet = new Inbound::ChunkDataPacket();
        break;
    case Protocol::Play::MultiBlockChange:
        packet = new Inbound::MultiBlockChangePacket();
        break;
    case Protocol::Play::BlockChange:
        packet = new Inbound::BlockChangePacket();
        break;
    case Protocol::Play::MapChunkBulk:
        packet = new Inbound::MapChunkBulkPacket();
        break;
    case Protocol::Play::Effect:
        packet = new Inbound::EffectPacket();
        break;
    case Protocol::Play::SoundEffect:
        packet = new Inbound::SoundEffectPacket();
        break;
    case Protocol::Play::ChangeGameState:
        packet = new Inbound::ChangeGameStatePacket();
        break;
    case Protocol::Play::SetSlot:
        packet = new Inbound::SetSlotPacket();
        break;
    case Protocol::Play::WindowItems:
        packet = new Inbound::WindowItemsPacket();
        break;
    case Protocol::Play::UpdateBlockEntity:
        packet = new Inbound::UpdateBlockEntityPacket();
        break;
    case Protocol::Play::Statistics:
        packet = new Inbound::StatisticsPacket();
        break;
    case Protocol::Play::PlayerListItem:
        packet = new Inbound::PlayerListItemPacket();
        break;
    case Protocol::Play::PlayerAbilities:
        packet = new Inbound::PlayerAbilitiesPacket();
        break;
    case Protocol::Play::PluginMessage:
        packet = new Inbound::PluginMessagePacket();
        break;
    case Protocol::Play::Disconnect:
        packet = new Inbound::DisconnectPacket();
        break;
    case Protocol::Play::ServerDifficulty:
        packet = new Inbound::ServerDifficultyPacket();
        break;
    case Protocol::Play::CombatEvent:
        packet = new Inbound::CombatEventPacket();
        break;
    case Protocol::Play::WorldBorder:
        packet = new Inbound::WorldBorderPacket();
        break;
    default:
        throw std::runtime_error("Unknown packet type " + std::to_string(id) + " received during play protocol state.");
    }

    if (packet)
        packet->Deserialize(data, length);

    return packet;
}

Packet* PacketFactory::CreatePacket(Protocol::State state, DataBuffer& data, std::size_t length) {
    switch (state) {
    case Protocol::State::Handshake:
        throw std::runtime_error("Packet received during handshake (wrong protocol state).");
    case Protocol::State::Play:
        return PlayPacketFactory::CreatePacket(data, length);
    case Protocol::State::Login:
        return LoginPacketFactory::CreatePacket(data, length);
    default:
        throw std::runtime_error("Protocol isn't in a valid state.");
    }
}

void PacketFactory::FreePacket(Packet* packet) {
    delete packet;
}

} // ns Packets
} // ns Minecraft
