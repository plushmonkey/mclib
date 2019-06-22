#include <mclib/protocol/Protocol.h>

#include <mclib/protocol/packets/Packet.h>

namespace mc {
namespace protocol {

class Protocol_1_12_0 : public Protocol {
public:
    Protocol_1_12_0(Version version, StateMap inbound)
        : Protocol(version, inbound) {}

    // Handshake
    virtual s32 GetPacketId(packets::out::HandshakePacket) override {
        return 0x00;
    }

    // Login
    virtual s32 GetPacketId(packets::out::LoginStartPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::EncryptionResponsePacket) override {
        return 0x01;
    }

    // Status
    virtual s32 GetPacketId(packets::out::status::RequestPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::status::PingPacket) override {
        return 0x01;
    }

    // Play
    virtual s32 GetPacketId(packets::out::TeleportConfirmPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::PrepareCraftingGridPacket) override {
        return 0x01;
    }
    virtual s32 GetPacketId(packets::out::TabCompletePacket) override {
        return 0x02;
    }
    virtual s32 GetPacketId(packets::out::ChatPacket) override { return 0x03; }
    virtual s32 GetPacketId(packets::out::ClientStatusPacket) override {
        return 0x04;
    }
    virtual s32 GetPacketId(packets::out::ClientSettingsPacket) override {
        return 0x05;
    }
    virtual s32 GetPacketId(packets::out::ConfirmTransactionPacket) override {
        return 0x06;
    }
    virtual s32 GetPacketId(packets::out::EnchantItemPacket) override {
        return 0x07;
    }
    virtual s32 GetPacketId(packets::out::ClickWindowPacket) override {
        return 0x08;
    }
    virtual s32 GetPacketId(packets::out::CloseWindowPacket) override {
        return 0x09;
    }
    virtual s32 GetPacketId(packets::out::PluginMessagePacket) override {
        return 0x0A;
    }
    virtual s32 GetPacketId(packets::out::UseEntityPacket) override {
        return 0x0B;
    }
    virtual s32 GetPacketId(packets::out::KeepAlivePacket) override {
        return 0x0C;
    }
    virtual s32 GetPacketId(packets::out::PlayerPacket) override {
        return 0x0D;
    }
    virtual s32 GetPacketId(packets::out::PlayerPositionPacket) override {
        return 0x0E;
    }
    virtual s32 GetPacketId(
        packets::out::PlayerPositionAndLookPacket) override {
        return 0x0F;
    }
    virtual s32 GetPacketId(packets::out::PlayerLookPacket) override {
        return 0x10;
    }
    virtual s32 GetPacketId(packets::out::VehicleMovePacket) override {
        return 0x11;
    }
    virtual s32 GetPacketId(packets::out::SteerBoatPacket) override {
        return 0x12;
    }
    virtual s32 GetPacketId(packets::out::PlayerAbilitiesPacket) override {
        return 0x13;
    }
    virtual s32 GetPacketId(packets::out::PlayerDiggingPacket) override {
        return 0x14;
    }
    virtual s32 GetPacketId(packets::out::EntityActionPacket) override {
        return 0x15;
    }
    virtual s32 GetPacketId(packets::out::SteerVehiclePacket) override {
        return 0x16;
    }
    virtual s32 GetPacketId(packets::out::CraftingBookDataPacket) override {
        return 0x17;
    }
    virtual s32 GetPacketId(packets::out::ResourcePackStatusPacket) override {
        return 0x18;
    }
    virtual s32 GetPacketId(packets::out::AdvancementTabPacket) override {
        return 0x19;
    }
    virtual s32 GetPacketId(packets::out::HeldItemChangePacket) override {
        return 0x1A;
    }
    virtual s32 GetPacketId(
        packets::out::CreativeInventoryActionPacket) override {
        return 0x1B;
    }
    virtual s32 GetPacketId(packets::out::UpdateSignPacket) override {
        return 0x1C;
    }
    virtual s32 GetPacketId(packets::out::AnimationPacket) override {
        return 0x1D;
    }
    virtual s32 GetPacketId(packets::out::SpectatePacket) override {
        return 0x1E;
    }
    virtual s32 GetPacketId(packets::out::PlayerBlockPlacementPacket) override {
        return 0x1F;
    }
    virtual s32 GetPacketId(packets::out::UseItemPacket) override {
        return 0x20;
    }
};

class Protocol_1_12_1 : public Protocol {
public:
    Protocol_1_12_1(Version version, StateMap inbound)
        : Protocol(version, inbound) {}

    // Handshake
    virtual s32 GetPacketId(packets::out::HandshakePacket) override {
        return 0x00;
    }

    // Login
    virtual s32 GetPacketId(packets::out::LoginStartPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::EncryptionResponsePacket) override {
        return 0x01;
    }

    // Status
    virtual s32 GetPacketId(packets::out::status::RequestPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::status::PingPacket) override {
        return 0x01;
    }

    // Play
    virtual s32 GetPacketId(packets::out::TeleportConfirmPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::TabCompletePacket) override {
        return 0x01;
    }
    virtual s32 GetPacketId(packets::out::ChatPacket) override { return 0x02; }
    virtual s32 GetPacketId(packets::out::ClientStatusPacket) override {
        return 0x03;
    }
    virtual s32 GetPacketId(packets::out::ClientSettingsPacket) override {
        return 0x04;
    }
    virtual s32 GetPacketId(packets::out::ConfirmTransactionPacket) override {
        return 0x05;
    }
    virtual s32 GetPacketId(packets::out::EnchantItemPacket) override {
        return 0x06;
    }
    virtual s32 GetPacketId(packets::out::ClickWindowPacket) override {
        return 0x07;
    }
    virtual s32 GetPacketId(packets::out::CloseWindowPacket) override {
        return 0x08;
    }
    virtual s32 GetPacketId(packets::out::PluginMessagePacket) override {
        return 0x09;
    }
    virtual s32 GetPacketId(packets::out::UseEntityPacket) override {
        return 0x0A;
    }
    virtual s32 GetPacketId(packets::out::KeepAlivePacket) override {
        return 0x0B;
    }
    virtual s32 GetPacketId(packets::out::PlayerPacket) override {
        return 0x0C;
    }
    virtual s32 GetPacketId(packets::out::PlayerPositionPacket) override {
        return 0x0D;
    }
    virtual s32 GetPacketId(
        packets::out::PlayerPositionAndLookPacket) override {
        return 0x0E;
    }
    virtual s32 GetPacketId(packets::out::PlayerLookPacket) override {
        return 0x0F;
    }
    virtual s32 GetPacketId(packets::out::VehicleMovePacket) override {
        return 0x10;
    }
    virtual s32 GetPacketId(packets::out::SteerBoatPacket) override {
        return 0x11;
    }
    virtual s32 GetPacketId(packets::out::CraftRecipeRequestPacket) override {
        return 0x12;
    }
    virtual s32 GetPacketId(packets::out::PlayerAbilitiesPacket) override {
        return 0x13;
    }
    virtual s32 GetPacketId(packets::out::PlayerDiggingPacket) override {
        return 0x14;
    }
    virtual s32 GetPacketId(packets::out::EntityActionPacket) override {
        return 0x15;
    }
    virtual s32 GetPacketId(packets::out::SteerVehiclePacket) override {
        return 0x16;
    }
    virtual s32 GetPacketId(packets::out::CraftingBookDataPacket) override {
        return 0x17;
    }
    virtual s32 GetPacketId(packets::out::ResourcePackStatusPacket) override {
        return 0x18;
    }
    virtual s32 GetPacketId(packets::out::AdvancementTabPacket) override {
        return 0x19;
    }
    virtual s32 GetPacketId(packets::out::HeldItemChangePacket) override {
        return 0x1A;
    }
    virtual s32 GetPacketId(
        packets::out::CreativeInventoryActionPacket) override {
        return 0x1B;
    }
    virtual s32 GetPacketId(packets::out::UpdateSignPacket) override {
        return 0x1C;
    }
    virtual s32 GetPacketId(packets::out::AnimationPacket) override {
        return 0x1D;
    }
    virtual s32 GetPacketId(packets::out::SpectatePacket) override {
        return 0x1E;
    }
    virtual s32 GetPacketId(packets::out::PlayerBlockPlacementPacket) override {
        return 0x1F;
    }
    virtual s32 GetPacketId(packets::out::UseItemPacket) override {
        return 0x20;
    }

