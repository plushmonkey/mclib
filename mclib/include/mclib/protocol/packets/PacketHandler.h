#ifndef PACKETS_PACKET_HANDLER_H_
#define PACKETS_PACKET_HANDLER_H_

#include <mclib/protocol/packets/Packet.h>

namespace mc {
namespace protocol {
namespace packets {

class PacketDispatcher;

/**
 * Used to listen for specific incoming packets.
 * The order in which these packets are handled isn't set, so care must be taken
 * when listening. The internal systems might not be updated when the handler is
 * called. This is mostly for internal classes, other classes should listen to
 * the other systems instead, like EntityManager/PlayerManager, to ensure that
 * all the internal data is updated before being handled.
 */
class PacketHandler {
private:
    PacketDispatcher* m_Dispatcher;

public:
    MCLIB_API PacketHandler(PacketDispatcher* dispatcher);
    virtual MCLIB_API ~PacketHandler();

    MCLIB_API PacketDispatcher* GetDispatcher();

    // Login protocol state
    virtual void HandlePacket(in::DisconnectPacket* packet) {}         // 0x00
    virtual void HandlePacket(in::EncryptionRequestPacket* packet) {}  // 0x01
    virtual void HandlePacket(in::LoginSuccessPacket* packet) {}       // 0x02
    virtual void HandlePacket(in::SetCompressionPacket* packet) {}     // 0x03

    // Status protocol state
    virtual void HandlePacket(in::status::ResponsePacket* packet) {}  // 0x00
    virtual void HandlePacket(in::status::PongPacket* packet) {}      // 0x01

