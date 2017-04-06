#ifndef MCLIB_PROTOCOL_PROTOCOL_H_
#define MCLIB_PROTOCOL_PROTOCOL_H_

#include <mclib/common/VarInt.h>
#include <string>

namespace mc {
namespace protocol {

enum class State {
    Handshake,
    Status,
    Login,
    Play
};

enum class Version {
    Minecraft_1_10_2 = 210,
    Minecraft_1_11_0 = 315,
    Minecraft_1_11_2 = 316,
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

namespace login {

enum ProtocolLogin {
    Disconnect,
    EncryptionRequest,
    LoginSuccess,
    SetCompression
};

} // ns login

namespace status {

enum ProtocolStatus {
    Response = 0,
    Pong
};

} // ns status

namespace play {

enum ProtocolPlay {
    SpawnObject,
    SpawnExperienceOrb,
    SpawnGlobalEntity,
    SpawnMob,
    SpawnPainting,
    SpawnPlayer,
    Animation,
    Statistics,
    BlockBreakAnimation,
    UpdateBlockEntity,
    BlockAction,
    BlockChange,
    BossBar,
    ServerDifficulty,
    TabComplete,
    Chat,
    MultiBlockChange,
    ConfirmTransaction,
    CloseWindow,
    OpenWindow,
    WindowItems,
    WindowProperty,
    SetSlot,
    SetCooldown,
    PluginMessage,
    NamedSoundEffect,
    Disconnect,
    EntityStatus,
    Explosion,
    UnloadChunk,
    ChangeGameState,
    KeepAlive,
    ChunkData,
    Effect,
    Particle,
    JoinGame,
    Map,
    EntityRelativeMove,
    EntityLookAndRelativeMove,
    EntityLook,
    Entity,
    VehicleMove,
    OpenSignEditor,
    PlayerAbilities,
    CombatEvent,
    PlayerListItem,
    PlayerPositionAndLook,
    UseBed,
    DestroyEntities,
    RemoveEntityEffect,
    ResourcePackSend,
    Respawn,
    EntityHeadLook,
    WorldBorder,
    Camera,
    HeldItemChange,
    DisplayScoreboard,
    EntityMetadata,
    AttachEntity,
    EntityVelocity,
    EntityEquipment,
    SetExperience,
    UpdateHealth,
    ScoreboardObjective,
    SetPassengers,
    Teams,
    UpdateScore,
    SpawnPosition,
    TimeUpdate,
    Title,
    SoundEffect,
    PlayerListHeaderAndFooter,
    CollectItem,
    EntityTeleport,
    EntityProperties,
    EntityEffect,
};

} // ns play

} // ns protocol
} // ns mc

#endif