    virtual s32 GetPacketId(packets::out::PrepareCraftingGridPacket) override {
        throw UnsupportedPacketException(
            "PrepareCraftingGridPacket does not work with protocol 1.12.1");
    }
};

class Protocol_1_13_2 : public Protocol {
public:
    Protocol_1_13_2(Version version, StateMap inbound)
        : Protocol(version, inbound) {}

    // Handshake
    virtual s32 GetPacketId(packets::out::HandshakePacket) override {
        return 0x00;
    }

    // Login
    virtual s32 GetPacketId(packets::out::LoginStartPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::EncryptionResponsePacket) override {
        return 0x01;
    }

    // Status
    virtual s32 GetPacketId(packets::out::status::RequestPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::status::PingPacket) override {
        return 0x01;
    }

    // Play
    virtual s32 GetPacketId(packets::out::TeleportConfirmPacket) override {
        return 0x00;
    }
    // QueryBlockNBT = 0x01
    virtual s32 GetPacketId(packets::out::ChatPacket) override { return 0x02; }
    virtual s32 GetPacketId(packets::out::ClientStatusPacket) override {
        return 0x03;
    }
    virtual s32 GetPacketId(packets::out::ClientSettingsPacket) override {
        return 0x04;
    }
    virtual s32 GetPacketId(packets::out::TabCompletePacket) override {
        return 0x05;
    }
    virtual s32 GetPacketId(packets::out::ConfirmTransactionPacket) override {
        return 0x06;
    }
    virtual s32 GetPacketId(packets::out::EnchantItemPacket) override {
        return 0x07;
    }
    virtual s32 GetPacketId(packets::out::ClickWindowPacket) override {
        return 0x08;
    }
    virtual s32 GetPacketId(packets::out::CloseWindowPacket) override {
        return 0x09;
    }
    virtual s32 GetPacketId(packets::out::PluginMessagePacket) override {
        return 0x0A;
    }
    // EditBook = 0x0B
    // QueryEntityNBT = 0x0C
    virtual s32 GetPacketId(packets::out::UseEntityPacket) override {
        return 0x0D;
    }
    virtual s32 GetPacketId(packets::out::KeepAlivePacket) override {
        return 0x0E;
    }
    virtual s32 GetPacketId(packets::out::PlayerPacket) override {
        return 0x0F;
    }
    virtual s32 GetPacketId(packets::out::PlayerPositionPacket) override {
        return 0x10;
    }
    virtual s32 GetPacketId(
        packets::out::PlayerPositionAndLookPacket) override {
        return 0x11;
    }
    virtual s32 GetPacketId(packets::out::PlayerLookPacket) override {
        return 0x12;
    }
    virtual s32 GetPacketId(packets::out::VehicleMovePacket) override {
        return 0x13;
    }
    virtual s32 GetPacketId(packets::out::SteerBoatPacket) override {
        return 0x14;
    }
    // PickItem = 0x15
    virtual s32 GetPacketId(packets::out::CraftRecipeRequestPacket) override {
        return 0x16;
    }
    virtual s32 GetPacketId(packets::out::PlayerAbilitiesPacket) override {
        return 0x17;
    }
    virtual s32 GetPacketId(packets::out::PlayerDiggingPacket) override {
        return 0x18;
    }
    virtual s32 GetPacketId(packets::out::EntityActionPacket) override {
        return 0x19;
    }
    virtual s32 GetPacketId(packets::out::SteerVehiclePacket) override {
        return 0x1A;
    }
    virtual s32 GetPacketId(packets::out::CraftingBookDataPacket) override {
        return 0x1B;
    }
    // NameItem = 0x1C
    virtual s32 GetPacketId(packets::out::ResourcePackStatusPacket) override {
        return 0x1D;
    }
    virtual s32 GetPacketId(packets::out::AdvancementTabPacket) override {
        return 0x1E;
    }
    // SelectTrade = 0x1F
    // SetBeaconEffect = 0x20
    virtual s32 GetPacketId(packets::out::HeldItemChangePacket) override {
        return 0x21;
    }
    // UpdateCommandBlock = 0x22
    // UpdateCommandBlockMinecart = 0x23
    virtual s32 GetPacketId(
        packets::out::CreativeInventoryActionPacket) override {
        return 0x24;
    }
    // UpdateStructure = 0x25
    virtual s32 GetPacketId(packets::out::UpdateSignPacket) override {
        return 0x26;
    }
    virtual s32 GetPacketId(packets::out::AnimationPacket) override {
        return 0x27;
    }
    virtual s32 GetPacketId(packets::out::SpectatePacket) override {
        return 0x28;
    }
    virtual s32 GetPacketId(packets::out::PlayerBlockPlacementPacket) override {
        return 0x29;
    }
    virtual s32 GetPacketId(packets::out::UseItemPacket) override {
        return 0x2A;
    }

    virtual s32 GetPacketId(packets::out::PrepareCraftingGridPacket) override {
        throw UnsupportedPacketException(
            "PrepareCraftingGridPacket does not work with protocol 1.13.2");
    }
};

class Protocol_1_14_2 : public Protocol {
public:
    Protocol_1_14_2(Version version, StateMap inbound)
        : Protocol(version, inbound) {}

    // Handshake
    virtual s32 GetPacketId(packets::out::HandshakePacket) override {
        return 0x00;
    }

    // Login
    virtual s32 GetPacketId(packets::out::LoginStartPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::EncryptionResponsePacket) override {
        return 0x01;
    }

    // Status
    virtual s32 GetPacketId(packets::out::status::RequestPacket) override {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::status::PingPacket) override {
        return 0x01;
    }