    // Play protocol state
    virtual void HandlePacket(in::SpawnObjectPacket* packet) {}         // 0x00
    virtual void HandlePacket(in::SpawnExperienceOrbPacket* packet) {}  // 0x01
    virtual void HandlePacket(in::SpawnGlobalEntityPacket* packet) {}   // 0x02
    virtual void HandlePacket(in::SpawnMobPacket* packet) {}            // 0x03
    virtual void HandlePacket(in::SpawnPaintingPacket* packet) {}       // 0x04
    virtual void HandlePacket(in::SpawnPlayerPacket* packet) {}         // 0x05
    virtual void HandlePacket(in::AnimationPacket* packet) {}           // 0x06
    virtual void HandlePacket(in::StatisticsPacket* packet) {}          // 0x07
    virtual void HandlePacket(in::AdvancementsPacket* packet) {}
    virtual void HandlePacket(in::BlockBreakAnimationPacket* packet) {}  // 0x08
    virtual void HandlePacket(in::UpdateBlockEntityPacket* packet) {}    // 0x09
    virtual void HandlePacket(in::BlockActionPacket* packet) {}          // 0x0A
    virtual void HandlePacket(in::BlockChangePacket* packet) {}          // 0x0B
    virtual void HandlePacket(in::BossBarPacket* packet) {}              // 0x0C
    virtual void HandlePacket(in::ServerDifficultyPacket* packet) {}     // 0x0D
    virtual void HandlePacket(in::TabCompletePacket* packet) {}          // 0x0E
    virtual void HandlePacket(in::ChatPacket* packet) {}                 // 0x0F
    virtual void HandlePacket(in::MultiBlockChangePacket* packet) {}     // 0x10
    virtual void HandlePacket(in::ConfirmTransactionPacket* packet) {}   // 0x11
    virtual void HandlePacket(in::CloseWindowPacket* packet) {}          // 0x12
    virtual void HandlePacket(in::OpenWindowPacket* packet) {}           // 0x13
    virtual void HandlePacket(in::WindowItemsPacket* packet) {}          // 0x14
    virtual void HandlePacket(in::WindowPropertyPacket* packet) {}       // 0x15
    virtual void HandlePacket(in::SetSlotPacket* packet) {}              // 0x16
    virtual void HandlePacket(in::SetCooldownPacket* packet) {}          // 0x17
    virtual void HandlePacket(in::PluginMessagePacket* packet) {}        // 0x18
    virtual void HandlePacket(in::NamedSoundEffectPacket* packet) {}     // 0x19
    // virtual void HandlePacket(in::DisconnectPacket* packet) { } // 0x1A
    virtual void HandlePacket(in::EntityStatusPacket* packet) {}        // 0x1B
    virtual void HandlePacket(in::ExplosionPacket* packet) {}           // 0x1C
    virtual void HandlePacket(in::UnloadChunkPacket* packet) {}         // 0x1D
    virtual void HandlePacket(in::ChangeGameStatePacket* packet) {}     // 0x1E
    virtual void HandlePacket(in::KeepAlivePacket* packet) {}           // 0x1F
    virtual void HandlePacket(in::ChunkDataPacket* packet) {}           // 0x20
    virtual void HandlePacket(in::EffectPacket* packet) {}              // 0x21
    virtual void HandlePacket(in::ParticlePacket* packet) {}            // 0x22
    virtual void HandlePacket(in::JoinGamePacket* packet) {}            // 0x23
    virtual void HandlePacket(in::MapPacket* packet) {}                 // 0x24
    virtual void HandlePacket(in::EntityRelativeMovePacket* packet) {}  // 0x25
    virtual void HandlePacket(in::EntityLookAndRelativeMovePacket* packet) {
    }                                                                // 0x26
    virtual void HandlePacket(in::EntityLookPacket* packet) {}       // 0x27
    virtual void HandlePacket(in::EntityPacket* packet) {}           // 0x28
    virtual void HandlePacket(in::VehicleMovePacket* packet) {}      // 0x29
    virtual void HandlePacket(in::OpenSignEditorPacket* packet) {}   // 0x2A
    virtual void HandlePacket(in::PlayerAbilitiesPacket* packet) {}  // 0x2B
    virtual void HandlePacket(in::CombatEventPacket* packet) {}      // 0x2C
    virtual void HandlePacket(in::PlayerListItemPacket* packet) {}   // 0x2D
    virtual void HandlePacket(in::PlayerPositionAndLookPacket* packet) {
    }                                                                // 0x2E
    virtual void HandlePacket(in::UseBedPacket* packet) {}           // 0x2F
    virtual void HandlePacket(in::DestroyEntitiesPacket* packet) {}  // 0x30
    virtual void HandlePacket(in::UnlockRecipesPacket* packet) {}
    virtual void HandlePacket(in::RemoveEntityEffectPacket* packet) {}   // 0x31
    virtual void HandlePacket(in::ResourcePackSendPacket* packet) {}     // 0x32
    virtual void HandlePacket(in::RespawnPacket* packet) {}              // 0x33
    virtual void HandlePacket(in::EntityHeadLookPacket* packet) {}       // 0x34
    virtual void HandlePacket(in::WorldBorderPacket* packet) {}          // 0x35
    virtual void HandlePacket(in::CameraPacket* packet) {}               // 0x36
    virtual void HandlePacket(in::HeldItemChangePacket* packet) {}       // 0x37
    virtual void HandlePacket(in::DisplayScoreboardPacket* packet) {}    // 0x38
    virtual void HandlePacket(in::EntityMetadataPacket* packet) {}       // 0x39
    virtual void HandlePacket(in::AttachEntityPacket* packet) {}         // 0x3A
    virtual void HandlePacket(in::EntityVelocityPacket* packet) {}       // 0x3B
    virtual void HandlePacket(in::EntityEquipmentPacket* packet) {}      // 0x3C
    virtual void HandlePacket(in::SetExperiencePacket* packet) {}        // 0x3D
    virtual void HandlePacket(in::UpdateHealthPacket* packet) {}         // 0x3E
    virtual void HandlePacket(in::ScoreboardObjectivePacket* packet) {}  // 0x3F
    virtual void HandlePacket(in::SetPassengersPacket* packet) {}        // 0x40
    virtual void HandlePacket(in::TeamsPacket* packet) {}                // 0x41
    virtual void HandlePacket(in::UpdateScorePacket* packet) {}          // 0x42
    virtual void HandlePacket(in::SpawnPositionPacket* packet) {}        // 0x43
    virtual void HandlePacket(in::TimeUpdatePacket* packet) {}           // 0x44
    virtual void HandlePacket(in::TitlePacket* packet) {}                // 0x45
    virtual void HandlePacket(in::SoundEffectPacket* packet) {}          // 0x46
    virtual void HandlePacket(in::PlayerListHeaderAndFooterPacket* packet) {
    }                                                                 // 0x47
    virtual void HandlePacket(in::CollectItemPacket* packet) {}       // 0x48
    virtual void HandlePacket(in::EntityTeleportPacket* packet) {}    // 0x49
    virtual void HandlePacket(in::EntityPropertiesPacket* packet) {}  // 0x4A
    virtual void HandlePacket(in::EntityEffectPacket* packet) {}      // 0x4B
    virtual void HandlePacket(in::AdvancementProgressPacket* packet) {}
    virtual void HandlePacket(in::CraftRecipeResponsePacket* packet) {}
};

}  // namespace packets
}  // namespace protocol
}  // namespace mc

#endif
