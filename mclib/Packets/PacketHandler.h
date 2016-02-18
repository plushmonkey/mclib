#ifndef PACKETS_PACKET_HANDLER_H_
#define PACKETS_PACKET_HANDLER_H_

#include "Packet.h"

namespace Minecraft {
namespace Packets {

class PacketDispatcher;

/**
 * Used to listen for specific incoming packets. 
 * The order in which these packets are handled isn't set, so care must be taken when listening. 
 * The internal systems might not be updated when the handler is called.
 * This is mostly for internal classes, other classes should listen to the other systems instead, 
 * like EntityManager/PlayerManager, to ensure that all the internal data is updated before being handled.
 */
class PacketHandler {
private:
    PacketDispatcher* m_Dispatcher;

public:
    MCLIB_API PacketHandler(PacketDispatcher* dispatcher);
    virtual MCLIB_API ~PacketHandler();

    MCLIB_API PacketDispatcher* GetDispatcher();

    // Login protocol state
    virtual void HandlePacket(Inbound::DisconnectPacket* packet) { }
    virtual void HandlePacket(Inbound::EncryptionRequestPacket* packet) { }
    virtual void HandlePacket(Inbound::LoginSuccessPacket* packet) { }
    virtual void HandlePacket(Inbound::SetCompressionPacket* packet) { }

    // Play protocol state
    virtual void HandlePacket(Inbound::KeepAlivePacket* packet) { } // 0x00
    virtual void HandlePacket(Inbound::JoinGamePacket* packet) { } // 0x01
    virtual void HandlePacket(Inbound::ChatPacket* packet) { } // 0x02
    virtual void HandlePacket(Inbound::TimeUpdatePacket* packet) { } // 0x03
    virtual void HandlePacket(Inbound::EntityEquipmentPacket* packet) { } // 0x04
    virtual void HandlePacket(Inbound::SpawnPositionPacket* packet) { } // 0x05
    virtual void HandlePacket(Inbound::UpdateHealthPacket* packet) { } // 0x06
    virtual void HandlePacket(Inbound::RespawnPacket* packet) { } // 0x07
    virtual void HandlePacket(Inbound::PlayerPositionAndLookPacket* packet) { } // 0x08
    virtual void HandlePacket(Inbound::HeldItemChangePacket* packet) { } // 0x09
    virtual void HandlePacket(Inbound::UseBedPacket* packet) { } // 0x0A
    virtual void HandlePacket(Inbound::AnimationPacket* packet) { } // 0x0B
    virtual void HandlePacket(Inbound::SpawnPlayerPacket* packet) { } // 0x0C
    virtual void HandlePacket(Inbound::CollectItemPacket* packet) { } // 0x0D
    virtual void HandlePacket(Inbound::SpawnObjectPacket* packet) { } // 0x0E
    virtual void HandlePacket(Inbound::SpawnMobPacket* packet) { } // 0x0F
    virtual void HandlePacket(Inbound::SpawnPaintingPacket* packet) { } // 0x10
    virtual void HandlePacket(Inbound::SpawnExperienceOrbPacket* packet) { } // 0x11
    virtual void HandlePacket(Inbound::EntityVelocityPacket* packet) { } // 0x12
    virtual void HandlePacket(Inbound::DestroyEntitiesPacket* packet) { } // 0x13
    virtual void HandlePacket(Inbound::EntityPacket* packet) { } // 0x14
    virtual void HandlePacket(Inbound::EntityRelativeMovePacket* packet) { } // 0x15
    virtual void HandlePacket(Inbound::EntityLookPacket* packet) { } // 0x16
    virtual void HandlePacket(Inbound::EntityLookAndRelativeMovePacket* packet) { } // 0x17
    virtual void HandlePacket(Inbound::EntityTeleportPacket* packet) { } // 0x18
    virtual void HandlePacket(Inbound::EntityHeadLookPacket* packet) { } // 0x19
    virtual void HandlePacket(Inbound::EntityStatusPacket* packet) { } // 0x1A
    virtual void HandlePacket(Inbound::AttachEntityPacket* packet) { } // 0x1B
    virtual void HandlePacket(Inbound::EntityMetadataPacket* packet) { } // 0x1C
    virtual void HandlePacket(Inbound::EntityEffectPacket* packet) { } // 0x1D
    virtual void HandlePacket(Inbound::RemoveEntityEffectPacket* packet) { } // 0x1E
    virtual void HandlePacket(Inbound::SetExperiencePacket* packet) { } // 0x1F
    virtual void HandlePacket(Inbound::EntityPropertiesPacket* packet) { } // 0x20
    virtual void HandlePacket(Inbound::ChunkDataPacket* packet) { } // 0x21
    virtual void HandlePacket(Inbound::MultiBlockChangePacket* packet) { } // 0x22
    virtual void HandlePacket(Inbound::BlockChangePacket* packet) { } // 0x23
    virtual void HandlePacket(Inbound::BlockActionPacket* packet) { } // 0x24
    virtual void HandlePacket(Inbound::BlockBreakAnimationPacket* packet) { } // 0x25
    virtual void HandlePacket(Inbound::MapChunkBulkPacket* packet) { } // 0x26
    virtual void HandlePacket(Inbound::ExplosionPacket* packet) { } // 0x27
    virtual void HandlePacket(Inbound::EffectPacket* packet) { } // 0x28
    virtual void HandlePacket(Inbound::SoundEffectPacket* packet) { } // 0x29
    virtual void HandlePacket(Inbound::ParticlePacket* packet) { } // 0x2A
    virtual void HandlePacket(Inbound::ChangeGameStatePacket* packet) { } // 0x2B
    virtual void HandlePacket(Inbound::SpawnGlobalEntityPacket* packet) { } // 0x2C
    virtual void HandlePacket(Inbound::SetSlotPacket* packet) { } // 0x2F
    virtual void HandlePacket(Inbound::WindowItemsPacket* packet) { } // 0x30
    virtual void HandlePacket(Inbound::UpdateBlockEntityPacket* packet) { } // 0x35
    virtual void HandlePacket(Inbound::StatisticsPacket* packet) { } // 0x37
    virtual void HandlePacket(Inbound::PlayerListItemPacket* packet) { } // 0x38
    virtual void HandlePacket(Inbound::PlayerAbilitiesPacket* packet) { } // 0x39
    virtual void HandlePacket(Inbound::PluginMessagePacket* packet) { } // 0x3F
    virtual void HandlePacket(Inbound::ServerDifficultyPacket* packet) { } // 0x41
    virtual void HandlePacket(Inbound::CombatEventPacket* packet) { } // 0x42
    virtual void HandlePacket(Inbound::WorldBorderPacket* packet) { } // 0x44
};

} // ns Packets
} // ns Minecraft

#endif