    // Play
    virtual s32 GetPacketId(packets::out::TeleportConfirmPacket) override {
        return 0x00;
    }
    // QueryBlockNBT = 0x01
    virtual s32 GetPacketId(packets::out::ChatPacket) override { return 0x02; }
    virtual s32 GetPacketId(packets::out::ClientStatusPacket) override {
        return 0x03;
    }
    virtual s32 GetPacketId(packets::out::ClientSettingsPacket) override {
        return 0x04;
    }
    virtual s32 GetPacketId(packets::out::TabCompletePacket) override {
        return 0x05;
    }
    virtual s32 GetPacketId(packets::out::ConfirmTransactionPacket) override {
        return 0x06;
    }
    virtual s32 GetPacketId(packets::out::EnchantItemPacket) override {
        return 0x07;
    }
    virtual s32 GetPacketId(packets::out::ClickWindowPacket) override {
        return 0x08;
    }
    virtual s32 GetPacketId(packets::out::CloseWindowPacket) override {
        return 0x09;
    }
    virtual s32 GetPacketId(packets::out::PluginMessagePacket) override {
        return 0x0A;
    }
    // EditBook = 0x0B
    // QueryEntityNBT = 0x0C
    virtual s32 GetPacketId(packets::out::UseEntityPacket) override {
        return 0x0D;
    }
    virtual s32 GetPacketId(packets::out::KeepAlivePacket) override {
        return 0x0E;
    }
    virtual s32 GetPacketId(packets::out::PlayerPacket) override {
        return 0x0F;
    }
    virtual s32 GetPacketId(packets::out::PlayerPositionPacket) override {
        return 0x10;
    }
    virtual s32 GetPacketId(
        packets::out::PlayerPositionAndLookPacket) override {
        return 0x11;
    }
    virtual s32 GetPacketId(packets::out::PlayerLookPacket) override {
        return 0x12;
    }
    virtual s32 GetPacketId(packets::out::VehicleMovePacket) override {
        return 0x13;
    }
    virtual s32 GetPacketId(packets::out::SteerBoatPacket) override {
        return 0x14;
    }
    // PickItem = 0x15
    virtual s32 GetPacketId(packets::out::CraftRecipeRequestPacket) override {
        return 0x16;
    }
    virtual s32 GetPacketId(packets::out::PlayerAbilitiesPacket) override {
        return 0x17;
    }
    virtual s32 GetPacketId(packets::out::PlayerDiggingPacket) override {
        return 0x18;
    }
    virtual s32 GetPacketId(packets::out::EntityActionPacket) override {
        return 0x19;
    }
    virtual s32 GetPacketId(packets::out::SteerVehiclePacket) override {
        return 0x1A;
    }
    virtual s32 GetPacketId(packets::out::CraftingBookDataPacket) override {
        return 0x1B;
    }
    // NameItem = 0x1C
    virtual s32 GetPacketId(packets::out::ResourcePackStatusPacket) override {
        return 0x1D;
    }
    virtual s32 GetPacketId(packets::out::AdvancementTabPacket) override {
        return 0x1E;
    }
    // SelectTrade = 0x1F
    // SetBeaconEffect = 0x20
    virtual s32 GetPacketId(packets::out::HeldItemChangePacket) override {
        return 0x21;
    }
    // UpdateCommandBlock = 0x22
    // UpdateCommandBlockMinecart = 0x23
    virtual s32 GetPacketId(
        packets::out::CreativeInventoryActionPacket) override {
        return 0x24;
    }
    // UpdateStructure = 0x25
    virtual s32 GetPacketId(packets::out::UpdateSignPacket) override {
        return 0x26;
    }
    virtual s32 GetPacketId(packets::out::AnimationPacket) override {
        return 0x27;
    }
    virtual s32 GetPacketId(packets::out::SpectatePacket) override {
        return 0x28;
    }
    virtual s32 GetPacketId(packets::out::PlayerBlockPlacementPacket) override {
        return 0x29;
    }
    virtual s32 GetPacketId(packets::out::UseItemPacket) override {
        return 0x2A;
    }

