#include <mclib/util/Utility.h>

#include <mclib/block/Block.h>
#include <mclib/common/DataBuffer.h>
#include <mclib/core/Connection.h>
#include <mclib/core/PlayerManager.h>
#include <mclib/entity/EntityManager.h>
#include <mclib/protocol/Protocol.h>
#include <mclib/world/World.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <array>
#include <vector>
#include <json/json.h>
#include <memory>
#include <chrono>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <regex>
#include <cmath>
#include <future>
#include <utf8.h>

#undef max
#undef min

#ifdef _WIN32
#include <ShlObj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif


namespace mc {
namespace util {

enum DyeColor {
    InkSac = 0x1E1B1B,
    RoseRed = 0xB3312C,
    CactusGreen = 0x3B511A,
    CocoaBeans = 0x51301A,
    LapisLazuli = 0x253192,
    Purple = 0x7B2FBE,
    Cyan = 0x287697,
    LightGray = 0xABABAB,
    Gray = 0x434343,
    Pink = 0xD88198,
    Lime = 0x41CD34,
    DandelionYellow = 0xDECF2A,
    LightBlue = 0x6689D3,
    Magenta = 0xC354CD,
    Orange = 0xEB8844,
    BoneMeal = 0xF0F0F0,
};

std::wstring to_wstring(const char* cstr) {
    std::string str(cstr);
    return std::wstring(str.begin(), str.end());
}
#ifdef _WIN32

std::wstring GetAppDataPath() {
    WCHAR* appdata = nullptr;

    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appdata) != S_OK)
        return false;

    std::wstring result(appdata);

    CoTaskMemFree(appdata);

    return result;
}

#else

std::wstring GetAppDataPath() {
    const char* home = nullptr;

    if ((home = getenv("HOME")) == nullptr)
        home = getpwuid(getuid())->pw_dir;

    return to_wstring(home);
}

#endif

bool GetProfileToken(const std::string& username, core::AuthToken* token) {
    std::wstring appData = GetAppDataPath();

    const std::wstring filename = appData + L"/.minecraft/launcher_profiles.json";

    std::ifstream in(std::string(filename.begin(), filename.end()));

    if (!in.is_open())
        return false;

    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(in, root))
        return false;

    if (root["clientToken"].isNull() || root["authenticationDatabase"].isNull())
        return false;

    auto& authDB = root["authenticationDatabase"];

    std::string accessToken;
    std::string clientToken;

    clientToken = root["clientToken"].asString();

    auto dbMembers = authDB.getMemberNames();
    for (auto& member : dbMembers) {
        auto& account = authDB[member];

        if (account["accessToken"].isNull())
            continue;

        accessToken = account["accessToken"].asString();

        // Check if it's using Linux format
        if (account["profiles"].isNull()) {
            if (account["displayName"].isNull())
                continue;

            if (account["uuid"].isNull())
                continue;

            if (account["displayName"].asString() != username)
                continue;

            *token = mc::core::AuthToken(accessToken, clientToken, member);
            return true;
        } else {
            auto& profiles = account["profiles"];
            std::vector<std::string> profileMembers = profiles.getMemberNames();
            for (const std::string& profileId : profileMembers) {
                auto& profile = profiles[profileId];

                if (profile["displayName"].isNull())
                    continue;

                if (profile["displayName"].asString() != username)
                    continue;

                *token = mc::core::AuthToken(accessToken, clientToken, profileId);
                return true;
            }
        }
    }

    return false;
}

s64 GetTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

inventory::Slot CreateFirework(bool flicker, bool trail, u8 type, u8 duration, std::vector<int> colors, const std::string& name = "") {
    using namespace nbt;

    NBT nbt;

    TagCompound* fireworks = new TagCompound(L"Fireworks");
    TagPtr flightTag(new TagByte("Flight", duration));

    fireworks->AddItem(TagType::Byte, flightTag);

    TagList* explosions = new TagList("Explosions", TagType::Compound);

    TagCompound* explosion = new TagCompound(L"Explosion");
    TagPtr flickerTag(new TagByte("Flicker", flicker ? 1 : 0));
    TagPtr trailTag(new TagByte("Trail", trail ? 1 : 0));
    TagPtr typeTag(new TagByte("Type", type));

    TagPtr colorsTag(new TagIntArray("Colors", colors));
    //TagPtr fadeColorsTag(new TagIntArray("FadeColors", colors));

    explosion->AddItem(TagType::Byte, flickerTag);
    explosion->AddItem(TagType::Byte, trailTag);
    explosion->AddItem(TagType::Byte, typeTag);
    explosion->AddItem(TagType::IntArray, colorsTag);

    explosions->AddItem(TagPtr(explosion));
    fireworks->AddItem(TagType::Compound, TagPtr(explosions));
    nbt.GetRoot().AddItem(TagType::Compound, TagPtr(fireworks));
    nbt.GetRoot().SetName(L"tag");

    if (!name.empty()) {
        TagCompound* display = new TagCompound(L"display");
        TagPtr nameTag(new TagString("Name", name));

        display->AddItem(TagType::String, nameTag);

        nbt.GetRoot().AddItem(TagType::Compound, TagPtr(display));
    }

    inventory::Slot slot(401, 64, 0, nbt);

    return slot;
}

std::string ParseChatNode(Json::Value node) {
    if (node.isObject()) {
        auto&& translateNode = node["translate"];

        if (translateNode.isString()) {
            std::string translate = translateNode.asString();
            std::string message;

            if (translate == "chat.type.text") {
                auto&& withNode = node["with"];

                if (withNode.isArray()) {
                    for (auto iter = withNode.begin(); iter != withNode.end(); ++iter) {
                        auto&& node = *iter;

                        if (node.isObject()) {
                            auto&& textNode = node["text"];
                            if (textNode.isString())
                                message += "<" + textNode.asString() + "> ";
                        } else if (node.isString()) {
                            message += node.asString();
                        }
                    }
                }
            }

            return message;
        }
    }

    if (node.isNull()) return "";
    if (node.isString()) return node.asString();
    if (node.isObject()) {
        std::string result;

        if (!node["extra"].isNull())
            result += ParseChatNode(node["extra"]);
        if (node["text"].isString())
            result += node["text"].asString();
        return result;
    }
    if (node.isArray()) {
        std::string result;

        for (auto arrayNode : node)
            result += ParseChatNode(arrayNode);
        return result;
    }
    return "";
}

