#include "DataBuffer.h"
#include "Protocol.h"
#include "World.h"
#include "PlayerManager.h"
#include "Entity/EntityManager.h"
#include "Connection.h"
#include "Utility.h"

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

#undef max
#undef min

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

s64 GetTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Minecraft::Slot CreateFirework(bool flicker, bool trail, u8 type, u8 duration, std::vector<int> colors, const std::string& name = "") {
    using namespace Minecraft::NBT;

    NBT nbt;

    TagCompound* fireworks = new TagCompound(L"Fireworks");
    TagPtr flightTag(new TagByte("Flight", duration));

    fireworks->AddItem(flightTag);

    TagList* explosions = new TagList("Explosions", TagType::Compound);

    TagCompound* explosion = new TagCompound(L"Explosion");
    TagPtr flickerTag(new TagByte("Flicker", flicker ? 1 : 0));
    TagPtr trailTag(new TagByte("Trail", trail ? 1 : 0));
    TagPtr typeTag(new TagByte("Type", type));

    TagPtr colorsTag(new TagIntArray("Colors", colors));
    //TagPtr fadeColorsTag(new TagIntArray("FadeColors", colors));

    explosion->AddItem(flickerTag);
    explosion->AddItem(trailTag);
    explosion->AddItem(typeTag);
    explosion->AddItem(colorsTag);

    explosions->AddItem(TagPtr(explosion));
    fireworks->AddItem(TagPtr(explosions));
    nbt.GetRoot().AddItem(TagPtr(fireworks));
    nbt.GetRoot().SetName(L"tag");

    if (!name.empty()) {
        TagCompound* display = new TagCompound(L"display");
        TagPtr nameTag(new TagString("Name", name));

        display->AddItem(nameTag);

        nbt.GetRoot().AddItem(TagPtr(display));
    }

    Minecraft::Slot slot(401, 64, 0, nbt);

    return slot;
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

PlayerController::PlayerController(Minecraft::Connection* connection, Minecraft::World& world, Minecraft::PlayerManager& playerManager)
    : m_PlayerManager(playerManager),
      m_Connection(connection),
      m_World(world),
      m_Position(0, 0, 0),
      m_BoundingBox(Vector3d(0, 0, 0), Vector3d(0.6, 1.8, 0.6)),
      m_EntityId(-1),
      m_LastUpdate(GetTime()),
      m_Sprinting(false),
      m_LoadedIn(false),
      m_MoveSpeed(4.3),
      m_HandleFall(true)
{
    m_PlayerManager.RegisterListener(this);

    //console.SetImpl(new LoggerConsole("PlayerController.log"));
}

PlayerController::~PlayerController() {
    m_PlayerManager.UnregisterListener(this);
}

AABB PlayerController::GetBoundingBox() const {
    Vector3d pos = m_Position;
    Vector3d min = m_BoundingBox.min;
    Vector3d max = m_BoundingBox.max;

    pos -= Vector3d(max.x / 2, 0.0, max.z / 2);

    return AABB(pos + min, pos + max);
}

bool PlayerController::ClearPath(Vector3d target) {
    Vector3d position = m_Position;

    double dist = target.Distance(position);

    Vector3d toTarget = target - position;
    Vector3d n = Vector3Normalize(toTarget);

    Vector3d side = n.Cross(Vector3d(0, 1, 0));

    const double CheckWidth = 0.3;

    auto check = [&](Vector3d start, Vector3d delta) {
        Vector3d checkAbove = start + delta + Vector3d(0, 1, 0);
        Vector3d checkBelow = start + delta + Vector3d(0, 0, 0);
        Minecraft::BlockPtr aboveBlock = m_World.GetBlock(checkAbove);
        Minecraft::BlockPtr belowBlock = m_World.GetBlock(checkBelow);

        if (aboveBlock && aboveBlock->IsSolid())
            return false;

        if (belowBlock && belowBlock->IsSolid()) {
            Minecraft::BlockPtr twoAboveBlock = m_World.GetBlock(checkAbove + Vector3d(0, 1, 0));
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
        Minecraft::BlockPtr floorBlock = m_World.GetBlock(checkFloor);
        if (floorBlock && !floorBlock->IsSolid()) {
            Minecraft::BlockPtr belowFloorBlock = m_World.GetBlock(checkFloor - Vector3d(0, 1, 0));

            // Fail if there is a two block drop
            if (belowFloorBlock && !belowFloorBlock->IsSolid())
                return false;

            position.y--;
        }
    }

    return true;
}

void PlayerController::SetMoveSpeed(double speed) { m_MoveSpeed = speed; }

void PlayerController::OnClientSpawn(Minecraft::PlayerPtr player) {
    m_Yaw = player->GetEntity()->GetYaw();
    m_Pitch = player->GetEntity()->GetPitch();
    m_Position = player->GetEntity()->GetPosition();
    m_LoadedIn = true;
    m_TargetPos = m_Position;
    auto entity = player->GetEntity();
    if (entity) {
        Minecraft::EntityId eid = entity->GetEntityId();

        m_EntityId = eid;
    }
}

void PlayerController::Dig(Vector3d target) {
    Vector3d toTarget = target - m_Position;

    if (toTarget.Length() > 6) return;

    m_DigQueue.push(target);
}

void PlayerController::Attack(Minecraft::EntityId id) {
    static u64 timer = 0;
    static const u64 cooldown = 500;

    if (GetTime() - timer < cooldown)
        return;

    using namespace Minecraft::Packets::Outbound;

    UseEntityPacket useEntity(id, UseEntityPacket::Action::Attack);
    m_Connection->SendPacket(&useEntity);

    timer = GetTime();
}

void PlayerController::UpdateDigging() {
    if (m_DigQueue.empty()) return;

    //Vector3d target = m_DigQueue.front();

}

bool PlayerController::HandleJump() {
    const float FullCircle = 2.0f * 3.14159f;
    AABB playerBounds = GetBoundingBox();
    const float CheckWidth = (float)m_BoundingBox.max.x / 2.0f;

    for (float angle = 0.0f; angle < FullCircle; angle += FullCircle / 8) {
        Vector3d checkPos = m_Position + Vector3RotateAboutY(Vector3d(0, 0, CheckWidth), angle);

        Minecraft::BlockPtr checkBlock = m_World.GetBlock(checkPos);
        if (checkBlock && checkBlock->IsSolid()) {
            Vector3i pos = ToVector3i(checkPos);
            /*AABB bounds = checkBlock->GetBoundingBox(pos);

            if (playerBounds.Intersects(bounds)) {
                m_Position.y += checkBlock->GetBoundingBox().max.y;
                return true;
            }*/

            if (checkBlock->CollidesWith(m_Position, playerBounds)) {
                m_Position.y += checkBlock->GetBoundingBox().max.y;
                return true;
            }
        }
    }

    return false;
}

bool PlayerController::HandleFall() {
    // Ray cast downards FallSpeed magnitude, use collision spot as new height
    const float FullCircle = 2.0f * 3.14159f;
    const float CheckWidth = (float)m_BoundingBox.max.x / 2.0f;
    const float RayCastStep = 0.05f;
    double bestDist = FallSpeed;
    Minecraft::BlockPtr bestBlock = nullptr;

    if (!InLoadedChunk())
        return false;

    for (float angle = 0.0f; angle < FullCircle; angle += FullCircle / 8) {
        Vector3d checkPos = m_Position + Vector3RotateAboutY(Vector3d(0, 0, CheckWidth), angle);
        float dist = 0.0f;
        Minecraft::BlockPtr checkBlock = nullptr;

        for (dist = 0.0f; dist < FallSpeed; dist += RayCastStep) {
            checkPos.y = m_Position.y - dist;

            checkBlock = m_World.GetBlock(checkPos);
            if (checkBlock && checkBlock->IsSolid()) {
                AABB blockBounds = checkBlock->GetBoundingBox();
                Vector3d point = checkPos - ToVector3d(ToVector3i(checkPos));
                if (blockBounds.Contains(point)) {
                    dist -= RayCastStep;
                    checkPos.y += RayCastStep;
                    break;
                }
            }
        }

        if (dist < bestDist) {
            bestDist = dist;
            bestBlock = m_World.GetBlock(checkPos);
        }
    }

    m_Position.y -= bestDist;
    // Drop down through the block then move back up to top of bounding box.
    m_Position.y = (double)(u64)m_Position.y;
    if (bestBlock)
        m_Position.y += bestBlock->GetBoundingBox().max.y;
    /*if (m_Position.y - (u64)m_Position.y < RayCastStep)
        m_Position.y = (u64)m_Position.y;*/

//      Minecraft::BlockPtr currentBlock = m_World.GetBlock(m_Position - Vector3d(0, FallSpeed, 0));

//        Vector3d highestPos = ToVector3d(ToVector3i(m_Position)) + Vector3d(0, currentBlock->GetBoundingBox().max.y, 0);

    return false;
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
        console << "Current pos: " << GetPosition();
        counter = 0;
    }

    Vector3d target = m_TargetPos;
    Vector3d toTarget = target - GetPosition();
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

    static s64 lastSend = 0;
    static u64 StartTime = GetTime();

    if (GetTime() - lastSend >= 50) {
        bool onGround = true;

        if (HandleJump()) {
            console << "Jumping\n";
        } else {
            if (!m_HandleFall) {
                const float FullCircle = 2.0f * 3.14159f;
                const float CheckWidth = (float)m_BoundingBox.max.x / 2.0f;
                onGround = false;
                for (float angle = 0.0f; angle < FullCircle; angle += FullCircle / 8) {
                    Vector3d checkPos = m_Position + Vector3RotateAboutY(Vector3d(0, 0, CheckWidth), angle) - Vector3d(0, 1, 0);

                    Minecraft::BlockPtr checkBlock = m_World.GetBlock(checkPos);
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

        Minecraft::Packets::Outbound::PlayerPositionAndLookPacket response(m_Position,
            m_Yaw * 180.0f / 3.14159f, m_Pitch * 180.0f / 3.14159f, onGround);

        m_Connection->SendPacket(&response);

        UpdateDigging();

        lastSend = GetTime();
    }
}

bool PlayerController::InLoadedChunk() const {
    return m_World.GetChunk(ToVector3i(m_Position)) != nullptr;
}

Vector3d PlayerController::GetPosition() const { return m_Position; }
Vector3d PlayerController::GetHeading() const {
    return Vector3d(
        -std::cosf(GetPitch()) * std::sinf(GetYaw()),
        -std::sinf(GetPitch()),
        std::cosf(GetPitch()) * std::cosf(GetYaw())
    );
}
float PlayerController::GetYaw() const { return m_Yaw; }
float PlayerController::GetPitch() const { return m_Pitch; }

void PlayerController::Move(Vector3d delta) {
    //delta.y = 0;

    if (m_EntityId != -1) {
        Minecraft::PlayerPtr player = m_PlayerManager.GetPlayerByEntityId(m_EntityId);
        if (player) {
            Minecraft::EntityPtr entity = player->GetEntity();

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

class EventLogger : public Minecraft::PlayerListener, public Minecraft::ConnectionListener, public Minecraft::Packets::PacketHandler {
private:
    Minecraft::PlayerManager* m_PlayerManager;
    Minecraft::Connection* m_Connection;

public:
    EventLogger(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::PlayerManager* playerManager, Minecraft::Connection* connection)
        : Minecraft::Packets::PacketHandler(dispatcher),
          m_PlayerManager(playerManager),
          m_Connection(connection)
    {
        m_PlayerManager->RegisterListener(this);
        m_Connection->RegisterListener(this);

        using namespace Minecraft::Protocol;

        dispatcher->RegisterHandler(State::Login, Login::Disconnect, this);
        dispatcher->RegisterHandler(State::Login, Login::EncryptionRequest, this);
        dispatcher->RegisterHandler(State::Login, Login::LoginSuccess, this);

        dispatcher->RegisterHandler(State::Play, Play::KeepAlive, this);
        dispatcher->RegisterHandler(State::Play, Play::JoinGame, this);
        dispatcher->RegisterHandler(State::Play, Play::Chat, this);
        dispatcher->RegisterHandler(State::Play, Play::SpawnPosition, this);
        dispatcher->RegisterHandler(State::Play, Play::UpdateHealth, this);
        dispatcher->RegisterHandler(State::Play, Play::PlayerPositionAndLook, this);
        dispatcher->RegisterHandler(State::Play, Play::SpawnPlayer, this);
        dispatcher->RegisterHandler(State::Play, Play::SpawnMob, this);
        dispatcher->RegisterHandler(State::Play, Play::EntityRelativeMove, this);
        dispatcher->RegisterHandler(State::Play, Play::EntityMetadata, this);
        dispatcher->RegisterHandler(State::Play, Play::SetExperience, this);
        dispatcher->RegisterHandler(State::Play, Play::EntityProperties, this);
        dispatcher->RegisterHandler(State::Play, Play::ChunkData, this);
        dispatcher->RegisterHandler(State::Play, Play::SetSlot, this);
        dispatcher->RegisterHandler(State::Play, Play::WindowItems, this);
        dispatcher->RegisterHandler(State::Play, Play::Statistics, this);
        dispatcher->RegisterHandler(State::Play, Play::PlayerAbilities, this);
        dispatcher->RegisterHandler(State::Play, Play::PluginMessage, this);
        dispatcher->RegisterHandler(State::Play, Play::Disconnect, this);
        dispatcher->RegisterHandler(State::Play, Play::ServerDifficulty, this);
        dispatcher->RegisterHandler(State::Play, Play::WorldBorder, this);
    }

    ~EventLogger() {
        m_PlayerManager->UnregisterListener(this);
        m_Connection->UnregisterListener(this);
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet) {
        
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChunkDataPacket* packet) {
        /*auto column = packet->GetChunkColumn();
        auto x = column->GetMetadata().x;
        auto z = column->GetMetadata().z;

        std::wcout << "Received chunk data for chunk " << x << ", " << z << std::endl;*/
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityPropertiesPacket* packet) {
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

    void HandlePacket(Minecraft::Packets::Inbound::EntityRelativeMovePacket* packet) {

    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnPlayerPacket* packet) {
        /*float x, y, z;
        x = packet->GetX();
        y = packet->GetY();
        z = packet->GetZ();
        m_Out << "Spawn player " << packet->GetUUID() << " at (" << x << ", " << y << ", " << z << ")" << std::endl;*/
    }

    void HandlePacket(Minecraft::Packets::Inbound::UpdateHealthPacket* packet) {
        console << "Set health. health: " << packet->GetHealth() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetExperiencePacket* packet) {
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

    void HandlePacket(Minecraft::Packets::Inbound::ChatPacket* packet) {
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

    void HandlePacket(Minecraft::Packets::Inbound::EntityMetadataPacket* packet) {
        //const auto& metadata = packet->GetMetadata();

        //std::wcout << "Received entity metadata" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnMobPacket* packet) {
        //const auto& metadata = packet->GetMetadata();

        //std::wcout << "Received SpawnMobPacket" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetSlotPacket* packet) {
        Minecraft::Slot slot = packet->GetSlot();
        int window = packet->GetWindowId();
        int index = packet->GetSlotIndex();

        console << "Set slot (" << window << ", " << index << ") = " << slot.GetItemId() << ":" << slot.GetItemDamage() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::WindowItemsPacket* packet) {
        console << "Received window items for WindowId " << (int)packet->GetWindowId() << "." << "\n";

        const std::vector<Minecraft::Slot>& slots = packet->GetSlots();

        for (const Minecraft::Slot& slot : slots) {
            s16 id = slot.GetItemId();
            u8 count = slot.GetItemCount();
            s16 dmg = slot.GetItemDamage();
            //const Minecraft::NBT::NBT& nbt = slot.GetNBT();

            if (id != -1)
                console << "Item: " << id << " Amount: " << (int)count << " Dmg: " << dmg << "\n";
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::WorldBorderPacket* packet) {
        using namespace Minecraft::Packets::Inbound;
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

    void OnPlayerSpawn(Minecraft::PlayerPtr player) {
        auto entity = player->GetEntity();
        Vector3d pos(0, 0, 0);

        if (entity)
            pos = entity->GetPosition();
        console << "Spawn player " << player->GetName() << " at " << pos << "\n";
    }

    void OnPlayerJoin(Minecraft::PlayerPtr player) {
        console << player->GetName() << L" added to playerlist" << "\n";
    }

    void OnPlayerLeave(Minecraft::PlayerPtr player) {
        console << player->GetName() << L" removed from playerlist" << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::StatisticsPacket* packet) {
        const auto& stats = packet->GetStatistics();

        for (auto& kv : stats)
            console << kv.first << " = " << kv.second << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerAbilitiesPacket* packet) {
        console << "Abilities: " << (int)packet->GetFlags() << ", " << packet->GetFlyingSpeed() << ", " << packet->GetFOVModifier() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnPositionPacket* packet) {
        console << "Spawn position: " << packet->GetLocation() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::DisconnectPacket* packet) {
        console << "Disconnected from server. Reason: " << "\n";
        console << packet->GetReason() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::EncryptionRequestPacket* packet) {
        console << "Encryption request received\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::LoginSuccessPacket* packet) {
        console << "Successfully logged in. Username: ";
        console << packet->GetUsername() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::JoinGamePacket* packet) {
        console << "Joining game with entity id of " << packet->GetEntityId() << "\n";
        console << "Game difficulty: " << (int)packet->GetDifficulty() << ", Dimension: " << (int)packet->GetDimension() << "\n";
        console << "Level type: " << packet->GetLevelType() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::PluginMessagePacket* packet) {
        console << "Plugin message received on channel " << packet->GetChannel();
        auto value = packet->GetData();
        console << " Value: " << std::wstring(value.begin(), value.end()) << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::ServerDifficultyPacket* packet) {
        console << "New server difficulty: " << (int)packet->GetDifficulty() << "\n";
    }
};

class PlayerFollower : public Minecraft::PlayerListener {
private:
    Minecraft::PlayerManager& m_PlayerManager;
    Minecraft::EntityManager& m_EntityManager;
    Minecraft::PlayerPtr m_Following;
    PlayerController& m_PlayerController;
    std::wstring m_Target;
    u64 m_LastUpdate;
    
public:
    PlayerFollower(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::PlayerManager& playerManager, Minecraft::EntityManager& emanager, PlayerController& playerController)
        : m_PlayerManager(playerManager),
          m_EntityManager(emanager),
          m_PlayerController(playerController),
          m_Target(L"")
    {
        m_PlayerManager.RegisterListener(this);
    }

    ~PlayerFollower() {
        m_PlayerManager.UnregisterListener(this);
    }

    void UpdateRotation() {
        if (!m_Following || !m_Following->GetEntity()) return;

        static u64 lastUpdate = GetTime();
        u64 ticks = GetTime();
        float dt = (ticks - lastUpdate) / 1000.0f;
        lastUpdate = ticks;

        static float yaw = 0.0f;
        static float RotationsPerSecond = 0.6f;

        yaw += dt * 360.0f / (1.0f / RotationsPerSecond);

       // std::wcout << yaw << std::endl;

        //m_Pitch = (((float)std::sin(ticks * 0.5 * 3.14 / 1000) * 0.5f + 0.5f) * 360.0f) - 180.0f;
        //float yaw = (((float)std::sin(ticks * 0.5 * 3.14 / 1000))) * 360.0f;

        //std::wcout << yaw << std::endl;

        m_PlayerController.SetYaw(yaw);
        //m_PlayerController.LookAt(m_Following->GetEntity()->GetPosition());
    }

    void Update() {
        UpdateRotation();

        if (!m_Following || !m_Following->GetEntity()) {
            m_PlayerController.SetTargetPosition(m_PlayerController.GetPosition());
            return;
        }

        auto entity = m_Following->GetEntity();
        Minecraft::EntityId vid = entity->GetVehicleId();
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
        m_PlayerController.SetYaw(yaw);
        m_PlayerController.SetMoveSpeed(8.6);
        m_PlayerController.SetTargetPosition(newPosition);
    }

    bool IsIgnored(const std::wstring& name) {
        static const std::vector<std::wstring> IgnoreList = {
            L"Traced_", L"Judam"
        };

        return std::find(IgnoreList.begin(), IgnoreList.end(), name) != IgnoreList.end();
    }

    void FindClosestPlayer() {
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
                Minecraft::PlayerPtr player = kv.second;

                if (IsIgnored(player->GetName())) continue;

                auto entity = player->GetEntity();

                if (!entity) continue;

                Minecraft::EntityId peid = m_EntityManager.GetPlayerEntity()->GetEntityId();
                if (entity->GetEntityId() == peid) continue;

                Vector3d pos = entity->GetPosition();
                Minecraft::EntityId vid = entity->GetVehicleId();
                if (vid != -1) {
                    Minecraft::EntityPtr vehicle = m_EntityManager.GetEntity(vid);
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
                    Minecraft::EntityId vid = entity->GetVehicleId();
                    if (vid != -1) {
                        Minecraft::EntityPtr vehicle = m_EntityManager.GetEntity(vid);
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

    void OnPlayerJoin(Minecraft::PlayerPtr player) {
        FindClosestPlayer();
        UpdateRotation();
    }

    void OnPlayerLeave(Minecraft::PlayerPtr player) {
        FindClosestPlayer();
        UpdateRotation();
    }

    void OnPlayerSpawn(Minecraft::PlayerPtr player) {
        FindClosestPlayer();
        UpdateRotation();
    }

    void OnPlayerDestroy(Minecraft::PlayerPtr player, Minecraft::EntityId eid) {
        FindClosestPlayer();
        UpdateRotation();
    }

    void OnPlayerMove(Minecraft::PlayerPtr player, Vector3d oldPos, Vector3d newPos) {
        FindClosestPlayer();
        UpdateRotation();
    }
};

#include <future>

class CreativeCreator : public Minecraft::Packets::PacketHandler {
private:
    Minecraft::Connection* m_Connection;
    PlayerController* m_Controller;
    s16 m_Slot;
    Minecraft::Slot m_Item;
    bool m_Created;

    std::queue<Minecraft::Slot> m_CreateQueue;
    
public:
    CreativeCreator(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::Connection* connection, PlayerController* controller, s16 slot, Minecraft::Slot item)
        : Minecraft::Packets::PacketHandler(dispatcher),
          m_Connection(connection), m_Controller(controller),
          m_Slot(slot), m_Item(item), m_Created(false)
    {
        using namespace Minecraft::Protocol;

        dispatcher->RegisterHandler(State::Play, Play::PlayerPositionAndLook, this);
        //dispatcher->RegisterHandler(State::Play, Play::SetSlot, this);
    }

    ~CreativeCreator() {
        GetDispatcher()->UnregisterHandler(this);
    }

    void CreateItem() {
        Minecraft::Packets::Outbound::CreativeInventoryActionPacket packet(m_Slot, m_Item);

        m_Connection->SendPacket(&packet);
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetSlotPacket* packet) {
        int index = packet->GetSlotIndex();
        Minecraft::Slot slot = packet->GetSlot();

        if (slot.GetItemId() > 0) {
            // Throw item
            using namespace Minecraft::Packets::Outbound;

            PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::DropItemStack;
            Vector3i pos(0, 0, 0);

            PlayerDiggingPacket packet(status, pos, 0);

            m_Connection->SendPacket(&packet);


            // Create new item in its place now
            if (!m_CreateQueue.empty()) {
                Minecraft::Packets::Outbound::CreativeInventoryActionPacket packet(36, m_CreateQueue.front());
                m_CreateQueue.pop();
                m_Connection->SendPacket(&packet);
            }
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet) {
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
            /*for (int colorIndex = 0; colorIndex < colors.size(); ++colorIndex) {
                for (int type = 0; type < 5; ++type) {
                    Minecraft::Slot slot = CreateFirework(true, true, type, 1, std::vector<int>{colors[colorIndex]});

                    m_CreateQueue.push(slot);
                }
            }*/

            /*if (!m_CreateQueue.empty()) {
                Minecraft::Packets::Outbound::CreativeInventoryActionPacket packet(36, m_CreateQueue.front());
                m_CreateQueue.pop();
                m_Connection->SendPacket(&packet);
            }*/
        });

    }
};

#include <utf8.h>

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

class TrackFollower : public Minecraft::WorldListener, public Minecraft::PlayerListener {
private:
    struct Track {
        Vector3i position;
        bool visited;

        Track(Vector3i pos) : position(pos), visited(false) { }
    };
    Minecraft::PlayerManager& m_PlayerManager;
    Minecraft::World& m_World;
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
    TrackFollower(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::PlayerManager& playerManager, Minecraft::World& world, PlayerController& playerController)
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

    void OnChunkLoad(Minecraft::ChunkPtr chunk, const Minecraft::ChunkColumnMetadata& meta, u16 yIndex) {
        static const std::vector<Minecraft::BlockPtr> trackBlocks = {
            Minecraft::BlockRegistry::GetInstance()->GetBlock(27, 0),
            Minecraft::BlockRegistry::GetInstance()->GetBlock(28, 0),
            Minecraft::BlockRegistry::GetInstance()->GetBlock(66, 0)
        };

        s64 chunkX = meta.x * 16;
        s64 chunkZ = meta.z * 16;

        std::vector<Vector3i> tracksFound;

        for (u16 y = 0; y < 16; ++y) {
            for (u16 z = 0; z < 16; ++z) {
                for (u16 x = 0; x < 16; ++x) {
                    Minecraft::BlockPtr block = chunk->GetBlock(Vector3i(x, y, z));

                    for (Minecraft::BlockPtr findBlock : trackBlocks) {
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

class BlockFinder : public Minecraft::WorldListener {
private:
    Minecraft::World* m_World;

    u16 m_BlockId;
    u16 m_BlockMeta;

public:
    BlockFinder(Minecraft::World* world, u16 blockId, u16 blockMeta)
        : m_World(world),
          m_BlockId(blockId),
          m_BlockMeta(blockMeta)
    {
        world->RegisterListener(this);
    }

    ~BlockFinder() {
        m_World->UnregisterListener(this);
    }

    void OnChunkLoad(Minecraft::ChunkPtr chunk, const Minecraft::ChunkColumnMetadata& meta, u16 yIndex) {
        s64 chunkX = meta.x * 16;
        s64 chunkZ = meta.z * 16;

        for (u16 y = 0; y < 16; ++y) {
            for (u16 z = 0; z < 16; ++z) {
                for (u16 x = 0; x < 16; ++x) {
                    Minecraft::BlockPtr block = chunk->GetBlock(Vector3i(x, y, z));

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