    virtual s32 GetPacketId(packets::out::PrepareCraftingGridPacket) override {
        throw UnsupportedPacketException(
            "PrepareCraftingGridPacket does not work with protocol 1.14.2");
    }  // TODO check the class
};

// Protocol agnostic protocol id to packet creators.
std::unordered_map<State, std::unordered_map<s32, PacketCreator>>
    agnosticStateMap = {
        {State::Login,
         {
             {protocol::login::Disconnect,
              []() -> packets::InboundPacket* {
                  return new packets::in::DisconnectPacket();
              }},
             {protocol::login::EncryptionRequest,
              []() -> packets::InboundPacket* {
                  return new packets::in::EncryptionRequestPacket();
              }},
             {protocol::login::LoginSuccess,
              []() -> packets::InboundPacket* {
                  return new packets::in::LoginSuccessPacket();
              }},
             {protocol::login::SetCompression,
              []() -> packets::InboundPacket* {
                  return new packets::in::SetCompressionPacket();
              }},
         }},
        {State::Status,
         {
             {protocol::status::Response,
              []() -> packets::InboundPacket* {
                  return new packets::in::status::ResponsePacket();
              }},
             {protocol::status::Pong,
              []() -> packets::InboundPacket* {
                  return new packets::in::status::PongPacket();
              }},
         }},
        {State::Play,
         {
             {protocol::play::SpawnObject,
              []() -> packets::InboundPacket* {
                  return new packets::in::SpawnObjectPacket();
              }},
             {protocol::play::SpawnExperienceOrb,
              []() -> packets::InboundPacket* {
                  return new packets::in::SpawnExperienceOrbPacket();
              }},
             {protocol::play::SpawnGlobalEntity,
              []() -> packets::InboundPacket* {
                  return new packets::in::SpawnGlobalEntityPacket();
              }},
             {protocol::play::SpawnMob,
              []() -> packets::InboundPacket* {
                  return new packets::in::SpawnMobPacket();
              }},
             {protocol::play::SpawnPainting,
              []() -> packets::InboundPacket* {
                  return new packets::in::SpawnPaintingPacket();
              }},
             {protocol::play::SpawnPlayer,
              []() -> packets::InboundPacket* {
                  return new packets::in::SpawnPlayerPacket();
              }},
             {protocol::play::Animation,
              []() -> packets::InboundPacket* {
                  return new packets::in::AnimationPacket();
              }},
             {protocol::play::Statistics,
              []() -> packets::InboundPacket* {
                  return new packets::in::StatisticsPacket();
              }},
             {protocol::play::BlockBreakAnimation,
              []() -> packets::InboundPacket* {
                  return new packets::in::BlockBreakAnimationPacket();
              }},
             {protocol::play::UpdateBlockEntity,
              []() -> packets::InboundPacket* {
                  return new packets::in::UpdateBlockEntityPacket();
              }},
             {protocol::play::BlockAction,
              []() -> packets::InboundPacket* {
                  return new packets::in::BlockActionPacket();
              }},
             {protocol::play::BlockChange,
              []() -> packets::InboundPacket* {
                  return new packets::in::BlockChangePacket();
              }},
             {protocol::play::BossBar,
              []() -> packets::InboundPacket* {
                  return new packets::in::BossBarPacket();
              }},
             {protocol::play::ServerDifficulty,
              []() -> packets::InboundPacket* {
                  return new packets::in::ServerDifficultyPacket();
              }},
             {protocol::play::TabComplete,
              []() -> packets::InboundPacket* {
                  return new packets::in::TabCompletePacket();
              }},
             {protocol::play::Chat,
              []() -> packets::InboundPacket* {
                  return new packets::in::ChatPacket();
              }},
             {protocol::play::MultiBlockChange,
              []() -> packets::InboundPacket* {
                  return new packets::in::MultiBlockChangePacket();
              }},
             {protocol::play::ConfirmTransaction,
              []() -> packets::InboundPacket* {
                  return new packets::in::ConfirmTransactionPacket();
              }},
             {protocol::play::CloseWindow,
              []() -> packets::InboundPacket* {
                  return new packets::in::CloseWindowPacket();
              }},
             {protocol::play::OpenWindow,
              []() -> packets::InboundPacket* {
                  return new packets::in::OpenWindowPacket();
              }},
             {protocol::play::WindowItems,
              []() -> packets::InboundPacket* {
                  return new packets::in::WindowItemsPacket();
              }},
             {protocol::play::WindowProperty,
              []() -> packets::InboundPacket* {
                  return new packets::in::WindowPropertyPacket();
              }},
             {protocol::play::SetSlot,
              []() -> packets::InboundPacket* {
                  return new packets::in::SetSlotPacket();
              }},
             {protocol::play::SetCooldown,
              []() -> packets::InboundPacket* {
                  return new packets::in::SetCooldownPacket();
              }},
             {protocol::play::PluginMessage,
              []() -> packets::InboundPacket* {
                  return new packets::in::PluginMessagePacket();
              }},
             {protocol::play::NamedSoundEffect,
              []() -> packets::InboundPacket* {
                  return new packets::in::NamedSoundEffectPacket();
              }},
             {protocol::play::Disconnect,
              []() -> packets::InboundPacket* {
                  return new packets::in::DisconnectPacket();
              }},
             {protocol::play::EntityStatus,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityStatusPacket();
              }},
             {protocol::play::Explosion,
              []() -> packets::InboundPacket* {
                  return new packets::in::ExplosionPacket();
              }},
             {protocol::play::UnloadChunk,
              []() -> packets::InboundPacket* {
                  return new packets::in::UnloadChunkPacket();
              }},
             {protocol::play::ChangeGameState,
              []() -> packets::InboundPacket* {
                  return new packets::in::ChangeGameStatePacket();
              }},
             {protocol::play::KeepAlive,
              []() -> packets::InboundPacket* {
                  return new packets::in::KeepAlivePacket();
              }},
             {protocol::play::ChunkData,
              []() -> packets::InboundPacket* {
                  return new packets::in::ChunkDataPacket();
              }},
             {protocol::play::Effect,
              []() -> packets::InboundPacket* {
                  return new packets::in::EffectPacket();
              }},
             {protocol::play::Particle,
              []() -> packets::InboundPacket* {
                  return new packets::in::ParticlePacket();
              }},
             {protocol::play::JoinGame,
              []() -> packets::InboundPacket* {
                  return new packets::in::JoinGamePacket();
              }},
             {protocol::play::Map,
              []() -> packets::InboundPacket* {
                  return new packets::in::MapPacket();
              }},
             {protocol::play::EntityRelativeMove,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityRelativeMovePacket();
              }},
             {protocol::play::EntityLookAndRelativeMove,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityLookAndRelativeMovePacket();
              }},
             {protocol::play::EntityLook,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityLookPacket();
              }},
             {protocol::play::Entity,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityPacket();
              }},
             {protocol::play::VehicleMove,
              []() -> packets::InboundPacket* {
                  return new packets::in::VehicleMovePacket();
              }},
             {protocol::play::OpenSignEditor,
              []() -> packets::InboundPacket* {
                  return new packets::in::OpenSignEditorPacket();
              }},
             {protocol::play::PlayerAbilities,
              []() -> packets::InboundPacket* {
                  return new packets::in::PlayerAbilitiesPacket();
              }},
             {protocol::play::CombatEvent,
              []() -> packets::InboundPacket* {
                  return new packets::in::CombatEventPacket();
              }},
             {protocol::play::PlayerListItem,
              []() -> packets::InboundPacket* {
                  return new packets::in::PlayerListItemPacket();
              }},
             {protocol::play::PlayerPositionAndLook,
              []() -> packets::InboundPacket* {
                  return new packets::in::PlayerPositionAndLookPacket();
              }},
             {protocol::play::UseBed,
              []() -> packets::InboundPacket* {
                  return new packets::in::UseBedPacket();
              }},
             {protocol::play::DestroyEntities,
              []() -> packets::InboundPacket* {
                  return new packets::in::DestroyEntitiesPacket();
              }},
             {protocol::play::RemoveEntityEffect,
              []() -> packets::InboundPacket* {
                  return new packets::in::RemoveEntityEffectPacket();
              }},
             {protocol::play::ResourcePackSend,
              []() -> packets::InboundPacket* {
                  return new packets::in::ResourcePackSendPacket();
              }},
             {protocol::play::Respawn,
              []() -> packets::InboundPacket* {
                  return new packets::in::RespawnPacket();
              }},
             {protocol::play::EntityHeadLook,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityHeadLookPacket();
              }},
             {protocol::play::WorldBorder,
              []() -> packets::InboundPacket* {
                  return new packets::in::WorldBorderPacket();
              }},
             {protocol::play::Camera,
              []() -> packets::InboundPacket* {
                  return new packets::in::CameraPacket();
              }},
             {protocol::play::HeldItemChange,
              []() -> packets::InboundPacket* {
                  return new packets::in::HeldItemChangePacket();
              }},
             {protocol::play::DisplayScoreboard,
              []() -> packets::InboundPacket* {
                  return new packets::in::DisplayScoreboardPacket();
              }},
             {protocol::play::EntityMetadata,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityMetadataPacket();
              }},
             {protocol::play::AttachEntity,
              []() -> packets::InboundPacket* {
                  return new packets::in::AttachEntityPacket();
              }},
             {protocol::play::EntityVelocity,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityVelocityPacket();
              }},
             {protocol::play::EntityEquipment,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityEquipmentPacket();
              }},
             {protocol::play::SetExperience,
              []() -> packets::InboundPacket* {
                  return new packets::in::SetExperiencePacket();
              }},
             {protocol::play::UpdateHealth,
              []() -> packets::InboundPacket* {
                  return new packets::in::UpdateHealthPacket();
              }},
             {protocol::play::ScoreboardObjective,
              []() -> packets::InboundPacket* {
                  return new packets::in::ScoreboardObjectivePacket();
              }},
             {protocol::play::SetPassengers,
              []() -> packets::InboundPacket* {
                  return new packets::in::SetPassengersPacket();
              }},
             {protocol::play::Teams,
              []() -> packets::InboundPacket* {
                  return new packets::in::TeamsPacket();
              }},
             {protocol::play::UpdateScore,
              []() -> packets::InboundPacket* {
                  return new packets::in::UpdateScorePacket();
              }},
             {protocol::play::SpawnPosition,
              []() -> packets::InboundPacket* {
                  return new packets::in::SpawnPositionPacket();
              }},
             {protocol::play::TimeUpdate,
              []() -> packets::InboundPacket* {
                  return new packets::in::TimeUpdatePacket();
              }},
             {protocol::play::Title,
              []() -> packets::InboundPacket* {
                  return new packets::in::TitlePacket();
              }},
             {protocol::play::SoundEffect,
              []() -> packets::InboundPacket* {
                  return new packets::in::SoundEffectPacket();
              }},
             {protocol::play::PlayerListHeaderAndFooter,
              []() -> packets::InboundPacket* {
                  return new packets::in::PlayerListHeaderAndFooterPacket();
              }},
             {protocol::play::CollectItem,
              []() -> packets::InboundPacket* {
                  return new packets::in::CollectItemPacket();
              }},
             {protocol::play::EntityTeleport,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityTeleportPacket();
              }},
             {protocol::play::EntityProperties,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityPropertiesPacket();
              }},
             {protocol::play::EntityEffect,
              []() -> packets::InboundPacket* {
                  return new packets::in::EntityEffectPacket();
              }},
             {protocol::play::AdvancementProgress,
              []() -> packets::InboundPacket* {
                  return new packets::in::AdvancementProgressPacket();
              }},
             {protocol::play::Advancements,
              []() -> packets::InboundPacket* {
                  return new packets::in::AdvancementsPacket();
              }},
             {protocol::play::UnlockRecipes,
              []() -> packets::InboundPacket* {
                  return new packets::in::UnlockRecipesPacket();
              }},
         }}};