static Console console;

class StandardConsole : public IConsole {
public:
    void Output(const std::string& str) {
        std::wcout << std::wstring(str.begin(), str.end());
    }

    void Output(const std::wstring& str) {
        std::wcout << str;
    }

    IConsole& operator<<(const std::string& str) {
        Output(str);
        return *this;
    }
    IConsole& operator<<(const std::wstring& str) {
        Output(str);
        return *this;
    }
};

PlayerController::PlayerController(core::Connection* connection, world::World& world, core::PlayerManager& playerManager)
    : m_PlayerManager(playerManager),
      m_Connection(connection),
      m_World(world),
      m_Position(0, 0, 0),
      m_BoundingBox(Vector3d(-0.3, 0, -0.3), Vector3d(0.3, 1.8, 0.3)),
      m_EntityId(-1),
      m_LastUpdate(GetTime()),
      m_Sprinting(false),
      m_LoadedIn(false),
      m_MoveSpeed(4.3),
      m_HandleFall(true)
{
    m_PlayerManager.RegisterListener(this);

    //console.SetImpl(new LoggerConsole("PlayerController.log"));
    //console.SetImpl(new StandardConsole());
}

PlayerController::~PlayerController() {
    m_PlayerManager.UnregisterListener(this);
}

AABB PlayerController::GetBoundingBox() const {
    return m_BoundingBox + m_Position;
}

bool PlayerController::ClearPath(Vector3d target) {
    return true;
    Vector3d position = m_Position;

    double dist = target.Distance(position);

    Vector3d toTarget = target - position;
    Vector3d n = Vector3Normalize(toTarget);

    Vector3d side = n.Cross(Vector3d(0, 1, 0));

    const double CheckWidth = 0.3;

    auto check = [&](Vector3d start, Vector3d delta) {
        Vector3d checkAbove = start + delta + Vector3d(0, 1, 0);
        Vector3d checkBelow = start + delta + Vector3d(0, 0, 0);
        block::BlockPtr aboveBlock = m_World.GetBlock(checkAbove).GetBlock();
        block::BlockPtr belowBlock = m_World.GetBlock(checkBelow).GetBlock();

        if (aboveBlock && aboveBlock->IsSolid())
            return false;

        if (belowBlock && belowBlock->IsSolid()) {
            block::BlockPtr twoAboveBlock = m_World.GetBlock(checkAbove + Vector3d(0, 1, 0)).GetBlock();
            // Bad path if there isn't a two high gap in it
            if (twoAboveBlock && twoAboveBlock->IsSolid())
                return false;

            // Jump up 1 block to keep searching
            position += Vector3d(0, 1, 0);
        }
        return true;
    };

    for (s32 i = 0; i < (int)std::ceil(dist); ++i) {
        Vector3d delta(i * n.x, 0, i * n.z);

        // Check right side
        if (!check(position + side * CheckWidth, delta)) return false;
        if (!check(position - side * CheckWidth, delta)) return false;

        Vector3d checkFloor = position + delta + Vector3d(0, -1, 0);
        block::BlockPtr floorBlock = m_World.GetBlock(checkFloor).GetBlock();
        if (floorBlock && !floorBlock->IsSolid()) {
            block::BlockPtr belowFloorBlock = m_World.GetBlock(checkFloor - Vector3d(0, 1, 0)).GetBlock();

            // Fail if there is a two block drop
            if (belowFloorBlock && !belowFloorBlock->IsSolid())
                return false;

            position.y--;
        }
    }

    return true;
}

void PlayerController::SetMoveSpeed(double speed) { m_MoveSpeed = speed; }

void PlayerController::OnClientSpawn(core::PlayerPtr player) {
    m_Yaw = player->GetEntity()->GetYaw();
    m_Pitch = player->GetEntity()->GetPitch();
    m_Position = player->GetEntity()->GetPosition();
    m_LoadedIn = true;
    m_TargetPos = m_Position;
    auto entity = player->GetEntity();
    if (entity) {
        EntityId eid = entity->GetEntityId();

        m_EntityId = eid;
    }
}

void PlayerController::Dig(Vector3d target) {
    Vector3d toTarget = target - m_Position;

    if (toTarget.Length() > 6) return;

    m_DigQueue.push(target);
}

void PlayerController::Attack(EntityId id) {
    static u64 timer = 0;
    static const u64 cooldown = 500;

    if (GetTime() - timer < cooldown)
        return;

    using namespace protocol::packets::out;

    UseEntityPacket useEntity(id, UseEntityPacket::Action::Attack);
    m_Connection->SendPacket(&useEntity);

    timer = GetTime();
}

void PlayerController::UpdateDigging() {
    if (m_DigQueue.empty()) return;

    //Vector3d target = m_DigQueue.front();

}

std::vector<block::BlockState> PlayerController::GetNearbyBlocks(const s32 radius) {
    std::vector<block::BlockState> nearbyBlocks;

    for (s32 x = -radius; x < radius; ++x) {
        for (s32 y = -radius; y < radius; ++y) {
            for (s32 z = -radius; z < radius; ++z) {
                Vector3d checkPos = m_Position + Vector3d(x, y, z);

                auto state = m_World.GetBlock(checkPos);

                if (state.GetBlock() && state.GetBlock()->IsSolid())
                    nearbyBlocks.push_back(state);
            }
        }
    }

    std::sort(nearbyBlocks.begin(), nearbyBlocks.end(), [](const block::BlockState& a, const block::BlockState& b) {
        if (a.GetBlock() == b.GetBlock()) return a.GetData() < b.GetData();
        return a.GetBlock() < b.GetBlock();
    });

    nearbyBlocks.erase(std::unique(nearbyBlocks.begin(), nearbyBlocks.end()), nearbyBlocks.end());

    return nearbyBlocks;
}

