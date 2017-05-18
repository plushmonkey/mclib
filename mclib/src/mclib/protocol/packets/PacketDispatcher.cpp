#include <mclib/protocol/packets/PacketDispatcher.h>

#include <mclib/protocol/packets/PacketHandler.h>

#include <algorithm>
#include <unordered_map>

namespace mc {
namespace protocol {
namespace packets {

std::unordered_map<s32, s32> map_1_12_0 = {
    { 0x00,  play::SpawnObject },
    { 0x01,  play::SpawnExperienceOrb },
    { 0x02,  play::SpawnGlobalEntity },
    { 0x03,  play::SpawnMob },
    { 0x04,  play::SpawnPainting },
    { 0x05,  play::SpawnPlayer },
    { 0x06,  play::Animation },
    { 0x07,  play::Statistics },
    { 0x08,  play::Advancements },
    { 0x09,  play::BlockBreakAnimation },
    { 0x0A,  play::UpdateBlockEntity },
    { 0x0B,  play::BlockAction },
    { 0x0C,  play::BlockChange},
    { 0x0D,  play::BossBar },
    { 0x0E,  play::ServerDifficulty},
    { 0x0F,  play::TabComplete },
    { 0x10,  play::Chat },
    { 0x11,  play::MultiBlockChange },
    { 0x12,  play::ConfirmTransaction },
    { 0x13,  play::CloseWindow },
    { 0x14,  play::OpenWindow },
    { 0x15,  play::WindowItems },
    { 0x16,  play::WindowProperty },
    { 0x17,  play::SetSlot },
    { 0x18,  play::SetCooldown },
    { 0x19,  play::PluginMessage },
    { 0x1A,  play::NamedSoundEffect },
    { 0x1B,  play::Disconnect },
    { 0x1C,  play::EntityStatus },
    { 0x1D,  play::Explosion },
    { 0x1E,  play::UnloadChunk },
    { 0x1F,  play::ChangeGameState },
    { 0x20,  play::KeepAlive },
    { 0x21,  play::ChunkData },
    { 0x22,  play::Effect },
    { 0x23,  play::Particle },
    { 0x24,  play::JoinGame },
    { 0x25,  play::Map },
    { 0x26,  play::EntityRelativeMove },
    { 0x27,  play::EntityLookAndRelativeMove },
    { 0x28,  play::EntityLook },
    { 0x29,  play::Entity },
    { 0x2A,  play::VehicleMove },
    { 0x2B,  play::OpenSignEditor },
    { 0x2C,  play::PlayerAbilities },
    { 0x2D,  play::CombatEvent },
    { 0x2E,  play::PlayerListItem },
    { 0x2F,  play::PlayerPositionAndLook },
    { 0x30,  play::UseBed },
    { 0x31,  play::UnlockRecipes },
    { 0x32,  play::DestroyEntities },
    { 0x33,  play::RemoveEntityEffect },
    { 0x34,  play::ResourcePackSend },
    { 0x35,  play::Respawn },
    { 0x36,  play::EntityHeadLook },
    { 0x37,  play::WorldBorder },
    { 0x38,  play::Camera },
    { 0x39,  play::HeldItemChange },
    { 0x3A,  play::DisplayScoreboard },
    { 0x3B,  play::EntityMetadata },
    { 0x3C,  play::AttachEntity },
    { 0x3D,  play::EntityVelocity },
    { 0x3E,  play::EntityEquipment },
    { 0x3F,  play::SetExperience },
    { 0x40,  play::UpdateHealth },
    { 0x41,  play::ScoreboardObjective },
    { 0x42,  play::SetPassengers },
    { 0x43,  play::Teams },
    { 0x44,  play::UpdateScore },
    { 0x45,  play::SpawnPosition },
    { 0x46,  play::TimeUpdate },
    { 0x47,  play::Title },
    { 0x48,  play::SoundEffect },
    { 0x49,  play::PlayerListHeaderAndFooter },
    { 0x4A,  play::CollectItem },
    { 0x4B,  play::EntityTeleport },
    { 0x4C,  play::EntityProperties },
    { 0x4D,  play::EntityEffect },
    { 0x4E,  play::AdvancementProgress },
};

s64 GetDispatcherId(Packet* packet) {
    auto version = packet->GetProtocolVersion();

    if (version <= Version::Minecraft_1_11_2)
        return packet->GetId().GetLong();

    if (version == Version::Minecraft_1_12_0) {
        s32 id = packet->GetId().GetInt();

        auto iter = map_1_12_0.find(id);
        if (iter != map_1_12_0.end())
            return iter->second;
    }

    throw std::runtime_error("Unknown packet type received");
}

void PacketDispatcher::RegisterHandler(protocol::State protocolState, PacketId id, PacketHandler* handler) {
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found == m_Handlers[type].end())
        m_Handlers[type].push_back(handler);
}

void PacketDispatcher::UnregisterHandler(protocol::State protocolState, PacketId id, PacketHandler* handler) {
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found != m_Handlers[type].end())
        m_Handlers[type].erase(found);
}

void PacketDispatcher::UnregisterHandler(PacketHandler* handler) {
    for (auto& pair : m_Handlers) {
        if (pair.second.empty()) continue;

        auto state = pair.first.first;
        auto id = pair.first.second;
        PacketType type(state, id);

        m_Handlers[type].erase(std::remove(m_Handlers[type].begin(), m_Handlers[type].end(), handler), m_Handlers[type].end());
    }
}

void PacketDispatcher::Dispatch(Packet* packet) {
    if (!packet) return;

    s64 id = GetDispatcherId(packet);
    
    PacketType type(packet->GetProtocolState(), id);
    for (PacketHandler* handler : m_Handlers[type])
        packet->Dispatch(handler);
}

} // ns packets
} // ns protocol
} // ns mc