Protocol::StateMap inboundMap_1_11_2 = {
    {State::Login,
     {
         {0x00, protocol::login::Disconnect},
         {0x01, protocol::login::EncryptionRequest},
         {0x02, protocol::login::LoginSuccess},
         {0x03, protocol::login::SetCompression},
     }},
    {State::Status,
     {
         {0x00, protocol::status::Response},
         {0x01, protocol::status::Pong},
     }},
    {State::Play,
     {
         {0x00, protocol::play::SpawnObject},
         {0x01, protocol::play::SpawnExperienceOrb},
         {0x02, protocol::play::SpawnGlobalEntity},
         {0x03, protocol::play::SpawnMob},
         {0x04, protocol::play::SpawnPainting},
         {0x05, protocol::play::SpawnPlayer},
         {0x06, protocol::play::Animation},
         {0x07, protocol::play::Statistics},
         {0x08, protocol::play::BlockBreakAnimation},
         {0x09, protocol::play::UpdateBlockEntity},
         {0x0A, protocol::play::BlockAction},
         {0x0B, protocol::play::BlockChange},
         {0x0C, protocol::play::BossBar},
         {0x0D, protocol::play::ServerDifficulty},
         {0x0E, protocol::play::TabComplete},
         {0x0F, protocol::play::Chat},
         {0x10, protocol::play::MultiBlockChange},
         {0x11, protocol::play::ConfirmTransaction},
         {0x12, protocol::play::CloseWindow},
         {0x13, protocol::play::OpenWindow},
         {0x14, protocol::play::WindowItems},
         {0x15, protocol::play::WindowProperty},
         {0x16, protocol::play::SetSlot},
         {0x17, protocol::play::SetCooldown},
         {0x18, protocol::play::PluginMessage},
         {0x19, protocol::play::NamedSoundEffect},
         {0x1A, protocol::play::Disconnect},
         {0x1B, protocol::play::EntityStatus},
         {0x1C, protocol::play::Explosion},
         {0x1D, protocol::play::UnloadChunk},
         {0x1E, protocol::play::ChangeGameState},
         {0x1F, protocol::play::KeepAlive},
         {0x20, protocol::play::ChunkData},
         {0x21, protocol::play::Effect},
         {0x22, protocol::play::Particle},
         {0x23, protocol::play::JoinGame},
         {0x24, protocol::play::Map},
         {0x25, protocol::play::EntityRelativeMove},
         {0x26, protocol::play::EntityLookAndRelativeMove},
         {0x27, protocol::play::EntityLook},
         {0x28, protocol::play::Entity},
         {0x29, protocol::play::VehicleMove},
         {0x2A, protocol::play::OpenSignEditor},
         {0x2B, protocol::play::PlayerAbilities},
         {0x2C, protocol::play::CombatEvent},
         {0x2D, protocol::play::PlayerListItem},
         {0x2E, protocol::play::PlayerPositionAndLook},
         {0x2F, protocol::play::UseBed},
         {0x30, protocol::play::DestroyEntities},
         {0x31, protocol::play::RemoveEntityEffect},
         {0x32, protocol::play::ResourcePackSend},
         {0x33, protocol::play::Respawn},
         {0x34, protocol::play::EntityHeadLook},
         {0x35, protocol::play::WorldBorder},
         {0x36, protocol::play::Camera},
         {0x37, protocol::play::HeldItemChange},
         {0x38, protocol::play::DisplayScoreboard},
         {0x39, protocol::play::EntityMetadata},
         {0x3A, protocol::play::AttachEntity},
         {0x3B, protocol::play::EntityVelocity},
         {0x3C, protocol::play::EntityEquipment},
         {0x3D, protocol::play::SetExperience},
         {0x3E, protocol::play::UpdateHealth},
         {0x3F, protocol::play::ScoreboardObjective},
         {0x40, protocol::play::SetPassengers},
         {0x41, protocol::play::Teams},
         {0x42, protocol::play::UpdateScore},
         {0x43, protocol::play::SpawnPosition},
         {0x44, protocol::play::TimeUpdate},
         {0x45, protocol::play::Title},
         {0x46, protocol::play::SoundEffect},
         {0x47, protocol::play::PlayerListHeaderAndFooter},
         {0x48, protocol::play::CollectItem},
         {0x49, protocol::play::EntityTeleport},
         {0x4A, protocol::play::EntityProperties},
         {0x4B, protocol::play::EntityEffect},
     }}};