bool PlayerController::HandleJump() {
    AABB playerBounds = m_BoundingBox + m_Position;

    for (const auto& state : GetNearbyBlocks(2)) {
        block::BlockPtr checkBlock = state.GetBlock();

        auto result = checkBlock->CollidesWith(state, state.GetPosition(), playerBounds);
        if (result.first) {
            m_Position.y = result.second.max.y;
            return true;
        }
    }

    return false;
}

bool PlayerController::HandleFall() {
    double bestDist = FallSpeed;
    block::BlockPtr bestBlock = nullptr;

    if (!InLoadedChunk())
        return false;

    AABB playerBounds = m_BoundingBox + (m_Position - Vector3d(0, FallSpeed, 0));

    for (const auto& state : GetNearbyBlocks(2)) {
        auto checkBlock = state.GetBlock();        

        auto boundingBoxes = checkBlock->GetBoundingBoxes(state);
        for (auto blockBounds : boundingBoxes) {
            AABB checkBounds = blockBounds + state.GetPosition();

            if (checkBounds.Intersects(playerBounds)) {
                double penetration = checkBounds.max.y - playerBounds.min.y;
                double dist = FallSpeed - penetration;

                if (dist < bestDist)
                    bestDist = dist;
            }
        }
    }

    m_Position -= Vector3d(0.0, bestDist, 0.0);

    return bestDist == FallSpeed;
}

void PlayerController::SetTargetPosition(Vector3d target) {
    m_TargetPos = target;
}

void PlayerController::SetHandleFall(bool handle) {
    m_HandleFall = handle;
}

void PlayerController::UpdatePosition() {
    //static const double WalkingSpeed = 4.3; // m/s
    //static const double WalkingSpeed = 8.6; // m/s
    static const double TicksPerSecond = 20;

    u64 dt = GetTime() - m_LastUpdate;

    if (dt < 1000 / TicksPerSecond)
        return;

    m_LastUpdate = GetTime();

    static int counter = 0;

    if (++counter == 30) {
        console << "Current pos: " << GetPosition() << "\n";
        counter = 0;
    }

    Vector3d target = m_TargetPos;
    Vector3d toTarget = target - GetPosition();
    toTarget.y = 0;
    double dist = toTarget.Length();

    if (!ClearPath(target))
        return;

    Vector3d n = Vector3Normalize(toTarget);
    double change = dt / 1000.0;

    n *= m_MoveSpeed * change;

    if (n.Length() > dist)
        n = Vector3Normalize(n) * dist;

    Move(n);
}

void PlayerController::Update() {
    if (!m_LoadedIn) return;

    UpdatePosition();

    bool onGround = true;

    if (HandleJump()) {
        console << "Jumping\n";
    } else {
        if (!m_HandleFall) {
            const float FullCircle = 2.0f * 3.14159f;
            const float CheckWidth = 0.3f;
            onGround = false;
            for (float angle = 0.0f; angle < FullCircle; angle += FullCircle / 8) {
                Vector3d checkPos = m_Position + Vector3RotateAboutY(Vector3d(0, 0, CheckWidth), angle) - Vector3d(0, 1, 0);

                block::BlockPtr checkBlock = m_World.GetBlock(checkPos).GetBlock();
                if (checkBlock && checkBlock->IsSolid()) {
                    onGround = true;
                    break;
                }
            }
        } else if (HandleFall()) {
            console << "Falling\n";
            onGround = false;
        }
            
    }

    protocol::packets::out::PlayerPositionAndLookPacket response(m_Position,
        m_Yaw * 180.0f / 3.14159f, m_Pitch * 180.0f / 3.14159f, onGround);

    m_Connection->SendPacket(&response);

    UpdateDigging();
}

bool PlayerController::InLoadedChunk() const {
    return m_World.GetChunk(ToVector3i(m_Position)) != nullptr;
}

Vector3d PlayerController::GetPosition() const { return m_Position; }
Vector3d PlayerController::GetHeading() const {
    return Vector3d(
        -std::cos(GetPitch()) * std::sin(GetYaw()),
        -std::sin(GetPitch()),
        std::cos(GetPitch()) * std::cos(GetYaw())
    );
}
float PlayerController::GetYaw() const { return m_Yaw; }
float PlayerController::GetPitch() const { return m_Pitch; }

void PlayerController::Move(Vector3d delta) {
    //delta.y = 0;

    if (m_EntityId != -1) {
        core::PlayerPtr player = m_PlayerManager.GetPlayerByEntityId(m_EntityId);
        if (player) {
            entity::EntityPtr entity = player->GetEntity();

            if (entity) {
                if (entity->GetVehicleId() != -1)
                    return; // Don't move if player is in a vehicle
            }
        }
    }

    m_Position += delta;
}

void PlayerController::SetYaw(float yaw) { m_Yaw = yaw; }
void PlayerController::SetPitch(float pitch) { m_Pitch = pitch; }
void PlayerController::LookAt(Vector3d target) {
    Vector3d toTarget = target - m_Position;

    double dist = std::sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z);
    double pitch = -std::atan2(toTarget.y, dist);
    double yaw = -std::atan2(toTarget.x, toTarget.z);

    SetYaw((float)yaw);
    SetPitch((float)pitch);
}

