#ifndef MCLIB_PROTOCOL_PROTOCOLSTATE_H_
#define MCLIB_PROTOCOL_PROTOCOLSTATE_H_

#include <mclib/common/VarInt.h>
#include <string>

namespace mc {
namespace protocol {

enum class State { Handshake, Status, Login, Play };

enum class Version {
    Minecraft_1_10_2 = 210,
    Minecraft_1_11_0 = 315,
    Minecraft_1_11_2 = 316,
    Minecraft_1_12_0 = 335,
    Minecraft_1_12_1 = 338,
    Minecraft_1_12_2 = 340,
    Minecraft_1_13_2 = 404,
    Minecraft_1_14_2 = 485,
};

MCLIB_API std::string to_string(Version version);

class UnfinishedProtocolException {
private:
    VarInt m_PacketId;
    State m_ProtocolState;

public:
    UnfinishedProtocolException(VarInt id, State state)
        : m_PacketId(id), m_ProtocolState(state) {}
    std::string what() const {
        return "Unknown packet type " + std::to_string(m_PacketId.GetInt()) +
               " received during " + std::to_string((s32)m_ProtocolState) +
               " protocol state.";
    }
};

namespace login {

enum ProtocolLogin {
    Disconnect,
    EncryptionRequest,
    LoginSuccess,
    SetCompression
};

}  // namespace login

namespace status {

enum ProtocolStatus { Response = 0, Pong };

}  // namespace status

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
    AdvancementProgress = 0x4E,
    Advancements,
    UnlockRecipes,
    CraftRecipeResponse,
    OpenHorseWindow,
    UpdateLight,
    TradeList,
    OpenBook,
    PlayerInfo,
    FacePlayer,
    SelectAdvancementTab,
    UpdateViewPosition,
    UpdateViewDistance,
    EntitySoundEffect,
    StopSound,
    NBTQueryResponse,
    DeclareRecipes,
    Tags,
};

}  // namespace play

}  // namespace protocol
}  // namespace mc

namespace std {

template <>
struct hash<mc::protocol::State> {
    std::size_t operator()(const mc::protocol::State& state) const {
        return std::hash<int>()(static_cast<int>(state));
    }
};

template <>
struct hash<mc::protocol::Version> {
    std::size_t operator()(const mc::protocol::Version& version) const {
        return std::hash<int>()(static_cast<int>(version));
    }
};

}  // namespace std

#endif