Protocol::StateMap inboundMap_1_12_0 = {
    {State::Login,
     {
         {0x00, protocol::login::Disconnect},
         {0x01, protocol::login::EncryptionRequest},
         {0x02, protocol::login::LoginSuccess},
         {0x03, protocol::login::SetCompression},
     }},
    {State::Status,
     {
         {0x00, protocol::status::Response},
         {0x01, protocol::status::Pong},
     }},
    {State::Play,
     {
         {0x00, protocol::play::SpawnObject},
         {0x01, protocol::play::SpawnExperienceOrb},
         {0x02, protocol::play::SpawnGlobalEntity},
         {0x03, protocol::play::SpawnMob},
         {0x04, protocol::play::SpawnPainting},
         {0x05, protocol::play::SpawnPlayer},
         {0x06, protocol::play::Animation},
         {0x07, protocol::play::Statistics},
         {0x08, protocol::play::BlockBreakAnimation},
         {0x09, protocol::play::UpdateBlockEntity},
         {0x0A, protocol::play::BlockAction},
         {0x0B, protocol::play::BlockChange},
         {0x0C, protocol::play::BossBar},
         {0x0D, protocol::play::ServerDifficulty},
         {0x0E, protocol::play::TabComplete},
         {0x0F, protocol::play::Chat},
         {0x10, protocol::play::MultiBlockChange},
         {0x11, protocol::play::ConfirmTransaction},
         {0x12, protocol::play::CloseWindow},
         {0x13, protocol::play::OpenWindow},
         {0x14, protocol::play::WindowItems},
         {0x15, protocol::play::WindowProperty},
         {0x16, protocol::play::SetSlot},
         {0x17, protocol::play::SetCooldown},
         {0x18, protocol::play::PluginMessage},
         {0x19, protocol::play::NamedSoundEffect},
         {0x1A, protocol::play::Disconnect},
         {0x1B, protocol::play::EntityStatus},
         {0x1C, protocol::play::Explosion},
         {0x1D, protocol::play::UnloadChunk},
         {0x1E, protocol::play::ChangeGameState},
         {0x1F, protocol::play::KeepAlive},
         {0x20, protocol::play::ChunkData},
         {0x21, protocol::play::Effect},
         {0x22, protocol::play::Particle},
         {0x23, protocol::play::JoinGame},
         {0x24, protocol::play::Map},
         {0x25, protocol::play::Entity},
         {0x26, protocol::play::EntityRelativeMove},
         {0x27, protocol::play::EntityLookAndRelativeMove},
         {0x28, protocol::play::EntityLook},
         {0x29, protocol::play::VehicleMove},
         {0x2A, protocol::play::OpenSignEditor},
         {0x2B, protocol::play::PlayerAbilities},
         {0x2C, protocol::play::CombatEvent},
         {0x2D, protocol::play::PlayerListItem},
         {0x2E, protocol::play::PlayerPositionAndLook},
         {0x2F, protocol::play::UseBed},
         {0x30, protocol::play::UnlockRecipes},
         {0x31, protocol::play::DestroyEntities},
         {0x32, protocol::play::RemoveEntityEffect},
         {0x33, protocol::play::ResourcePackSend},
         {0x34, protocol::play::Respawn},
         {0x35, protocol::play::EntityHeadLook},
         {0x36, protocol::play::AdvancementProgress},
         {0x37, protocol::play::WorldBorder},
         {0x38, protocol::play::Camera},
         {0x39, protocol::play::HeldItemChange},
         {0x3A, protocol::play::DisplayScoreboard},
         {0x3B, protocol::play::EntityMetadata},
         {0x3C, protocol::play::AttachEntity},
         {0x3D, protocol::play::EntityVelocity},
         {0x3E, protocol::play::EntityEquipment},
         {0x3F, protocol::play::SetExperience},
         {0x40, protocol::play::UpdateHealth},
         {0x41, protocol::play::ScoreboardObjective},
         {0x42, protocol::play::SetPassengers},
         {0x43, protocol::play::Teams},
         {0x44, protocol::play::UpdateScore},
         {0x45, protocol::play::SpawnPosition},
         {0x46, protocol::play::TimeUpdate},
         {0x47, protocol::play::Title},
         {0x48, protocol::play::SoundEffect},
         {0x49, protocol::play::PlayerListHeaderAndFooter},
         {0x4A, protocol::play::CollectItem},
         {0x4B, protocol::play::EntityTeleport},
         {0x4C, protocol::play::Advancements},
         {0x4D, protocol::play::EntityProperties},
         {0x4E, protocol::play::EntityEffect},
     }}};

Protocol::StateMap inboundMap_1_12_1 = {
    {State::Login,
     {
         {0x00, protocol::login::Disconnect},
         {0x01, protocol::login::EncryptionRequest},
         {0x02, protocol::login::LoginSuccess},
         {0x03, protocol::login::SetCompression},
     }},
    {State::Status,
     {
         {0x00, protocol::status::Response},
         {0x01, protocol::status::Pong},
     }},
    {State::Play,
     {
         {0x00, protocol::play::SpawnObject},
         {0x01, protocol::play::SpawnExperienceOrb},
         {0x02, protocol::play::SpawnGlobalEntity},
         {0x03, protocol::play::SpawnMob},
         {0x04, protocol::play::SpawnPainting},
         {0x05, protocol::play::SpawnPlayer},
         {0x06, protocol::play::Animation},
         {0x07, protocol::play::Statistics},
         {0x08, protocol::play::BlockBreakAnimation},
         {0x09, protocol::play::UpdateBlockEntity},
         {0x0A, protocol::play::BlockAction},
         {0x0B, protocol::play::BlockChange},
         {0x0C, protocol::play::BossBar},
         {0x0D, protocol::play::ServerDifficulty},
         {0x0E, protocol::play::TabComplete},
         {0x0F, protocol::play::Chat},
         {0x10, protocol::play::MultiBlockChange},
         {0x11, protocol::play::ConfirmTransaction},
         {0x12, protocol::play::CloseWindow},
         {0x13, protocol::play::OpenWindow},
         {0x14, protocol::play::WindowItems},
         {0x15, protocol::play::WindowProperty},
         {0x16, protocol::play::SetSlot},
         {0x17, protocol::play::SetCooldown},
         {0x18, protocol::play::PluginMessage},
         {0x19, protocol::play::NamedSoundEffect},
         {0x1A, protocol::play::Disconnect},
         {0x1B, protocol::play::EntityStatus},
         {0x1C, protocol::play::Explosion},
         {0x1D, protocol::play::UnloadChunk},
         {0x1E, protocol::play::ChangeGameState},
         {0x1F, protocol::play::KeepAlive},
         {0x20, protocol::play::ChunkData},
         {0x21, protocol::play::Effect},
         {0x22, protocol::play::Particle},
         {0x23, protocol::play::JoinGame},
         {0x24, protocol::play::Map},
         {0x25, protocol::play::Entity},
         {0x26, protocol::play::EntityRelativeMove},
         {0x27, protocol::play::EntityLookAndRelativeMove},
         {0x28, protocol::play::EntityLook},
         {0x29, protocol::play::VehicleMove},
         {0x2A, protocol::play::OpenSignEditor},

         {0x2B, protocol::play::CraftRecipeResponse},

         {0x2C, protocol::play::PlayerAbilities},
         {0x2D, protocol::play::CombatEvent},
         {0x2E, protocol::play::PlayerListItem},
         {0x2F, protocol::play::PlayerPositionAndLook},
         {0x30, protocol::play::UseBed},
         {0x31, protocol::play::UnlockRecipes},
         {0x32, protocol::play::DestroyEntities},
         {0x33, protocol::play::RemoveEntityEffect},
         {0x34, protocol::play::ResourcePackSend},
         {0x35, protocol::play::Respawn},
         {0x36, protocol::play::EntityHeadLook},
         {0x37, protocol::play::AdvancementProgress},
         {0x38, protocol::play::WorldBorder},
         {0x39, protocol::play::Camera},
         {0x3A, protocol::play::HeldItemChange},
         {0x3B, protocol::play::DisplayScoreboard},
         {0x3C, protocol::play::EntityMetadata},
         {0x3D, protocol::play::AttachEntity},
         {0x3E, protocol::play::EntityVelocity},
         {0x3F, protocol::play::EntityEquipment},
         {0x40, protocol::play::SetExperience},
         {0x41, protocol::play::UpdateHealth},
         {0x42, protocol::play::ScoreboardObjective},
         {0x43, protocol::play::SetPassengers},
         {0x44, protocol::play::Teams},
         {0x45, protocol::play::UpdateScore},
         {0x46, protocol::play::SpawnPosition},
         {0x47, protocol::play::TimeUpdate},
         {0x48, protocol::play::Title},
         {0x49, protocol::play::SoundEffect},
         {0x4A, protocol::play::PlayerListHeaderAndFooter},
         {0x4B, protocol::play::CollectItem},
         {0x4C, protocol::play::EntityTeleport},
         {0x4D, protocol::play::Advancements},
         {0x4E, protocol::play::EntityProperties},
         {0x4F, protocol::play::EntityEffect},
     }}};