class EventLogger : public core::PlayerListener, public core::ConnectionListener, public protocol::packets::PacketHandler {
private:
    core::PlayerManager* m_PlayerManager;
    core::Connection* m_Connection;

public:
    EventLogger(protocol::packets::PacketDispatcher* dispatcher, core::PlayerManager* playerManager, core::Connection* connection)
        : protocol::packets::PacketHandler(dispatcher),
          m_PlayerManager(playerManager),
          m_Connection(connection)
    {
        m_PlayerManager->RegisterListener(this);
        m_Connection->RegisterListener(this);

        using namespace protocol;

        dispatcher->RegisterHandler(State::Login, login::Disconnect, this);
        dispatcher->RegisterHandler(State::Login, login::EncryptionRequest, this);
        dispatcher->RegisterHandler(State::Login, login::LoginSuccess, this);

        dispatcher->RegisterHandler(State::Play, play::KeepAlive, this);
        dispatcher->RegisterHandler(State::Play, play::JoinGame, this);
        dispatcher->RegisterHandler(State::Play, play::Chat, this);
        dispatcher->RegisterHandler(State::Play, play::SpawnPosition, this);
        dispatcher->RegisterHandler(State::Play, play::UpdateHealth, this);
        dispatcher->RegisterHandler(State::Play, play::PlayerPositionAndLook, this);
        dispatcher->RegisterHandler(State::Play, play::SpawnPlayer, this);
        dispatcher->RegisterHandler(State::Play, play::SpawnMob, this);
        dispatcher->RegisterHandler(State::Play, play::EntityRelativeMove, this);
        dispatcher->RegisterHandler(State::Play, play::EntityMetadata, this);
        dispatcher->RegisterHandler(State::Play, play::SetExperience, this);
        dispatcher->RegisterHandler(State::Play, play::EntityProperties, this);
        dispatcher->RegisterHandler(State::Play, play::ChunkData, this);
        dispatcher->RegisterHandler(State::Play, play::SetSlot, this);
        dispatcher->RegisterHandler(State::Play, play::WindowItems, this);
        dispatcher->RegisterHandler(State::Play, play::Statistics, this);
        dispatcher->RegisterHandler(State::Play, play::PlayerAbilities, this);
        dispatcher->RegisterHandler(State::Play, play::PluginMessage, this);
        dispatcher->RegisterHandler(State::Play, play::Disconnect, this);
        dispatcher->RegisterHandler(State::Play, play::ServerDifficulty, this);
        dispatcher->RegisterHandler(State::Play, play::WorldBorder, this);
    }

    ~EventLogger() {
        m_PlayerManager->UnregisterListener(this);
        m_Connection->UnregisterListener(this);
    }

    void HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet) {
        
    }

    void HandlePacket(protocol::packets::in::ChunkDataPacket* packet) {
        /*auto column = packet->GetChunkColumn();
        auto x = column->GetMetadata().x;
        auto z = column->GetMetadata().z;

        std::wcout << "Received chunk data for chunk " << x << ", " << z << std::endl;*/
    }

    void HandlePacket(protocol::packets::in::EntityPropertiesPacket* packet) {
        /*  std::wcout << "Received entity properties: " << std::endl;
        const auto& properties = packet->GetProperties();
        for (const auto& kv : properties) {
        std::wstring key = kv.first;
        const auto& property = kv.second;
        std::wcout << key << " : " << property.value << std::endl;
        for (const auto& modifier : property.modifiers)
        std::wcout << "Modifier: " << modifier.uuid << " " << modifier.amount << " " << (int)modifier.operation << std::endl;
        }*/
    }

    void HandlePacket(protocol::packets::in::EntityRelativeMovePacket* packet) {

    }

    void HandlePacket(protocol::packets::in::SpawnPlayerPacket* packet) {
        /*float x, y, z;
        x = packet->GetX();
        y = packet->GetY();
        z = packet->GetZ();
        m_Out << "Spawn player " << packet->GetUUID() << " at (" << x << ", " << y << ", " << z << ")" << std::endl;*/
    }

    void HandlePacket(protocol::packets::in::UpdateHealthPacket* packet) {
        console << "Set health. health: " << packet->GetHealth() << "\n";
    }

    void HandlePacket(protocol::packets::in::SetExperiencePacket* packet) {
        console << "Set experience. Level: " << packet->GetLevel() << "\n";
    }

    std::string ParseChatNode(Json::Value node) {
        if (node.isNull()) return "";
        if (node.isString()) return node.asString();
        if (node.isObject()) {
            std::string result;

            if (!node["extra"].isNull())
                result += ParseChatNode(node["extra"]);
            if (node["text"].isString())
                result += node["text"].asString();
            return result;
        }
        if (node.isArray()) {
            std::string result;

            for (auto arrayNode : node)
                result += ParseChatNode(arrayNode);
            return result;
        }
        return "";
    }

    void HandlePacket(protocol::packets::in::ChatPacket* packet) {
        const Json::Value& root = packet->GetChatData();

        std::string message = ParseChatNode(root);
        std::size_t pos = message.find((char)0xA7);
        
        while (pos != std::string::npos) {
            message.erase(pos - 1, 3);
            pos = message.find((char)0xA7);
        }
        
        message.erase(std::remove_if(message.begin(), message.end(), [](char c) {
            return c < 32 || c > 126;
        }), message.end());
        if (message.length() > 0)
            console << std::wstring(message.begin(), message.end()) << "\n";
    }

    void HandlePacket(protocol::packets::in::EntityMetadataPacket* packet) {
        //const auto& metadata = packet->GetMetadata();

        //std::wcout << "Received entity metadata" << std::endl;
    }

    void HandlePacket(protocol::packets::in::SpawnMobPacket* packet) {
        //const auto& metadata = packet->GetMetadata();

        //std::wcout << "Received SpawnMobPacket" << std::endl;
    }

    void HandlePacket(protocol::packets::in::SetSlotPacket* packet) {
        inventory::Slot slot = packet->GetSlot();
        int window = packet->GetWindowId();
        int index = packet->GetSlotIndex();

        console << "Set slot (" << window << ", " << index << ") = " << slot.GetItemId() << ":" << slot.GetItemDamage() << "\n";
    }

    void HandlePacket(protocol::packets::in::WindowItemsPacket* packet) {
        console << "Received window items for WindowId " << (int)packet->GetWindowId() << "." << "\n";

        const std::vector<inventory::Slot>& slots = packet->GetSlots();

        for (const inventory::Slot& slot : slots) {
            s16 id = slot.GetItemId();
            u8 count = slot.GetItemCount();
            s16 dmg = slot.GetItemDamage();
            //const Minecraft::NBT::NBT& nbt = slot.GetNBT();

            if (id != -1)
                console << "Item: " << id << " Amount: " << (int)count << " Dmg: " << dmg << "\n";
        }
    }

    void HandlePacket(protocol::packets::in::WorldBorderPacket* packet) {
        using namespace protocol::packets::in;
        WorldBorderPacket::Action action = packet->GetAction();

        console << "Received world border packet\n";

        switch (action) {
        case WorldBorderPacket::Action::Initialize:
        {
            console << "World border radius: " << (packet->GetDiameter() / 2.0) << "\n";
            console << "World border center: " << packet->GetX() << ", " << packet->GetZ() << "\n";
            console << "World border warning time: " << packet->GetWarningTime() << " seconds, blocks: " << packet->GetWarningBlocks() << " meters" << "\n";
        }
        break;
        default:
            break;
        }
    }

    void OnPlayerSpawn(core::PlayerPtr player) {
        auto entity = player->GetEntity();
        Vector3d pos(0, 0, 0);

        if (entity)
            pos = entity->GetPosition();
        console << "Spawn player " << player->GetName() << " at " << pos << "\n";
    }

    void OnPlayerJoin(core::PlayerPtr player) {
        console << player->GetName() << L" added to playerlist" << "\n";
    }

    void OnPlayerLeave(core::PlayerPtr player) {
        console << player->GetName() << L" removed from playerlist" << "\n";
    }

    void HandlePacket(protocol::packets::in::StatisticsPacket* packet) {
        const auto& stats = packet->GetStatistics();

        for (auto& kv : stats)
            console << kv.first << " = " << kv.second << "\n";
    }

    void HandlePacket(protocol::packets::in::PlayerAbilitiesPacket* packet) {
        console << "Abilities: " << (int)packet->GetFlags() << ", " << packet->GetFlyingSpeed() << ", " << packet->GetFOVModifier() << "\n";
    }

    void HandlePacket(protocol::packets::in::SpawnPositionPacket* packet) {
        console << "Spawn position: " << packet->GetLocation() << "\n";
    }

    void HandlePacket(protocol::packets::in::DisconnectPacket* packet) {
        console << "Disconnected from server. Reason: " << "\n";
        console << packet->GetReason() << "\n";
    }

    void HandlePacket(protocol::packets::in::EncryptionRequestPacket* packet) {
        console << "Encryption request received\n";
    }

    void HandlePacket(protocol::packets::in::LoginSuccessPacket* packet) {
        console << "Successfully logged in. Username: ";
        console << packet->GetUsername() << "\n";
    }

    void HandlePacket(protocol::packets::in::JoinGamePacket* packet) {
        console << "Joining game with entity id of " << packet->GetEntityId() << "\n";
        console << "Game difficulty: " << (int)packet->GetDifficulty() << ", Dimension: " << (int)packet->GetDimension() << "\n";
        console << "Level type: " << packet->GetLevelType() << "\n";
    }

    void HandlePacket(protocol::packets::in::PluginMessagePacket* packet) {
        console << "Plugin message received on channel " << packet->GetChannel();
        auto value = packet->GetData();
        console << " Value: " << std::wstring(value.begin(), value.end()) << "\n";
    }

    void HandlePacket(protocol::packets::in::ServerDifficultyPacket* packet) {
        console << "New server difficulty: " << (int)packet->GetDifficulty() << "\n";
    }
};


