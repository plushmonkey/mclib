#include <mclib/protocol/Protocol.h>

#include <mclib/protocol/packets/Packet.h>

namespace mc {
namespace protocol {


class Protocol_1_12_0 : public Protocol {
public:
    Protocol_1_12_0(Version version, StateMap inbound)
        : Protocol(version, inbound)
    {

    }

    // Handshake
    virtual s32 GetPacketId(packets::out::HandshakePacket) override { return 0x00; }

    // Login
    virtual s32 GetPacketId(packets::out::LoginStartPacket) override { return 0x00; }
    virtual s32 GetPacketId(packets::out::EncryptionResponsePacket) override { return 0x01; }

    // Status
    virtual s32 GetPacketId(packets::out::status::RequestPacket) override { return 0x00; }
    virtual s32 GetPacketId(packets::out::status::PingPacket) override { return 0x01; }

    // Play
    virtual s32 GetPacketId(packets::out::TeleportConfirmPacket) override { return 0x00; }
    virtual s32 GetPacketId(packets::out::PrepareCraftingGridPacket) override { return 0x01; }
    virtual s32 GetPacketId(packets::out::TabCompletePacket) override { return 0x02; }
    virtual s32 GetPacketId(packets::out::ChatPacket) override { return 0x03; }
    virtual s32 GetPacketId(packets::out::ClientStatusPacket) override { return 0x04; }
    virtual s32 GetPacketId(packets::out::ClientSettingsPacket) override { return 0x05; }
    virtual s32 GetPacketId(packets::out::ConfirmTransactionPacket) override { return 0x06; }
    virtual s32 GetPacketId(packets::out::EnchantItemPacket) override { return 0x07; }
    virtual s32 GetPacketId(packets::out::ClickWindowPacket) override { return 0x08; }
    virtual s32 GetPacketId(packets::out::CloseWindowPacket) override { return 0x09; }
    virtual s32 GetPacketId(packets::out::PluginMessagePacket) override { return 0x0A; }
    virtual s32 GetPacketId(packets::out::UseEntityPacket) override { return 0x0B; }
    virtual s32 GetPacketId(packets::out::KeepAlivePacket) override { return 0x0C; }
    virtual s32 GetPacketId(packets::out::PlayerPositionPacket) override { return 0x0D; }
    virtual s32 GetPacketId(packets::out::PlayerPositionAndLookPacket) override { return 0x0E; }
    virtual s32 GetPacketId(packets::out::PlayerLookPacket) override { return 0x0F; }
    virtual s32 GetPacketId(packets::out::PlayerPacket) override { return 0x10; }
    virtual s32 GetPacketId(packets::out::VehicleMovePacket) override { return 0x11; }
    virtual s32 GetPacketId(packets::out::SteerBoatPacket) override { return 0x12; }
    virtual s32 GetPacketId(packets::out::PlayerAbilitiesPacket) override { return 0x13; }
    virtual s32 GetPacketId(packets::out::PlayerDiggingPacket) override { return 0x14; }
    virtual s32 GetPacketId(packets::out::EntityActionPacket) override { return 0x15; }
    virtual s32 GetPacketId(packets::out::SteerVehiclePacket) override { return 0x16; }
    virtual s32 GetPacketId(packets::out::CraftingBookDataPacket) override { return 0x17; }
    virtual s32 GetPacketId(packets::out::ResourcePackStatusPacket) override { return 0x18; }
    virtual s32 GetPacketId(packets::out::HeldItemChangePacket) override { return 0x19; }
    virtual s32 GetPacketId(packets::out::CreativeInventoryActionPacket) override { return 0x1A; }
    virtual s32 GetPacketId(packets::out::UpdateSignPacket) override { return 0x1B; }
    virtual s32 GetPacketId(packets::out::AnimationPacket) override { return 0x1C; }
    virtual s32 GetPacketId(packets::out::SpectatePacket) override { return 0x1D; }
    virtual s32 GetPacketId(packets::out::PlayerBlockPlacementPacket) override { return 0x1E; }
    virtual s32 GetPacketId(packets::out::UseItemPacket) override { return 0x1F; }
    virtual s32 GetPacketId(packets::out::AdvancementTabPacket) override { return 0x20; }
};


Protocol::StateMap inboundMap_1_11_2 = {
    {
        State::Login,
        {
          { 0x00, []() -> packets::InboundPacket* { return new packets::in::DisconnectPacket(); } },
          { 0x01, []() -> packets::InboundPacket* { return new packets::in::EncryptionRequestPacket(); } },
          { 0x02, []() -> packets::InboundPacket* { return new packets::in::LoginSuccessPacket(); } },
          { 0x03, []() -> packets::InboundPacket* { return new packets::in::SetCompressionPacket(); } },
        }
    },
    {
        State::Status,
        {
          { 0x00, []() -> packets::InboundPacket* { return new packets::in::status::ResponsePacket(); } },
          { 0x01, []() -> packets::InboundPacket* { return new packets::in::status::PongPacket(); } },
        }
    },
    { 
        State::Play, 
        {
          { 0x00, []() -> packets::InboundPacket* { return new packets::in::SpawnObjectPacket(); } },
          { 0x01, []() -> packets::InboundPacket* { return new packets::in::SpawnExperienceOrbPacket(); } },
          { 0x02, []() -> packets::InboundPacket* { return new packets::in::SpawnGlobalEntityPacket(); } },
          { 0x03, []() -> packets::InboundPacket* { return new packets::in::SpawnMobPacket(); } },
          { 0x04, []() -> packets::InboundPacket* { return new packets::in::SpawnPaintingPacket(); } },
          { 0x05, []() -> packets::InboundPacket* { return new packets::in::SpawnPlayerPacket(); } },
          { 0x06, []() -> packets::InboundPacket* { return new packets::in::AnimationPacket(); } },
          { 0x07, []() -> packets::InboundPacket* { return new packets::in::StatisticsPacket(); } },
          { 0x08, []() -> packets::InboundPacket* { return new packets::in::BlockBreakAnimationPacket(); } },
          { 0x09, []() -> packets::InboundPacket* { return new packets::in::UpdateBlockEntityPacket(); } },
          { 0x0A, []() -> packets::InboundPacket* { return new packets::in::BlockActionPacket(); } },
          { 0x0B, []() -> packets::InboundPacket* { return new packets::in::BlockChangePacket(); } },
          { 0x0C, []() -> packets::InboundPacket* { return new packets::in::BossBarPacket(); } },
          { 0x0D, []() -> packets::InboundPacket* { return new packets::in::ServerDifficultyPacket(); } },
          { 0x0E, []() -> packets::InboundPacket* { return new packets::in::TabCompletePacket(); } },
          { 0x0F, []() -> packets::InboundPacket* { return new packets::in::ChatPacket(); } },
          { 0x10, []() -> packets::InboundPacket* { return new packets::in::MultiBlockChangePacket(); } },
          { 0x11, []() -> packets::InboundPacket* { return new packets::in::ConfirmTransactionPacket(); } },
          { 0x12, []() -> packets::InboundPacket* { return new packets::in::CloseWindowPacket(); } },
          { 0x13, []() -> packets::InboundPacket* { return new packets::in::OpenWindowPacket(); } },
          { 0x14, []() -> packets::InboundPacket* { return new packets::in::WindowItemsPacket(); } },
          { 0x15, []() -> packets::InboundPacket* { return new packets::in::WindowPropertyPacket(); } },
          { 0x16, []() -> packets::InboundPacket* { return new packets::in::SetSlotPacket(); } },
          { 0x17, []() -> packets::InboundPacket* { return new packets::in::SetCooldownPacket(); } },
          { 0x18, []() -> packets::InboundPacket* { return new packets::in::PluginMessagePacket(); } },
          { 0x19, []() -> packets::InboundPacket* { return new packets::in::NamedSoundEffectPacket(); } },
          { 0x1A, []() -> packets::InboundPacket* { return new packets::in::DisconnectPacket(); } },
          { 0x1B, []() -> packets::InboundPacket* { return new packets::in::EntityStatusPacket(); } },
          { 0x1C, []() -> packets::InboundPacket* { return new packets::in::ExplosionPacket(); } },
          { 0x1D, []() -> packets::InboundPacket* { return new packets::in::UnloadChunkPacket(); } },
          { 0x1E, []() -> packets::InboundPacket* { return new packets::in::ChangeGameStatePacket(); } },
          { 0x1F, []() -> packets::InboundPacket* { return new packets::in::KeepAlivePacket(); } },
          { 0x20, []() -> packets::InboundPacket* { return new packets::in::ChunkDataPacket(); } },
          { 0x21, []() -> packets::InboundPacket* { return new packets::in::EffectPacket(); } },
          { 0x22, []() -> packets::InboundPacket* { return new packets::in::ParticlePacket(); } },
          { 0x23, []() -> packets::InboundPacket* { return new packets::in::JoinGamePacket(); } },
          { 0x24, []() -> packets::InboundPacket* { return new packets::in::MapPacket(); } },
          { 0x25, []() -> packets::InboundPacket* { return new packets::in::EntityRelativeMovePacket(); } },
          { 0x26, []() -> packets::InboundPacket* { return new packets::in::EntityLookAndRelativeMovePacket(); } },
          { 0x27, []() -> packets::InboundPacket* { return new packets::in::EntityLookPacket(); } },
          { 0x28, []() -> packets::InboundPacket* { return new packets::in::EntityPacket(); } },
          { 0x29, []() -> packets::InboundPacket* { return new packets::in::VehicleMovePacket(); } },
          { 0x2A, []() -> packets::InboundPacket* { return new packets::in::OpenSignEditorPacket(); } },
          { 0x2B, []() -> packets::InboundPacket* { return new packets::in::PlayerAbilitiesPacket(); } },
          { 0x2C, []() -> packets::InboundPacket* { return new packets::in::CombatEventPacket(); } },
          { 0x2D, []() -> packets::InboundPacket* { return new packets::in::PlayerListItemPacket(); } },
          { 0x2E, []() -> packets::InboundPacket* { return new packets::in::PlayerPositionAndLookPacket(); } },
          { 0x2F, []() -> packets::InboundPacket* { return new packets::in::UseBedPacket(); } },
          { 0x30, []() -> packets::InboundPacket* { return new packets::in::DestroyEntitiesPacket(); } },
          { 0x31, []() -> packets::InboundPacket* { return new packets::in::RemoveEntityEffectPacket(); } },
          { 0x32, []() -> packets::InboundPacket* { return new packets::in::ResourcePackSendPacket(); } },
          { 0x33, []() -> packets::InboundPacket* { return new packets::in::RespawnPacket(); } },
          { 0x34, []() -> packets::InboundPacket* { return new packets::in::EntityHeadLookPacket(); } },
          { 0x35, []() -> packets::InboundPacket* { return new packets::in::WorldBorderPacket(); } },
          { 0x36, []() -> packets::InboundPacket* { return new packets::in::CameraPacket(); } },
          { 0x37, []() -> packets::InboundPacket* { return new packets::in::HeldItemChangePacket(); } },
          { 0x38, []() -> packets::InboundPacket* { return new packets::in::DisplayScoreboardPacket(); } },
          { 0x39, []() -> packets::InboundPacket* { return new packets::in::EntityMetadataPacket(); } },
          { 0x3A, []() -> packets::InboundPacket* { return new packets::in::AttachEntityPacket(); } },
          { 0x3B, []() -> packets::InboundPacket* { return new packets::in::EntityVelocityPacket(); } },
          { 0x3C, []() -> packets::InboundPacket* { return new packets::in::EntityEquipmentPacket(); } },
          { 0x3D, []() -> packets::InboundPacket* { return new packets::in::SetExperiencePacket(); } },
          { 0x3E, []() -> packets::InboundPacket* { return new packets::in::UpdateHealthPacket(); } },
          { 0x3F, []() -> packets::InboundPacket* { return new packets::in::ScoreboardObjectivePacket(); } },
          { 0x40, []() -> packets::InboundPacket* { return new packets::in::SetPassengersPacket(); } },
          { 0x41, []() -> packets::InboundPacket* { return new packets::in::TeamsPacket(); } },
          { 0x42, []() -> packets::InboundPacket* { return new packets::in::UpdateScorePacket(); } },
          { 0x43, []() -> packets::InboundPacket* { return new packets::in::SpawnPositionPacket(); } },
          { 0x44, []() -> packets::InboundPacket* { return new packets::in::TimeUpdatePacket(); } },
          { 0x45, []() -> packets::InboundPacket* { return new packets::in::TitlePacket(); } },
          { 0x46, []() -> packets::InboundPacket* { return new packets::in::SoundEffectPacket(); } },
          { 0x47, []() -> packets::InboundPacket* { return new packets::in::PlayerListHeaderAndFooterPacket(); } },
          { 0x48, []() -> packets::InboundPacket* { return new packets::in::CollectItemPacket(); } },
          { 0x49, []() -> packets::InboundPacket* { return new packets::in::EntityTeleportPacket(); } },
          { 0x4A, []() -> packets::InboundPacket* { return new packets::in::EntityPropertiesPacket(); } },
          { 0x4B, []() -> packets::InboundPacket* { return new packets::in::EntityEffectPacket(); } },
        }
    }
};


Protocol::StateMap inboundMap_1_12_0 = {
    {
        State::Login,
        {
            { 0x00, []() -> packets::InboundPacket* { return new packets::in::DisconnectPacket(); } },
            { 0x01, []() -> packets::InboundPacket* { return new packets::in::EncryptionRequestPacket(); } },
            { 0x02, []() -> packets::InboundPacket* { return new packets::in::LoginSuccessPacket(); } },
            { 0x03, []() -> packets::InboundPacket* { return new packets::in::SetCompressionPacket(); } },
        }
    },
    {
        State::Status,
        {
            { 0x00, []() -> packets::InboundPacket* { return new packets::in::status::ResponsePacket(); } },
            { 0x01, []() -> packets::InboundPacket* { return new packets::in::status::PongPacket(); } },
        }
    },
    {
        State::Play,
        {
            { 0x00, []() -> packets::InboundPacket* { return new packets::in::SpawnObjectPacket(); } },
            { 0x01, []() -> packets::InboundPacket* { return new packets::in::SpawnExperienceOrbPacket(); } },
            { 0x02, []() -> packets::InboundPacket* { return new packets::in::SpawnGlobalEntityPacket(); } },
            { 0x03, []() -> packets::InboundPacket* { return new packets::in::SpawnMobPacket(); } },
            { 0x04, []() -> packets::InboundPacket* { return new packets::in::SpawnPaintingPacket(); } },
            { 0x05, []() -> packets::InboundPacket* { return new packets::in::SpawnPlayerPacket(); } },
            { 0x06, []() -> packets::InboundPacket* { return new packets::in::AnimationPacket(); } },
            { 0x07, []() -> packets::InboundPacket* { return new packets::in::StatisticsPacket(); } },
            { 0x08, []() -> packets::InboundPacket* { return new packets::in::AdvancementsPacket(); } },
            { 0x09, []() -> packets::InboundPacket* { return new packets::in::BlockBreakAnimationPacket(); } },
            { 0x0A, []() -> packets::InboundPacket* { return new packets::in::UpdateBlockEntityPacket(); } },
            { 0x0B, []() -> packets::InboundPacket* { return new packets::in::BlockActionPacket(); } },
            { 0x0C, []() -> packets::InboundPacket* { return new packets::in::BlockChangePacket(); } },
            { 0x0D, []() -> packets::InboundPacket* { return new packets::in::BossBarPacket(); } },
            { 0x0E, []() -> packets::InboundPacket* { return new packets::in::ServerDifficultyPacket(); } },
            { 0x0F, []() -> packets::InboundPacket* { return new packets::in::TabCompletePacket(); } },
            { 0x10, []() -> packets::InboundPacket* { return new packets::in::ChatPacket(); } },
            { 0x11, []() -> packets::InboundPacket* { return new packets::in::MultiBlockChangePacket(); } },
            { 0x12, []() -> packets::InboundPacket* { return new packets::in::ConfirmTransactionPacket(); } },
            { 0x13, []() -> packets::InboundPacket* { return new packets::in::CloseWindowPacket(); } },
            { 0x14, []() -> packets::InboundPacket* { return new packets::in::OpenWindowPacket(); } },
            { 0x15, []() -> packets::InboundPacket* { return new packets::in::WindowItemsPacket(); } },
            { 0x16, []() -> packets::InboundPacket* { return new packets::in::WindowPropertyPacket(); } },
            { 0x17, []() -> packets::InboundPacket* { return new packets::in::SetSlotPacket(); } },
            { 0x18, []() -> packets::InboundPacket* { return new packets::in::SetCooldownPacket(); } },
            { 0x19, []() -> packets::InboundPacket* { return new packets::in::PluginMessagePacket(); } },
            { 0x1A, []() -> packets::InboundPacket* { return new packets::in::NamedSoundEffectPacket(); } },
            { 0x1B, []() -> packets::InboundPacket* { return new packets::in::DisconnectPacket(); } },
            { 0x1C, []() -> packets::InboundPacket* { return new packets::in::EntityStatusPacket(); } },
            { 0x1D, []() -> packets::InboundPacket* { return new packets::in::ExplosionPacket(); } },
            { 0x1E, []() -> packets::InboundPacket* { return new packets::in::UnloadChunkPacket(); } },
            { 0x1F, []() -> packets::InboundPacket* { return new packets::in::ChangeGameStatePacket(); } },
            { 0x20, []() -> packets::InboundPacket* { return new packets::in::KeepAlivePacket(); } },
            { 0x21, []() -> packets::InboundPacket* { return new packets::in::ChunkDataPacket(); } },
            { 0x22, []() -> packets::InboundPacket* { return new packets::in::EffectPacket(); } },
            { 0x23, []() -> packets::InboundPacket* { return new packets::in::ParticlePacket(); } },
            { 0x24, []() -> packets::InboundPacket* { return new packets::in::JoinGamePacket(); } },
            { 0x25, []() -> packets::InboundPacket* { return new packets::in::MapPacket(); } },
            { 0x26, []() -> packets::InboundPacket* { return new packets::in::EntityRelativeMovePacket(); } },
            { 0x27, []() -> packets::InboundPacket* { return new packets::in::EntityLookAndRelativeMovePacket(); } },
            { 0x28, []() -> packets::InboundPacket* { return new packets::in::EntityLookPacket(); } },
            { 0x29, []() -> packets::InboundPacket* { return new packets::in::EntityPacket(); } },
            { 0x2A, []() -> packets::InboundPacket* { return new packets::in::VehicleMovePacket(); } },
            { 0x2B, []() -> packets::InboundPacket* { return new packets::in::OpenSignEditorPacket(); } },
            { 0x2C, []() -> packets::InboundPacket* { return new packets::in::PlayerAbilitiesPacket(); } },
            { 0x2D, []() -> packets::InboundPacket* { return new packets::in::CombatEventPacket(); } },
            { 0x2E, []() -> packets::InboundPacket* { return new packets::in::PlayerListItemPacket(); } },
            { 0x2F, []() -> packets::InboundPacket* { return new packets::in::PlayerPositionAndLookPacket(); } },
            { 0x30, []() -> packets::InboundPacket* { return new packets::in::UseBedPacket(); } },
            { 0x31, []() -> packets::InboundPacket* { return new packets::in::UnlockRecipesPacket(); } },
            { 0x32, []() -> packets::InboundPacket* { return new packets::in::DestroyEntitiesPacket(); } },
            { 0x33, []() -> packets::InboundPacket* { return new packets::in::RemoveEntityEffectPacket(); } },
            { 0x34, []() -> packets::InboundPacket* { return new packets::in::ResourcePackSendPacket(); } },
            { 0x35, []() -> packets::InboundPacket* { return new packets::in::RespawnPacket(); } },
            { 0x36, []() -> packets::InboundPacket* { return new packets::in::EntityHeadLookPacket(); } },
            { 0x37, []() -> packets::InboundPacket* { return new packets::in::WorldBorderPacket(); } },
            { 0x38, []() -> packets::InboundPacket* { return new packets::in::CameraPacket(); } },
            { 0x39, []() -> packets::InboundPacket* { return new packets::in::HeldItemChangePacket(); } },
            { 0x3A, []() -> packets::InboundPacket* { return new packets::in::DisplayScoreboardPacket(); } },
            { 0x3B, []() -> packets::InboundPacket* { return new packets::in::EntityMetadataPacket(); } },
            { 0x3C, []() -> packets::InboundPacket* { return new packets::in::AttachEntityPacket(); } },
            { 0x3D, []() -> packets::InboundPacket* { return new packets::in::EntityVelocityPacket(); } },
            { 0x3E, []() -> packets::InboundPacket* { return new packets::in::EntityEquipmentPacket(); } },
            { 0x3F, []() -> packets::InboundPacket* { return new packets::in::SetExperiencePacket(); } },
            { 0x40, []() -> packets::InboundPacket* { return new packets::in::UpdateHealthPacket(); } },
            { 0x41, []() -> packets::InboundPacket* { return new packets::in::ScoreboardObjectivePacket(); } },
            { 0x42, []() -> packets::InboundPacket* { return new packets::in::SetPassengersPacket(); } },
            { 0x43, []() -> packets::InboundPacket* { return new packets::in::TeamsPacket(); } },
            { 0x44, []() -> packets::InboundPacket* { return new packets::in::UpdateScorePacket(); } },
            { 0x45, []() -> packets::InboundPacket* { return new packets::in::SpawnPositionPacket(); } },
            { 0x46, []() -> packets::InboundPacket* { return new packets::in::TimeUpdatePacket(); } },
            { 0x47, []() -> packets::InboundPacket* { return new packets::in::TitlePacket(); } },
            { 0x48, []() -> packets::InboundPacket* { return new packets::in::SoundEffectPacket(); } },
            { 0x49, []() -> packets::InboundPacket* { return new packets::in::PlayerListHeaderAndFooterPacket(); } },
            { 0x4A, []() -> packets::InboundPacket* { return new packets::in::CollectItemPacket(); } },
            { 0x4B, []() -> packets::InboundPacket* { return new packets::in::EntityTeleportPacket(); } },
            { 0x4C, []() -> packets::InboundPacket* { return new packets::in::EntityPropertiesPacket(); } },
            { 0x4D, []() -> packets::InboundPacket* { return new packets::in::EntityEffectPacket(); } },
            { 0x4E, []() -> packets::InboundPacket* { return new packets::in::AdvancementProgressPacket(); } },
        }
    }
};

Protocol protocol_1_11_2(Version::Minecraft_1_11_2, inboundMap_1_11_2);
Protocol_1_12_0 protocol_1_12_0(Version::Minecraft_1_12_0, inboundMap_1_12_0);

packets::InboundPacket* Protocol::CreateInboundPacket(State state, s32 id) {
    auto& packetMap = m_InboundMap[state];

    auto iter = packetMap.find(id);
    if (iter == packetMap.end()) return nullptr;

    packets::InboundPacket* packet = iter->second();

    if (packet) {
        packet->SetId(id);
        packet->SetProtocolVersion(m_Version);
    }
    
    return packet;
}

Protocol& Protocol::GetProtocol(Version version) {
    if (version == Version::Minecraft_1_10_2 ||
        version == Version::Minecraft_1_11_0 ||
        version == Version::Minecraft_1_11_2)
    {
        return protocol_1_11_2;
    }

    if (version == Version::Minecraft_1_12_0)
        return protocol_1_12_0;

    throw std::runtime_error(std::string("Unknown protocol version ") + std::to_string((s32)version));
}

} // ns protocol
} // ns mclib