Protocol::StateMap inboundMap_1_13_2 = {
    {State::Login,
     {
         {0x00, protocol::login::Disconnect},
         {0x01, protocol::login::EncryptionRequest},
         {0x02, protocol::login::LoginSuccess},
         {0x03, protocol::login::SetCompression},
         // 0x04 LoginPluginRequest
     }},
    {State::Status,
     {
         {0x00, protocol::status::Response},
         {0x01, protocol::status::Pong},
     }},
    {State::Play,
     {
         {0x00, protocol::play::SpawnObject},
         {0x01, protocol::play::SpawnExperienceOrb},
         {0x02, protocol::play::SpawnGlobalEntity},
         {0x03, protocol::play::SpawnMob},
         {0x04, protocol::play::SpawnPainting},
         {0x05, protocol::play::SpawnPlayer},
         {0x06, protocol::play::Animation},
         {0x07, protocol::play::Statistics},
         {0x08, protocol::play::BlockBreakAnimation},
         {0x09, protocol::play::UpdateBlockEntity},
         {0x0A, protocol::play::BlockAction},
         {0x0B, protocol::play::BlockChange},
         {0x0C, protocol::play::BossBar},
         {0x0D, protocol::play::ServerDifficulty},
         {0x0E, protocol::play::Chat},
         {0x0F, protocol::play::MultiBlockChange},
         {0x10, protocol::play::TabComplete},
         // { 0x11, protocol::play::DeclareCommands },
         {0x12, protocol::play::ConfirmTransaction},
         {0x13, protocol::play::CloseWindow},
         {0x14, protocol::play::OpenWindow},
         {0x15, protocol::play::WindowItems},
         {0x16, protocol::play::WindowProperty},
         {0x17, protocol::play::SetSlot},
         {0x18, protocol::play::SetCooldown},
         {0x19, protocol::play::PluginMessage},
         {0x1A, protocol::play::NamedSoundEffect},
         {0x1B, protocol::play::Disconnect},
         {0x1C, protocol::play::EntityStatus},
         // { 0x1D, protocol::play::NBTQueryResponse },
         {0x1E, protocol::play::Explosion},
         {0x1F, protocol::play::UnloadChunk},
         {0x20, protocol::play::ChangeGameState},
         {0x21, protocol::play::KeepAlive},
         {0x22, protocol::play::ChunkData},
         {0x23, protocol::play::Effect},
         {0x24, protocol::play::Particle},
         {0x25, protocol::play::JoinGame},
         {0x26, protocol::play::Map},
         {0x27, protocol::play::Entity},
         {0x28, protocol::play::EntityRelativeMove},
         {0x29, protocol::play::EntityLookAndRelativeMove},
         {0x2A, protocol::play::EntityLook},
         {0x2B, protocol::play::VehicleMove},
         {0x2C, protocol::play::OpenSignEditor},
         {0x2D, protocol::play::CraftRecipeResponse},
         {0x2E, protocol::play::PlayerAbilities},
         {0x2F, protocol::play::CombatEvent},
         {0x30, protocol::play::PlayerListItem},
         // { 0x31, protocol::play::FacePlayer },
         {0x32, protocol::play::PlayerPositionAndLook},
         {0x33, protocol::play::UseBed},
         {0x34, protocol::play::UnlockRecipes},
         {0x35, protocol::play::DestroyEntities},
         {0x36, protocol::play::RemoveEntityEffect},
         {0x37, protocol::play::ResourcePackSend},
         {0x38, protocol::play::Respawn},
         {0x39, protocol::play::EntityHeadLook},
         // { 0x3A, protocol::play::SelectAdvancementTab },
         {0x3B, protocol::play::WorldBorder},
         {0x3C, protocol::play::Camera},
         {0x3D, protocol::play::HeldItemChange},
         {0x3E, protocol::play::DisplayScoreboard},
         {0x3F, protocol::play::EntityMetadata},
         {0x40, protocol::play::AttachEntity},
         {0x41, protocol::play::EntityVelocity},
         {0x42, protocol::play::EntityEquipment},
         {0x43, protocol::play::SetExperience},
         {0x44, protocol::play::UpdateHealth},
         {0x45, protocol::play::ScoreboardObjective},
         {0x46, protocol::play::SetPassengers},
         {0x47, protocol::play::Teams},
         {0x48, protocol::play::UpdateScore},
         {0x49, protocol::play::SpawnPosition},
         {0x4A, protocol::play::TimeUpdate},
         {0x4B, protocol::play::Title},
         // { 0x4C, protocol::play::StopSound },
         {0x4D, protocol::play::SoundEffect},
         {0x4E, protocol::play::PlayerListHeaderAndFooter},
         {0x4F, protocol::play::CollectItem},
         {0x50, protocol::play::EntityTeleport},
         {0x51, protocol::play::Advancements},
         {0x52, protocol::play::EntityProperties},
         {0x53, protocol::play::EntityEffect},
         // { 0x54, protocol::play::DeclareRecipes },
     }}};