PlayerFollower::PlayerFollower(protocol::packets::PacketDispatcher* dispatcher, core::Client* client)
    : m_Client(client),
      m_PlayerManager(*client->GetPlayerManager()),
      m_EntityManager(*client->GetEntityManager()),
      m_PlayerController(*client->GetPlayerController()),
      m_Target(L"")
{
    client->RegisterListener(this);
    m_PlayerManager.RegisterListener(this);
}

PlayerFollower::~PlayerFollower() {
    m_Client->UnregisterListener(this);
    m_PlayerManager.UnregisterListener(this);
}

void PlayerFollower::UpdateRotation() {
    if (!m_Following || !m_Following->GetEntity()) return;

    m_PlayerController.LookAt(m_Following->GetEntity()->GetPosition());
    /*static u64 lastUpdate = GetTime();
    u64 ticks = GetTime();
    float dt = (ticks - lastUpdate) / 1000.0f;
    lastUpdate = ticks;

    static float yaw = 0.0f;
    static float RotationsPerSecond = 0.6f;

    yaw += dt * 360.0f / (1.0f / RotationsPerSecond);*/

    // std::wcout << yaw << std::endl;

    //m_Pitch = (((float)std::sin(ticks * 0.5 * 3.14 / 1000) * 0.5f + 0.5f) * 360.0f) - 180.0f;
    //float yaw = (((float)std::sin(ticks * 0.5 * 3.14 / 1000))) * 360.0f;

    //std::wcout << yaw << std::endl;

    //m_PlayerController.SetYaw(yaw);
    //m_PlayerController.LookAt(m_Following->GetEntity()->GetPosition());
}

void PlayerFollower::OnTick() {
    UpdateRotation();

    if (!m_Following || !m_Following->GetEntity()) {
        m_PlayerController.SetTargetPosition(m_PlayerController.GetPosition());
        return;
    }

    auto entity = m_Following->GetEntity();
    EntityId vid = entity->GetVehicleId();
    Vector3d targetPosition = entity->GetPosition();

    if (vid != -1)
        targetPosition = m_EntityManager.GetEntity(vid)->GetPosition();

    float yaw = (entity->GetYaw() / 256.0f) * 360.0f;
    float pitch = (entity->GetPitch() / 256.0f) * 360.0f;
    const float toRads = 3.14159f / 180.0f;

    if ((yaw += 90) >= 360.0) yaw -= 360;

    Vector3d heading(
        std::cos(yaw * toRads) * std::cos(pitch * toRads),
        0.0,
        std::sin(yaw * toRads) * std::cos(pitch * toRads)
    );

    Vector3d newPosition = targetPosition + Vector3Normalize(heading) * 0.4;

    if ((yaw -= 180.0f + 90.0f) < 0.0f) yaw += 360.0f;
    //m_PlayerController.SetYaw(yaw);
    m_PlayerController.SetMoveSpeed(4.3 * 1.3);
    m_PlayerController.SetTargetPosition(newPosition);
}

