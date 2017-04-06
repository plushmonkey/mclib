#include <mclib/protocol/packets/PacketFactory.h>

#include <exception>
#include <string>

namespace mc {
namespace protocol {
namespace packets {

class LoginPacketFactory {
public:
    static Packet* CreatePacket(DataBuffer& data, std::size_t length);
};

class StatusPacketFactory {
public:
    static Packet* CreatePacket(DataBuffer& data, std::size_t length);
};

class PlayPacketFactory {
public:
    static Packet* CreatePacket(DataBuffer& data, std::size_t length);
};

Packet* LoginPacketFactory::CreatePacket(DataBuffer& data, std::size_t length) {
    if (data.GetSize() == 0) return nullptr;

    VarInt vid;
    data >> vid;

    u32 id = vid.GetInt();

    InboundPacket* packet = nullptr;

    switch (id) {
    case protocol::login::Disconnect:
        packet = new in::DisconnectPacket();
        break;
    case protocol::login::EncryptionRequest:
        packet = new in::EncryptionRequestPacket();
        break;
    case protocol::login::LoginSuccess:
        packet = new in::LoginSuccessPacket();
        break;
    case protocol::login::SetCompression:
        packet = new in::SetCompressionPacket();
        break;
    default:
        throw protocol::UnfinishedProtocolException(vid, protocol::State::Login);
    }

    if (packet)
        packet->Deserialize(data, length);

    return packet;
}


Packet* StatusPacketFactory::CreatePacket(DataBuffer& data, std::size_t length) {
    if (data.GetSize() == 0) return nullptr;

    VarInt vid;
    data >> vid;

    u32 id = vid.GetInt();

    InboundPacket* packet = nullptr;

    switch (id) {
    case protocol::status::Response:
        packet = new in::status::ResponsePacket();
        break;
    case protocol::status::Pong:
        packet = new in::status::PongPacket();
        break;
    default:
        throw protocol::UnfinishedProtocolException(vid, protocol::State::Status);
    }

    if (packet)
        packet->Deserialize(data, length);

    return packet;
}

Packet* PlayPacketFactory::CreatePacket(DataBuffer& data, std::size_t length) {
    if (data.GetSize() == 0) return nullptr;

    VarInt vid;
    data >> vid;

    u32 id = vid.GetInt();

    InboundPacket* packet = nullptr;

    switch (id) {
    case protocol::play::KeepAlive:
        packet = new in::KeepAlivePacket();
        break;
    case protocol::play::JoinGame:
        packet = new in::JoinGamePacket();
        break;
    case protocol::play::Chat:
        packet = new in::ChatPacket();
        break;
    case protocol::play::TimeUpdate:
        packet = new in::TimeUpdatePacket();
        break;
    case protocol::play::EntityEquipment:
        packet = new in::EntityEquipmentPacket();
        break;
    case protocol::play::SpawnPosition:
        packet = new in::SpawnPositionPacket();
        break;
    case protocol::play::UpdateHealth:
        packet = new in::UpdateHealthPacket();
        break;
    case protocol::play::Respawn:
        packet = new in::RespawnPacket();
        break;
    case protocol::play::PlayerPositionAndLook:
        packet = new in::PlayerPositionAndLookPacket();
        break;
    case protocol::play::HeldItemChange:
        packet = new in::HeldItemChangePacket();
        break;
    case protocol::play::UseBed:
        packet = new in::UseBedPacket();
        break;
    case protocol::play::Animation:
        packet = new in::AnimationPacket();
        break;
    case protocol::play::SpawnPlayer:
        packet = new in::SpawnPlayerPacket();
        break;
    case protocol::play::CollectItem:
        packet = new in::CollectItemPacket();
        break;
    case protocol::play::SpawnObject:
        packet = new in::SpawnObjectPacket();
        break;
    case protocol::play::SpawnMob:
        packet = new in::SpawnMobPacket();
        break;
    case protocol::play::SpawnPainting:
        packet = new in::SpawnPaintingPacket();
        break;
    case protocol::play::SpawnExperienceOrb:
        packet = new in::SpawnExperienceOrbPacket();
        break;
    case protocol::play::EntityVelocity:
        packet = new in::EntityVelocityPacket();
        break;
    case protocol::play::DestroyEntities:
        packet = new in::DestroyEntitiesPacket();
        break;
    case protocol::play::Entity:
        packet = new in::EntityPacket();
        break;
    case protocol::play::EntityRelativeMove:
        packet = new in::EntityRelativeMovePacket();
        break;
    case protocol::play::EntityLook:
        packet = new in::EntityLookPacket();
        break;
    case protocol::play::EntityLookAndRelativeMove:
        packet = new in::EntityLookAndRelativeMovePacket();
        break;
    case protocol::play::EntityTeleport:
        packet = new in::EntityTeleportPacket();
        break;
    case protocol::play::EntityHeadLook:
        packet = new in::EntityHeadLookPacket();
        break;
    case protocol::play::EntityStatus:
        packet = new in::EntityStatusPacket();
        break;
    case protocol::play::AttachEntity:
        packet = new in::AttachEntityPacket();
        break;
    case protocol::play::EntityMetadata:
        packet = new in::EntityMetadataPacket();
        break;
    case protocol::play::EntityEffect:
        packet = new in::EntityEffectPacket();
        break;
    case protocol::play::RemoveEntityEffect:
        packet = new in::RemoveEntityEffectPacket();
        break;
    case protocol::play::SetExperience:
        packet = new in::SetExperiencePacket();
        break;
    case protocol::play::EntityProperties:
        packet = new in::EntityPropertiesPacket();
        break;
    case protocol::play::ChunkData:
        packet = new in::ChunkDataPacket();
        break;
    case protocol::play::MultiBlockChange:
        packet = new in::MultiBlockChangePacket();
        break;
    case protocol::play::BlockChange:
        packet = new in::BlockChangePacket();
        break;
    case protocol::play::BlockAction:
        packet = new in::BlockActionPacket();
        break;
    case protocol::play::BlockBreakAnimation:
        packet = new in::BlockBreakAnimationPacket();
        break;
    case protocol::play::Explosion:
        packet = new in::ExplosionPacket();
        break;
    case protocol::play::Effect:
        packet = new in::EffectPacket();
        break;
    case protocol::play::SoundEffect:
        packet = new in::SoundEffectPacket();
        break;
    case protocol::play::Particle:
        packet = new in::ParticlePacket();
        break;
    case protocol::play::ChangeGameState:
        packet = new in::ChangeGameStatePacket();
        break;
    case protocol::play::SpawnGlobalEntity:
        packet = new in::SpawnGlobalEntityPacket();
        break;
    case protocol::play::SetSlot:
        packet = new in::SetSlotPacket();
        break;
    case protocol::play::WindowItems:
        packet = new in::WindowItemsPacket();
        break;
    case protocol::play::UpdateBlockEntity:
        packet = new in::UpdateBlockEntityPacket();
        break;
    case protocol::play::Statistics:
        packet = new in::StatisticsPacket();
        break;
    case protocol::play::PlayerListItem:
        packet = new in::PlayerListItemPacket();
        break;
    case protocol::play::PlayerAbilities:
        packet = new in::PlayerAbilitiesPacket();
        break;
    case protocol::play::PluginMessage:
        packet = new in::PluginMessagePacket();
        break;
    case protocol::play::Disconnect:
        packet = new in::DisconnectPacket();
        break;
    case protocol::play::ServerDifficulty:
        packet = new in::ServerDifficultyPacket();
        break;
    case protocol::play::CombatEvent:
        packet = new in::CombatEventPacket();
        break;
    case protocol::play::WorldBorder:
        packet = new in::WorldBorderPacket();
        break;
    case protocol::play::UnloadChunk:
        packet = new in::UnloadChunkPacket();
        break;
    default:
        throw protocol::UnfinishedProtocolException(vid, protocol::State::Play);
    }

    if (packet)
        packet->Deserialize(data, length);

    return packet;
}

Packet* PacketFactory::CreatePacket(protocol::State state, DataBuffer data, std::size_t length) {
    switch (state) {
    case protocol::State::Handshake:
        throw std::runtime_error("Packet received during handshake (wrong protocol state).");
    case protocol::State::Play:
        return PlayPacketFactory::CreatePacket(data, length);
    case protocol::State::Login:
        return LoginPacketFactory::CreatePacket(data, length);
    case protocol::State::Status:
        return StatusPacketFactory::CreatePacket(data, length);
    default:
        throw std::runtime_error("Protocol isn't in a valid state.");
    }
}

void PacketFactory::FreePacket(Packet* packet) {
    delete packet;
}

} // ns packets
} // ns protocol
} // ns mc