Protocol::StateMap inboundMap_1_14_2 = {
    {State::Login,
     {
         {0x00, protocol::login::Disconnect},
         {0x01, protocol::login::EncryptionRequest},
         {0x02, protocol::login::LoginSuccess},
         {0x03, protocol::login::SetCompression},
         // 0x04 LoginPluginRequest
     }},
    {State::Status,
     {
         {0x00, protocol::status::Response},
         {0x01, protocol::status::Pong},
     }},
    {State::Play,
     {
         {0x00, protocol::play::SpawnObject},
         {0x01, protocol::play::SpawnExperienceOrb},
         {0x02, protocol::play::SpawnGlobalEntity},
         {0x03, protocol::play::SpawnMob},
         {0x04, protocol::play::SpawnPainting},
         {0x05, protocol::play::SpawnPlayer},
         {0x06, protocol::play::Animation},
         {0x07, protocol::play::Statistics},
         {0x08, protocol::play::BlockBreakAnimation},
         {0x09, protocol::play::UpdateBlockEntity},
         {0x0A, protocol::play::BlockAction},
         {0x0B, protocol::play::BlockChange},
         {0x0C, protocol::play::BossBar},
         {0x0D, protocol::play::ServerDifficulty},
         {0x0E, protocol::play::Chat},
         {0x0F, protocol::play::MultiBlockChange},
         {0x10, protocol::play::TabComplete},
         {0x12, protocol::play::ConfirmTransaction},
         {0x13, protocol::play::CloseWindow},
         {0x14, protocol::play::WindowItems},
         {0x15, protocol::play::WindowProperty},
         {0x16, protocol::play::SetSlot},
         {0x17, protocol::play::SetCooldown},
         {0x18, protocol::play::PluginMessage},
         {0x19, protocol::play::NamedSoundEffect},
         {0x1A, protocol::play::Disconnect},
         {0x1B, protocol::play::EntityStatus},
         {0x1C, protocol::play::Explosion},
         {0x1D, protocol::play::UnloadChunk},
         {0x1E, protocol::play::ChangeGameState},
         {0x1F, protocol::play::OpenHorseWindow},
         {0x20, protocol::play::KeepAlive},
         {0x21, protocol::play::ChunkData},
         {0x22, protocol::play::Effect},
         {0x23, protocol::play::Particle},
         {0x24, protocol::play::UpdateLight},
         {0x25, protocol::play::JoinGame},
         {0x26, protocol::play::Map},  // TODO MapData??
         {0x27, protocol::play::TradeList},
         {0x28, protocol::play::EntityRelativeMove},
         {0x29, protocol::play::EntityLookAndRelativeMove},
         {0x2A, protocol::play::EntityLook},
         {0x2B, protocol::play::Entity},
         {0x2C, protocol::play::VehicleMove},
         {0x2D, protocol::play::OpenBook},
         {0x2E, protocol::play::OpenWindow},
         {0x2F, protocol::play::OpenSignEditor},
         {0x30, protocol::play::CraftRecipeResponse},
         {0x31, protocol::play::PlayerAbilities},
         {0x32, protocol::play::CombatEvent},
         {0x33, protocol::play::PlayerInfo},
         {0x34, protocol::play::FacePlayer},
         {0x35, protocol::play::PlayerPositionAndLook},
         {0x36, protocol::play::UnlockRecipes},
         {0x37, protocol::play::DestroyEntities},
         {0x38, protocol::play::RemoveEntityEffect},
         {0x39, protocol::play::ResourcePackSend},
         {0x3A, protocol::play::Respawn},
         {0x3B, protocol::play::EntityHeadLook},
         {0x3C, protocol::play::SelectAdvancementTab},
         {0x3D, protocol::play::WorldBorder},
         {0x3E, protocol::play::Camera},
         {0x3F, protocol::play::HeldItemChange},
         {0x40, protocol::play::UpdateViewPosition},
         {0x41, protocol::play::UpdateViewDistance},
         {0x42, protocol::play::DisplayScoreboard},
         {0x43, protocol::play::EntityMetadata},
         {0x44, protocol::play::AttachEntity},
         {0x45, protocol::play::EntityVelocity},
         {0x46, protocol::play::EntityEquipment},
         {0x47, protocol::play::SetExperience},
         {0x48, protocol::play::UpdateHealth},
         {0x49, protocol::play::ScoreboardObjective},
         {0x4A, protocol::play::SetPassengers},
         {0x4B, protocol::play::Teams},
         {0x4C, protocol::play::UpdateScore},
         {0x4D, protocol::play::SpawnPosition},
         {0x4E, protocol::play::TimeUpdate},
         {0x4F, protocol::play::Title},
         {0x50, protocol::play::EntitySoundEffect},
         {0x51, protocol::play::SoundEffect},
         {0x52, protocol::play::StopSound},
         {0x53, protocol::play::PlayerListHeaderAndFooter},
         {0x54, protocol::play::NBTQueryResponse},
         {0x55, protocol::play::CollectItem},
         {0x56, protocol::play::EntityTeleport},
         {0x57, protocol::play::Advancements},
         {0x58, protocol::play::EntityProperties},
         {0x59, protocol::play::EntityEffect},
         {0x5A, protocol::play::DeclareRecipes},
         {0x5B, protocol::play::Tags},
     }}};

const std::unordered_map<Version, std::shared_ptr<Protocol>> protocolMap = {
    {Version::Minecraft_1_10_2,
     std::make_shared<Protocol>(Version::Minecraft_1_10_2, inboundMap_1_11_2)},
    {Version::Minecraft_1_11_0,
     std::make_shared<Protocol>(Version::Minecraft_1_11_0, inboundMap_1_11_2)},
    {Version::Minecraft_1_11_2,
     std::make_shared<Protocol>(Version::Minecraft_1_11_2, inboundMap_1_11_2)},
    {Version::Minecraft_1_12_0,
     std::make_shared<Protocol_1_12_0>(Version::Minecraft_1_12_0,
                                       inboundMap_1_12_0)},
    {Version::Minecraft_1_12_1,
     std::make_shared<Protocol_1_12_1>(Version::Minecraft_1_12_1,
                                       inboundMap_1_12_1)},
    {Version::Minecraft_1_12_2,
     std::make_shared<Protocol_1_12_1>(Version::Minecraft_1_12_2,
                                       inboundMap_1_12_1)},
    {Version::Minecraft_1_13_2,
     std::make_shared<Protocol_1_13_2>(Version::Minecraft_1_13_2,
                                       inboundMap_1_13_2)},
    {Version::Minecraft_1_14_2,
     std::make_shared<Protocol_1_14_2>(Version::Minecraft_1_14_2,
                                       inboundMap_1_14_2)},
};

bool Protocol::GetAgnosticId(State state, s32 protocolId, s32& agnosticId) {
    auto& packetMap = m_InboundMap[state];
    auto iter = packetMap.find(protocolId);

    if (iter == packetMap.end()) return false;

    agnosticId = iter->second;

    return true;
}

packets::InboundPacket* Protocol::CreateInboundPacket(State state,
                                                      s32 protocolId) {
    s32 agnosticId = 0;

    if (!GetAgnosticId(state, protocolId, agnosticId)) return nullptr;

    packets::InboundPacket* packet = nullptr;

    auto stateMap = agnosticStateMap[state];
    auto iter = stateMap.find(agnosticId);
    if (iter != stateMap.end()) {
        packet = iter->second();

        if (packet) {
            packet->SetId(protocolId);
            packet->SetProtocolVersion(m_Version);
        }
    }

    return packet;
}

Protocol& Protocol::GetProtocol(Version version) {
    auto&& iter = protocolMap.find(version);

    if (iter == protocolMap.end()) {
        throw std::runtime_error(std::string("Unknown protocol version ") +
                                 std::to_string((s32)version));
    }

    return *iter->second;
}

std::string to_string(Version version) {
    static const std::unordered_map<mc::protocol::Version, std::string>
        mapping = {
            {mc::protocol::Version::Minecraft_1_10_2, "1.10.2"},
            {mc::protocol::Version::Minecraft_1_11_0, "1.11.0"},
            {mc::protocol::Version::Minecraft_1_11_2, "1.11.2"},
            {mc::protocol::Version::Minecraft_1_12_0, "1.12.0"},
            {mc::protocol::Version::Minecraft_1_12_1, "1.12.1"},
            {mc::protocol::Version::Minecraft_1_12_2, "1.12.2"},
            {mc::protocol::Version::Minecraft_1_13_2, "1.13.2"},
            {mc::protocol::Version::Minecraft_1_14_2, "1.14.2"},
        };

    auto iter = mapping.find(version);
    if (iter == mapping.end()) return "Unknown";
    return iter->second;
}

}  // namespace protocol
}  // namespace mc