bool PlayerFollower::IsIgnored(const std::wstring& name) {
    static const std::vector<std::wstring> IgnoreList = {
        L"Traced_", L"Judam"
    };

    return std::find(IgnoreList.begin(), IgnoreList.end(), name) != IgnoreList.end();
}

void PlayerFollower::FindClosestPlayer() {
    double closest = std::numeric_limits<double>::max();
        

    m_Following = nullptr;

    if (!m_Target.empty()) {
        for (auto& kv : m_PlayerManager) {
            auto player = kv.second;
            auto entity = player->GetEntity();

            if (entity && player->GetName() == m_Target)
                m_Following = player;
        }
    }

    if (!m_Following) {
        for (auto& kv : m_PlayerManager) {
            core::PlayerPtr player = kv.second;

            if (IsIgnored(player->GetName())) continue;

            auto entity = player->GetEntity();

            if (!entity) continue;

            EntityId peid = m_EntityManager.GetPlayerEntity()->GetEntityId();
            if (entity->GetEntityId() == peid) continue;

            Vector3d pos = entity->GetPosition();
            EntityId vid = entity->GetVehicleId();
            if (vid != -1) {
                entity::EntityPtr vehicle = m_EntityManager.GetEntity(vid);
                if (vehicle)
                    pos = vehicle->GetPosition();
            }

            double dist = pos.Distance(m_PlayerController.GetPosition());

            if (dist < closest) {
                closest = dist;
                m_Following = player;
            }
        }
    }

    static u64 lastOutput = 0;

    if (GetTime() - lastOutput >= 3000) {
        lastOutput = GetTime();
        if (m_Following) {
            auto entity = m_Following->GetEntity();
            if (entity) {
                Vector3d pos = entity->GetPosition();
                EntityId vid = entity->GetVehicleId();
                if (vid != -1) {
                    entity::EntityPtr vehicle = m_EntityManager.GetEntity(vid);
                    if (vehicle)
                        pos = vehicle->GetPosition();
                }
                double dist = m_PlayerController.GetPosition().Distance(pos);
                std::wstring followName = m_Following->GetName();

                std::string followMesg = "Tracking " + std::string(followName.begin(), followName.end()) + " dist: " + std::to_string(dist) + "\n";

                console << followMesg;
            }
        }
    }

    if (!m_Following)
        console << "Not tracking anyone\n";
    else
        m_PlayerController.SetTargetPosition(m_Following->GetEntity()->GetPosition());
}

void PlayerFollower::OnPlayerJoin(core::PlayerPtr player) {
    FindClosestPlayer();
    UpdateRotation();
}

void PlayerFollower::OnPlayerLeave(core::PlayerPtr player) {
    FindClosestPlayer();
    UpdateRotation();
}

void PlayerFollower::OnPlayerSpawn(core::PlayerPtr player) {
    FindClosestPlayer();
    UpdateRotation();
}

void PlayerFollower::OnPlayerDestroy(core::PlayerPtr player, EntityId eid) {
    FindClosestPlayer();
    UpdateRotation();
}

void PlayerFollower::OnPlayerMove(core::PlayerPtr player, Vector3d oldPos, Vector3d newPos) {
    FindClosestPlayer();
    UpdateRotation();
}

class CreativeCreator : public protocol::packets::PacketHandler {
private:
    core::Connection* m_Connection;
    PlayerController* m_Controller;
    s16 m_Slot;
    inventory::Slot m_Item;
    bool m_Created;

    std::queue<inventory::Slot> m_CreateQueue;
    
public:
    CreativeCreator(protocol::packets::PacketDispatcher* dispatcher, core::Connection* connection, PlayerController* controller, s16 slot, inventory::Slot item)
        : protocol::packets::PacketHandler(dispatcher),
          m_Connection(connection), m_Controller(controller),
          m_Slot(slot), m_Item(item), m_Created(false)
    {
        using namespace protocol;

        dispatcher->RegisterHandler(State::Play, play::PlayerPositionAndLook, this);
        //dispatcher->RegisterHandler(State::Play, Play::SetSlot, this);
    }

    ~CreativeCreator() {
        GetDispatcher()->UnregisterHandler(this);
    }

    void CreateItem() {
        protocol::packets::out::CreativeInventoryActionPacket packet(m_Slot, m_Item);

        m_Connection->SendPacket(&packet);
    }

    void HandlePacket(protocol::packets::in::SetSlotPacket* packet) {
        int index = packet->GetSlotIndex();
        inventory::Slot slot = packet->GetSlot();

        if (slot.GetItemId() > 0) {
            // Throw item
            using namespace protocol::packets::out;

            PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::DropItemStack;
            Vector3i pos(0, 0, 0);

            PlayerDiggingPacket packet(status, pos, Face::Bottom);

            m_Connection->SendPacket(&packet);


            // Create new item in its place now
            if (!m_CreateQueue.empty()) {
                CreativeInventoryActionPacket packet(36, m_CreateQueue.front());
                m_CreateQueue.pop();
                m_Connection->SendPacket(&packet);
            }
        }
    }

