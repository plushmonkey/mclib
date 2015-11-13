#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "VarInt.h"
#include <string>
namespace Minecraft {
namespace Protocol {

enum class State {
    Handshake,
    Status,
    Login,
    Play
};

class UnfinishedProtocolException {
private:
    VarInt m_PacketId;
    State m_ProtocolState;

public:
    UnfinishedProtocolException(VarInt id, State state) : m_PacketId(id), m_ProtocolState(state) { }
    std::string what() const {
        return "Unknown packet type " + std::to_string(m_PacketId.GetInt()) + " received during " + std::to_string((s32)m_ProtocolState) + " protocol state.";
    }
};

namespace Login {

enum ProtocolLogin {
    Disconnect,
    EncryptionRequest,
    LoginSuccess,
    SetCompression
};

} // ns Login

namespace Play {

enum ProtocolPlay {
    KeepAlive,
    JoinGame,
    Chat,
    TimeUpdate,
    EntityEquipment,
    SpawnPosition,
    UpdateHealth,
    Respawn,
    PlayerPositionAndLook,
    HeldItemChange,
    UseBed,
    Animation,
    SpawnPlayer,
    CollectItem,
    SpawnObject,
    SpawnMob,
    SpawnPainting,
    SpawnExperienceOrb,
    EntityVelocity,
    DestroyEntities,
    Entity,
    EntityRelativeMove,
    EntityLook,
    EntityLookAndRelativeMove,
    EntityTeleport,
    EntityHeadLook,
    EntityStatus,
    AttachEntity,
    EntityMetadata,
    EntityEffect,
    RemoveEntityEffect,
    SetExperience,
    EntityProperties,
    ChunkData,
    MultiBlockChange,
    BlockChange,
    BlockAction,
    BlockBreakAnimation,
    MapChunkBulk,
    Explosion,
    Effect,
    SoundEffect,
    Particle,
    ChangeGameState,
    SpawnGlobalEntity,
    OpenWindow,
    CloseWindow,
    SetSlot,
    WindowItems,
    WindowProperty,
    ConfirmTransaction,
    UpdateSign,
    Map,
    UpdateBlockEntity,
    OpenSignEditor,
    Statistics,
    PlayerListItem,
    PlayerAbilities,
    TabComplete,
    ScoreboardObjective,
    UpdateScore,
    DisplayScoreboard,
    Teams,
    PluginMessage,
    Disconnect,
    ServerDifficulty,
    CombatEvent,
    Camera,
    WorldBorder,
    Title,
    SetCompression,
    PlayerListHeadAndFooter,
    ResourcePackSend,
    UpdateEntityNBT
};

} // ns Play
} // ns Protocol
} // ns Minecraft

#endif