    void HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet) {
        if (m_Created) return;

        m_Created = true;

        std::async(std::launch::async, [&] {
            std::this_thread::sleep_for(std::chrono::seconds(7));

            std::vector<int> colors = {
                //DyeColor::InkSac,
                DyeColor::RoseRed,
                DyeColor::CactusGreen,
                //DyeColor::CocoaBeans,
                DyeColor::LapisLazuli,
                DyeColor::Purple,
                DyeColor::Cyan,
                DyeColor::LightGray,
                DyeColor::Gray,
                DyeColor::Pink,
                DyeColor::Lime,
                DyeColor::DandelionYellow,
                DyeColor::LightBlue,
                DyeColor::Magenta,
                DyeColor::Orange,
                DyeColor::BoneMeal
            };
            int slotIndex = 36;

           /* std::map<u32, bool> usedMap;

            for (u8 colorIndex1 = 0; colorIndex1 < colors.size(); ++colorIndex1) {
                for (u8 colorIndex2 = 0; colorIndex2 < colors.size(); ++colorIndex2) {
                    for (u8 colorIndex3 = 0; colorIndex3 < colors.size(); ++colorIndex3) {
                        if (colorIndex1 == colorIndex2) continue;
                        if (colorIndex1 == colorIndex3) continue;
                        if (colorIndex2 == colorIndex3) continue;

                        std::vector<u8> sorted = { colorIndex1, colorIndex2, colorIndex3 };
                        std::sort(sorted.begin(), sorted.end());

                        u32 combined = (sorted[0] << 16) | (sorted[1] << 8) | sorted[2];

                        if (usedMap.find(combined) != usedMap.end()) continue;

                        usedMap[combined] = true;

                        for (int type = 0; type < 5; ++type) {
                            Minecraft::Slot slot = CreateFirework(false, true, type, 2, std::vector<int> {
                                colors[colorIndex1],
                                colors[colorIndex2],
                                colors[colorIndex3]
                            });

                            m_CreateQueue.push(slot);
                        }
                    }
                }
            }
            std::wcout << m_CreateQueue.size() << std::endl;
            for (int colorIndex = 0; colorIndex < colors.size(); ++colorIndex) {
                for (int type = 0; type < 5; ++type) {
                    Minecraft::Slot slot = CreateFirework(true, true, type, 1, std::vector<int>{colors[colorIndex]});

                    m_CreateQueue.push(slot);
                }
            }

            if (!m_CreateQueue.empty()) {
                Minecraft::Packets::Outbound::CreativeInventoryActionPacket packet(36, m_CreateQueue.front());
                m_CreateQueue.pop();
                m_Connection->SendPacket(&packet);
            }*/
        });

    }
};

std::string StripChatMessage(const std::string& message) {
    std::wstring utf16;

    utf8::utf8to16(message.begin(), message.end(), std::back_inserter(utf16));

    std::size_t pos = utf16.find((wchar_t)0xA7);

    while (pos != std::string::npos) {
        utf16.erase(pos, 2);
        pos = utf16.find((wchar_t)0xA7);
    }

    std::string utf8;
    utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(utf8));

    return utf8;
}

/*#include "ChatWindow.h"
#include <utf8.h>
class ChatWindowController : public Minecraft::Packets::PacketHandler, public Minecraft::PlayerListener {
private:
    Minecraft::Connection* m_Connection;
    Minecraft::PlayerManager* m_PlayerManager;
    ChatWindow m_ChatWindow;
    std::thread m_MessageThread;

public:
    ChatWindowController(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::Connection* connection, Minecraft::PlayerManager* playerManager)
        : Minecraft::Packets::PacketHandler(dispatcher),
          m_Connection(connection),
          m_PlayerManager(playerManager)
    {
        m_ChatWindow.Initialize();

        m_ChatWindow.SetInputCallback(std::bind(&ChatWindowController::OnWindowChatInput, this, std::placeholders::_1));

        using namespace Minecraft::Protocol;

        dispatcher->RegisterHandler(State::Play, Play::Chat, this);
        m_PlayerManager->RegisterListener(this);
    }

    ~ChatWindowController() {
        GetDispatcher()->UnregisterHandler(this);
        m_PlayerManager->UnregisterListener(this);
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChatPacket* packet) {
        Json::Value root = packet->GetChatData();
        std::string str = ParseChatNode(root);

        std::wstring utf16;
        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(utf16));

        std::size_t pos = utf16.find((wchar_t)0xA7);

        while (pos != std::string::npos) {
            utf16.erase(pos, 2);
            pos = utf16.find((wchar_t)0xA7);
        }

        if (utf16.length() == 0) return;

        static const std::wregex ChatRegex(LR"::(^.*?(\[.+\]|).*?<(.+)> (.+)$)::");
        //static const std::wregex ChatRegex(LR"::(^.*?(\[.+\]|).*?(.+): (.+)$)::");

        std::wsregex_iterator begin(utf16.begin(), utf16.end(), ChatRegex);
        std::wsregex_iterator end;

        if (begin == end) {
            m_ChatWindow.AppendChat(L"\r\n" + utf16, ChatColor::Blue);
            return;
        }

        std::wsmatch match = *begin;
        std::wstring channel = match[1];
        std::wstring name = match[2];
        std::wstring message = match[3];

        const int NameLength = 13;

        if (name.length() < NameLength) {
            std::wstring pad = L"";
            for (std::size_t i = 0; i < NameLength - name.length(); ++i)
                pad += L" ";
            name = pad + name;
        } else if (name.length() > NameLength) {
            name = name.substr(0, NameLength);
        }

        std::wstring out = L"\r\n" + channel + name + L"> " + message;

        ChatColor color = ChatColor::Yellow;

        if (channel.find(L"STAFF") != std::string::npos)
            color = ChatColor::Orange;

        m_ChatWindow.AppendChat(out, color);
    }

    void OnPlayerJoin(Minecraft::PlayerPtr player) {
        m_ChatWindow.AddPlayer(player->GetName().c_str());
    }

    void OnPlayerLeave(Minecraft::PlayerPtr player) {
        m_ChatWindow.RemovePlayer(player->GetName().c_str());
    }

    void OnWindowChatInput(const std::wstring& input) {
        Minecraft::Packets::Outbound::ChatPacket packet(input);

        m_Connection->SendPacket(&packet);
    }
};*/

class TrackFollower : public world::WorldListener, public core::PlayerListener {
private:
    struct Track {
        Vector3i position;
        bool visited;

        Track(Vector3i pos) : position(pos), visited(false) { }
    };
    core::PlayerManager& m_PlayerManager;
    world::World& m_World;
    PlayerController& m_PlayerController;
    std::vector<Track> m_Tracks;
    Track* m_TargetTrack;
    u64 m_LastUpdate;

    // Path to follow after initial build up
    std::vector<Track*> m_TrackPath;
    bool m_BuildingTrack;
    std::size_t m_PathIndex;

    bool AtTargetTrack() {
        if (!m_TargetTrack || m_TargetTrack->visited) return true;

        Vector3d position = m_PlayerController.GetPosition();
        return position.Distance(ToVector3d(m_TargetTrack->position)) < 1.0;
    }

    Track* FindConnectedTrack() {
        if (!m_TargetTrack) return nullptr;

        for (std::size_t i = 0; i < m_Tracks.size(); ++i) {
            Track* track = &m_Tracks[i];
            if (track->visited) continue;

            if (m_TargetTrack->position.Distance(track->position) <= 1)
                return track;
        }
        return nullptr;
    }

    Track* FindNextTrack() {
        if (!m_BuildingTrack) {
            if (m_PathIndex >= m_TrackPath.size()) return nullptr;

            return m_TrackPath[m_PathIndex];
        }

        Track* connected = FindConnectedTrack();

        if (connected)
            return connected;

        Vector3i position = ToVector3i(m_PlayerController.GetPosition());
        Track* closest = nullptr;
        double closest_dist = std::numeric_limits<double>::max();

        for (Track& track : m_Tracks) {
            if (track.visited) continue;
            double dist = position.Distance(track.position);
            if (dist < closest_dist) {
                closest_dist = dist;
                closest = &track;
            }
        }

        return closest;
    }

public:
    TrackFollower(protocol::packets::PacketDispatcher* dispatcher, core::PlayerManager& playerManager, world::World& world, PlayerController& playerController)
        : m_PlayerManager(playerManager),
          m_World(world),
          m_PlayerController(playerController),
          m_TargetTrack(nullptr),
          m_LastUpdate(GetTime()),
          m_PathIndex(0),
          m_BuildingTrack(true)
    {
        m_PlayerManager.RegisterListener(this);
        m_World.RegisterListener(this);
    }

    ~TrackFollower() {
        m_PlayerManager.UnregisterListener(this);
        m_World.UnregisterListener(this);
    }

    void Update() {
        if (m_Tracks.empty()) return;

        if (AtTargetTrack()) {
            if (m_TargetTrack)
                m_TargetTrack->visited = true;

            Track* next = FindNextTrack();

            if (!next) {
                for (Track& track : m_Tracks)
                    track.visited = false;

                m_BuildingTrack = false;
                m_PathIndex = 0;
                return;
            }

            if (m_BuildingTrack)
                m_TrackPath.push_back(next);
            else if (next)
                ++m_PathIndex;

            m_TargetTrack = next;
        }

        if (m_TargetTrack)
            m_PlayerController.SetTargetPosition(ToVector3d(m_TargetTrack->position) + Vector3d(0.5, 0, 0.5));
    }

    void OnChunkLoad(world::ChunkPtr chunk, const world::ChunkColumnMetadata& meta, u16 yIndex) {
        static const std::vector<block::BlockPtr> trackBlocks = {
            block::BlockRegistry::GetInstance()->GetBlock(27, 0),
            block::BlockRegistry::GetInstance()->GetBlock(28, 0),
            block::BlockRegistry::GetInstance()->GetBlock(66, 0)
        };

        s64 chunkX = meta.x * 16;
        s64 chunkZ = meta.z * 16;

        std::vector<Vector3i> tracksFound;

        for (u16 y = 0; y < 16; ++y) {
            for (u16 z = 0; z < 16; ++z) {
                for (u16 x = 0; x < 16; ++x) {
                    block::BlockPtr block = chunk->GetBlock(Vector3i(x, y, z)).GetBlock();

                    for (block::BlockPtr findBlock : trackBlocks) {
                        u16 blockId = findBlock->GetType();
                        u16 blockMeta = findBlock->GetMeta();

                        if (block && blockId == block->GetType() && blockMeta == block->GetMeta()) {
                            s64 blockX = chunkX + x;
                            s64 blockY = yIndex * 16 + y;
                            s64 blockZ = chunkZ + z;

                            Vector3i pos(blockX, blockY, blockZ);

                            tracksFound.push_back(pos);
                        }
                    }
                }
            }
        }


        for (const Vector3i& pos : tracksFound) {
            auto iter = std::find_if(m_Tracks.begin(), m_Tracks.end(), [pos](Track track) {
                return track.position.Distance(pos) < 1.0;
            });

            if (iter == m_Tracks.end()) {
                if (m_PlayerController.ClearPath(ToVector3d(pos))) {
                    console << "Adding track at position " << pos << "\n";
                    m_BuildingTrack = true;
                    m_TrackPath.clear();
                    m_PathIndex = 0;
                    m_Tracks.push_back(Track(pos));
                }
            }
        }
    }

};

class BlockFinder : public world::WorldListener {
private:
    world::World* m_World;

    u16 m_BlockId;
    u16 m_BlockMeta;

public:
    BlockFinder(world::World* world, u16 blockId, u16 blockMeta)
        : m_World(world),
          m_BlockId(blockId),
          m_BlockMeta(blockMeta)
    {
        world->RegisterListener(this);
    }

    ~BlockFinder() {
        m_World->UnregisterListener(this);
    }

    void OnChunkLoad(world::ChunkPtr chunk, const world::ChunkColumnMetadata& meta, u16 yIndex) {
        s64 chunkX = meta.x * 16;
        s64 chunkZ = meta.z * 16;

        for (u16 y = 0; y < 16; ++y) {
            for (u16 z = 0; z < 16; ++z) {
                for (u16 x = 0; x < 16; ++x) {
                    block::BlockPtr block = chunk->GetBlock(Vector3i(x, y, z)).GetBlock();

                    if (block && m_BlockId == block->GetType() && m_BlockMeta == block->GetMeta()) {
                        s64 blockX = chunkX + x;
                        s64 blockY = yIndex * 16 + y;
                        s64 blockZ = chunkZ + z;

                        Vector3i pos(blockX, blockY, blockZ);
                        console << "Block " << m_BlockId << ":" << m_BlockMeta << " found at " << pos << "\n";
                    }
                }
            }
        }
    }
};

} // ns util
} // ns mc
