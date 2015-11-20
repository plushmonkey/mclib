#include "DataBuffer.h"
#include "Protocol.h"
#include "World.h"
#include "PlayerManager.h"
#include "Entity/EntityManager.h"
#include "Connection.h"

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

#ifdef max
#undef max
#endif

s64 GetTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

class PlayerController : public Minecraft::PlayerListener {
private:
    Minecraft::PlayerManager& m_PlayerManager;
    Minecraft::Connection* m_Connection;
    Minecraft::World& m_World;
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;
    
    std::queue<Vector3d> m_DigQueue;
    
public:
    PlayerController(Minecraft::Connection* connection, Minecraft::World& world, Minecraft::PlayerManager& playerManager)
        : m_PlayerManager(playerManager), 
          m_Connection(connection),
          m_World(world),
          m_Position(0, 0, 0)
    { 
        m_PlayerManager.RegisterListener(this);
    }

    ~PlayerController() {
        m_PlayerManager.UnregisterListener(this);
    }

    bool ClearPath(Vector3d target) {
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

    void OnClientSpawn(Minecraft::PlayerPtr player) {
        m_Yaw = player->GetEntity()->GetYaw();
        m_Pitch = player->GetEntity()->GetPitch();
        m_Position = player->GetEntity()->GetPosition();
    }

    void Dig(Vector3d target) {
        Vector3d toTarget = target - m_Position;

        if (toTarget.Length() > 6) return;

        m_DigQueue.push(target);
    }

    void Attack(Minecraft::EntityId id) {
        static u64 timer = 0;
        static const u64 cooldown = 500;

        if (GetTime() - timer < cooldown)
            return;

        using namespace Minecraft::Packets::Outbound;

        UseEntityPacket useEntity(id, UseEntityPacket::Action::Attack);
        m_Connection->SendPacket(&useEntity);

        timer = GetTime();
    }

    void UpdateDigging() {
        if (m_DigQueue.empty()) return;

        //Vector3d target = m_DigQueue.front();

    }

    bool HandleJump() {
        const float CheckWidth = 0.3f;
        const float FullCircle = 2.0f * 3.14159f;
        bool jump = false;

        for (float angle = 0.0f; angle < FullCircle; angle += FullCircle / 8) {
            Vector3d checkPos = m_Position + Vector3RotateAboutY(Vector3d(0, 0, CheckWidth), angle);

            Minecraft::BlockPtr checkBlock = m_World.GetBlock(checkPos);
            if (checkBlock && checkBlock->IsSolid()) {
                jump = true;
                break;
            }
        }
        return jump;
    }

    bool HandleFall() {
        const float CheckWidth = 0.3f;
        const float FullCircle = 2.0f * 3.14159f;
        bool fall = true;

        for (float angle = 0.0f; angle < FullCircle; angle += FullCircle / 8) {
            Vector3d checkPos = m_Position + Vector3RotateAboutY(Vector3d(0, -1, CheckWidth), angle);

            Minecraft::BlockPtr checkBlock = m_World.GetBlock(checkPos);
            if (!checkBlock || (checkBlock && checkBlock->IsSolid())) {
                fall = false;
                break;
            }
        }

        return fall;
    }

    void Update() {
        if (m_Position == Vector3d(0, 0, 0)) return;
        static s64 lastSend = 0;
        static u64 StartTime = GetTime();

        m_Position.y = (double)(s64)m_Position.y;

        if (GetTime() - lastSend >= 50) {
            bool onGround = true;

            if (HandleJump()) {
                std::cout << "Jumping\n";
                m_Position.y++;
            } else {
                if (HandleFall()) {
                    std::cout << "Falling\n";
                    m_Position.y -= 4.3 * (50 / 1000.0);
                    onGround = false;
                }
            }

            u64 ticks = GetTime() - StartTime;

            
            m_Pitch = (((float)std::sin(ticks * 0.5 * 3.14 / 1000) * 0.5f + 0.5f)  * 360.0f) - 180.0f;

            Minecraft::Packets::Outbound::PlayerPositionAndLookPacket response(m_Position.x, m_Position.y, m_Position.z,
                m_Yaw, m_Pitch, onGround);

            m_Connection->SendPacket(&response);

            UpdateDigging();

            lastSend = GetTime();
        }
    }

    Vector3d GetPosition() const { return m_Position; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }

    void Move(Vector3d delta) { 
        delta.y = 0;
        m_Position += delta;
    }

    void SetYaw(float yaw) { m_Yaw = yaw; }
    void SetPitch(float pitch) { m_Pitch = pitch; }
    void LookAt(Vector3d target) {
        Vector3d toTarget = target - m_Position;

        double dist = std::sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z);
        double pitch = -std::atan2(toTarget.y, dist);
        double yaw = -std::atan2(toTarget.x, toTarget.z);

        SetYaw((float)(yaw * 180 / 3.14));
        SetPitch((float)(pitch * 180 / 3.14));
    }
};

class EventLogger : public Minecraft::PlayerListener, public Minecraft::ConnectionListener, public Minecraft::Packets::PacketHandler {
private:
    Minecraft::PlayerManager* m_PlayerManager;
    Minecraft::Connection* m_Connection;
    std::ostream& m_Out;

public:
    EventLogger(std::ostream& out, Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::PlayerManager* playerManager, Minecraft::Connection* connection)
        : Minecraft::Packets::PacketHandler(dispatcher),
          m_Out(out),
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
        dispatcher->RegisterHandler(State::Play, Play::MapChunkBulk, this);
        dispatcher->RegisterHandler(State::Play, Play::ChunkData, this);
        dispatcher->RegisterHandler(State::Play, Play::SetSlot, this);
        dispatcher->RegisterHandler(State::Play, Play::WindowItems, this);
        dispatcher->RegisterHandler(State::Play, Play::Statistics, this);
        dispatcher->RegisterHandler(State::Play, Play::PlayerListItem, this);
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
        s32 x = (s32)packet->GetX();
        s32 y = (s32)packet->GetY();
        s32 z = (s32)packet->GetZ();
        Minecraft::Position pos(x, y, z);

        std::cout << "Pos: " << pos << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChunkDataPacket* packet) {
        /*auto column = packet->GetChunkColumn();
        auto x = column->GetMetadata().x;
        auto z = column->GetMetadata().z;

        std::cout << "Received chunk data for chunk " << x << ", " << z << std::endl;*/
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityPropertiesPacket* packet) {
        /*  std::cout << "Received entity properties: " << std::endl;
        const auto& properties = packet->GetProperties();
        for (const auto& kv : properties) {
        std::wstring key = kv.first;
        const auto& property = kv.second;
        std::wcout << key << " : " << property.value << std::endl;
        for (const auto& modifier : property.modifiers)
        std::cout << "Modifier: " << modifier.uuid << " " << modifier.amount << " " << (int)modifier.operation << std::endl;
        }*/
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityRelativeMovePacket* packet) {
        if (packet->GetDeltaX() != 0 || packet->GetDeltaY() != 0 || packet->GetDeltaZ() != 0) {
            //std::cout << "Entity " << packet->GetEntityId() << " relative move: (";
            //std::cout << packet->GetDeltaX() << ", " << packet->GetDeltaY() << ", " << packet->GetDeltaZ() << ")" << std::endl;
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnPlayerPacket* packet) {
        float x, y, z;
        x = packet->GetX();
        y = packet->GetY();
        z = packet->GetZ();
        std::cout << "Spawn player " << packet->GetUUID() << " at (" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::UpdateHealthPacket* packet) {
        std::cout << "Set health. health: " << packet->GetHealth() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetExperiencePacket* packet) {
        std::cout << "Set experience. Level: " << packet->GetLevel() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChatPacket* packet) {
        const Json::Value& root = packet->GetChatData();

        if (root.isString()) {
            std::cout << root.asString() << std::endl;
            return;
        }

        if (root.isNull() || root["text"].isNull()) return;

        std::string message = root["text"].asString();

        if (!root["extra"].isNull()) {
            auto iter = root["extra"].begin();
            for (; iter != root["extra"].end(); ++iter) {
                if ((*iter).isString()) {
                    message += (*iter).asString();
                } else {
                    if (!(*iter)["text"].isNull())
                        message += (*iter)["text"].asString();
                }
            }
        }

        std::cout << message << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityMetadataPacket* packet) {
        //const auto& metadata = packet->GetMetadata();

        //std::cout << "Received entity metadata" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnMobPacket* packet) {
        //const auto& metadata = packet->GetMetadata();

        //std::cout << "Received SpawnMobPacket" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::MapChunkBulkPacket* packet) {
        std::cout << "Received MapChunkBulkPacket" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetSlotPacket* packet) {
        Minecraft::Slot slot = packet->GetSlot();
        int window = packet->GetWindowId();
        int index = packet->GetSlotIndex();

        std::cout << "Set slot (" << window << ", " << index << ") = " << slot.GetItemId() << ":" << slot.GetItemDamage() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::WindowItemsPacket* packet) {
        std::cout << "Received window items for WindowId " << (int)packet->GetWindowId() << "." << std::endl;

        const std::vector<Minecraft::Slot>& slots = packet->GetSlots();

        for (const Minecraft::Slot& slot : slots) {
            s16 id = slot.GetItemId();
            u8 count = slot.GetItemCount();
            s16 dmg = slot.GetItemDamage();
            //const Minecraft::NBT::NBT& nbt = slot.GetNBT();

            if (id != -1) {
                std::cout << "Item: " << id << " Amount: " << (int)count << " Dmg: " << dmg << std::endl;
            }
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::WorldBorderPacket* packet) {
        using namespace Minecraft::Packets::Inbound;
        WorldBorderPacket::Action action = packet->GetAction();

        std::cout << "Received world border packet\n";

        switch (action) {
        case WorldBorderPacket::Action::Initialize:
        {
            std::cout << "World border radius: " << packet->GetRadius() << std::endl;
            std::cout << "World border center: " << packet->GetX() << ", " << packet->GetZ() << std::endl;
            std::cout << "World border warning time: " << packet->GetWarningTime() << " seconds, blocks: " << packet->GetWarningBlocks() << " meters" << std::endl;
        }
        break;
        default:
            break;
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerListItemPacket* packet) {
        using namespace Minecraft::Packets::Inbound;

        PlayerListItemPacket::Action action = packet->GetAction();

        std::vector<PlayerListItemPacket::ActionDataPtr> dataList = packet->GetActionData();

        for (auto actionData : dataList) {
            switch (action) {
            case PlayerListItemPacket::Action::AddPlayer:
                std::wcout << "Adding player " << actionData->name << " uuid: " << actionData->uuid << std::endl;

                for (auto& property : actionData->properties)
                    std::wcout << property.first << " = " << property.second << std::endl;
                break;
            case PlayerListItemPacket::Action::UpdateGamemode:
                std::wcout << "Updating gamemode to " << actionData->gamemode << " for player " << actionData->uuid << std::endl;
                break;
            case PlayerListItemPacket::Action::UpdateLatency:
                //     std::wcout << "Ping for " << actionData->uuid << " is " << actionData->ping << std::endl;
                break;
            case PlayerListItemPacket::Action::RemovePlayer:
                std::cout << "Removing player " << actionData->uuid << std::endl;
                break;
            default:
                break;
            }
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::StatisticsPacket* packet) {
        const auto& stats = packet->GetStatistics();

        for (auto& kv : stats)
            std::wcout << kv.first << " = " << kv.second << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerAbilitiesPacket* packet) {
        std::cout << "Abilities: " << (int)packet->GetFlags() << ", " << packet->GetFlyingSpeed() << ", " << packet->GetWalkingSpeed() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnPositionPacket* packet) {
        std::cout << "Spawn position: " << packet->GetLocation() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::DisconnectPacket* packet) {
        std::cout << "Disconnected from server. Reason: " << std::endl;
        std::wcout << packet->GetReason() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::EncryptionRequestPacket* packet) {
        std::cout << "Encryption request received\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::LoginSuccessPacket* packet) {
        std::cout << "Successfully logged in. Username: ";
        std::wcout << packet->GetUsername() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::JoinGamePacket* packet) {
        std::cout << "Joining game with entity id of " << packet->GetEntityId() << std::endl;
        std::cout << "Game difficulty: " << (int)packet->GetDifficulty() << ", Dimension: " << (int)packet->GetDimension() << std::endl;
        std::wcout << "Level type: " << packet->GetLevelType() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::PluginMessagePacket* packet) {
        std::wcout << "Plugin message received on channel " << packet->GetChannel();
        auto value = packet->GetData();
        std::cout << " Value: " << value << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::ServerDifficultyPacket* packet) {
        std::wcout << "New server difficulty: " << (int)packet->GetDifficulty() << std::endl;
    }
};

class PlayerFollower : public Minecraft::PlayerListener {
private:
    Minecraft::PlayerManager& m_PlayerManager;
    Minecraft::EntityManager& m_EntityManager;
    Minecraft::PlayerPtr m_Following;
    PlayerController& m_PlayerController;
    u64 m_LastUpdate;
    
public:
    PlayerFollower(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::PlayerManager& playerManager, Minecraft::EntityManager& emanager, PlayerController& playerController)
        : m_PlayerManager(playerManager),
          m_EntityManager(emanager),
          m_PlayerController(playerController)
    {
        m_PlayerManager.RegisterListener(this);
    }

    ~PlayerFollower() {
        m_PlayerManager.UnregisterListener(this);
    }

    void UpdatePosition() {
        static const double WalkingSpeed = 4.3; // m/s
        static const double TicksPerSecond = 20;

        u64 dt = GetTime() - m_LastUpdate;

        if (dt < 1000 / TicksPerSecond)
            return;

        m_LastUpdate = GetTime();

        if (!m_Following || !m_Following->GetEntity()) return;

        Vector3d toFollowing = m_Following->GetEntity()->GetPosition() - m_PlayerController.GetPosition();
        double dist = toFollowing.Length();

        if (dist >= 1.0) {
            if (!m_PlayerController.ClearPath(m_Following->GetEntity()->GetPosition()))
                return;

            Vector3d n = Vector3Normalize(toFollowing);
            double change = dt / 1000.0;

            n *= WalkingSpeed * change;
            
            m_PlayerController.Move(n);
        }
    }

    void UpdateRotation() {
        if (!m_Following || !m_Following->GetEntity()) return;

        m_PlayerController.LookAt(m_Following->GetEntity()->GetPosition());
    }

    void FindClosestPlayer() {
        double closest = std::numeric_limits<double>::max();

        m_Following = nullptr;

        for (auto& kv : m_PlayerManager) {
            Minecraft::PlayerPtr player = kv.second;

            auto entity = player->GetEntity();

            if (!entity) continue;

            Minecraft::EntityId peid = m_EntityManager.GetPlayerEntity()->GetEntityId();
            if (entity->GetEntityId() == peid) continue;

            double dist = entity->GetPosition().Distance(m_PlayerController.GetPosition());

            if (dist < closest) {
                closest = dist;
                m_Following = player;
            }
        }

        static u64 lastOutput = 0;

        if (GetTime() - lastOutput >= 3000) {
            lastOutput = GetTime();
            if (m_Following) {
                auto entity = m_Following->GetEntity();
                if (entity) {
                    double dist = m_PlayerController.GetPosition().Distance(entity->GetPosition());
                    std::wcout << L"Tracking " << m_Following->GetName() << " dist: " << dist << std::endl;
                }
            }
        }

        if (!m_Following)
            std::wcout << L"Not tracking anyone" << std::endl;
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

class CreativeCreator : public Minecraft::Packets::PacketHandler {
private:
    Minecraft::Connection* m_Connection;
    s16 m_Slot;
    Minecraft::Slot m_Item;

public:
    CreativeCreator(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::Connection* connection, s16 slot, Minecraft::Slot item)
        : Minecraft::Packets::PacketHandler(dispatcher),
          m_Connection(connection),
          m_Slot(slot), m_Item(item)
    {
        using namespace Minecraft::Protocol;

        dispatcher->RegisterHandler(State::Play, Play::Chat, this);
        dispatcher->RegisterHandler(State::Play, Play::PlayerPositionAndLook, this);
    }

    ~CreativeCreator() {
        GetDispatcher()->UnregisterHandler(this);
    }

    void CreateItem() {
        Minecraft::Packets::Outbound::CreativeInventoryActionPacket packet(m_Slot, m_Item);
        
        m_Connection->SendPacket(&packet);
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        CreateItem();
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChatPacket* packet) {
        const Json::Value& root = packet->GetChatData();

        if (root["text"].isNull()) return;

        std::string message = root["text"].asString();

        if (!root["extra"].isNull()) {
            auto iter = root["extra"].begin();
            for (; iter != root["extra"].end(); ++iter) {
                if ((*iter).isString()) {
                    message += (*iter).asString();
                } else {
                    if (!(*iter)["text"].isNull())
                        message += (*iter)["text"].asString();
                }
            }
        }

        if (message.find("!create") != std::string::npos)
            CreateItem();
    }
};

class Client : public Minecraft::ConnectionListener {
private:
    Minecraft::Packets::PacketDispatcher m_Dispatcher;
    Minecraft::Connection m_Connection;
    Minecraft::EntityManager m_EntityManager;
    Minecraft::PlayerManager m_PlayerManager;
    Minecraft::World m_World;
    PlayerFollower m_Follower;
    PlayerController m_PlayerController;
    bool m_Connected;
    CreativeCreator* m_Creator;
    EventLogger m_Logger;

public:
    Client() 
        : m_Dispatcher(),
          m_Connection(&m_Dispatcher),
          m_EntityManager(&m_Dispatcher),
          m_PlayerManager(&m_Dispatcher, &m_EntityManager),
          m_World(&m_Dispatcher),
          m_Follower(&m_Dispatcher, m_PlayerManager, m_EntityManager, m_PlayerController),
          m_PlayerController(&m_Connection, m_World, m_PlayerManager),
          m_Connected(false),
          m_Creator(nullptr),
          m_Logger(std::cout, &m_Dispatcher, &m_PlayerManager, &m_Connection)
    {
        Minecraft::BlockRegistry::GetInstance().RegisterVanillaBlocks();
        m_Connection.RegisterListener(this);

        using namespace Minecraft::NBT;

       /* 
       
       // Test code to create skulls with enchants/attributes
       NBT nbt;

        Minecraft::Yggdrasil profileService;
        
        Minecraft::UUID uuid;
        std::string playerName = "MHF_Chest";
        std::string textureValue;

        try {
            uuid = profileService.GetPlayerUUID(playerName);
            Json::Value profile = profileService.GetPlayerProfile(uuid);

            if (profile["properties"].isArray()) {
                Json::Value texture = profile["properties"][0];
                if (!texture.isNull())
                    textureValue = texture["value"].asString();
            }

        } catch (Minecraft::YggdrasilException& e) {
            std::cerr << e.what() << std::endl;
            return;
        }
        
        if (textureValue.length() == 0) {
            std::cerr << "Failed to fetch texture" << std::endl;
            return;
        }
       
        TagCompound* skullOwner = new TagCompound(L"SkullOwner");
        TagPtr name(new TagString("Name", playerName));
        TagPtr id(new TagString("Id", uuid.ToString()));
        TagCompound* properties = new TagCompound(L"Properties");
        TagList* textures = new TagList(L"Textures", TagType::Compound);
        TagCompound* textureCompound = new TagCompound(L"");
        TagPtr value(new TagString("Value", textureValue));
        textureCompound->AddItem(value);

        textures->AddItem(TagPtr(textureCompound));
        properties->AddItem(TagPtr(textures));
        skullOwner->AddItem(TagPtr(properties));
        skullOwner->AddItem(name);
        skullOwner->AddItem(id);

        nbt.GetRoot().AddItem(TagPtr(skullOwner));

        TagList* ench = new TagList("ench", TagType::Compound);
        {
            TagCompound* enchCompound = new TagCompound(L"");
            TagPtr enchId(new TagShort("id", 61));
            TagPtr enchLvl(new TagShort("lvl", 10));

            enchCompound->AddItem(enchId);
            enchCompound->AddItem(enchLvl);
            ench->AddItem(TagPtr(enchCompound));
        }
        {
            TagCompound* enchCompound = new TagCompound(L"");
            TagPtr enchId(new TagShort("id", 62));
            TagPtr enchLvl(new TagShort("lvl", 10));

            enchCompound->AddItem(enchId);
            enchCompound->AddItem(enchLvl);
            ench->AddItem(TagPtr(enchCompound));
        }

        nbt.GetRoot().AddItem(TagPtr(ench));

        TagList* attr = new TagList("AttributeModifiers", TagType::Compound);
        TagCompound* attrCompound = new TagCompound(L"");
        TagPtr attrAttrName(new TagString("AttributeName", "generic.movementSpeed"));
        TagPtr attrName(new TagString("Name", "Test"));
        TagPtr attrSlot(new TagString("Slot", "head"));
        TagPtr attrOperation(new TagInt("Operation", 2));
        TagPtr attrAmount(new TagDouble("Amount", 10.0));
        TagPtr attrMost(new TagLong("UUIDMost", 7361814797886573596));
        TagPtr attrLeast(new TagLong("UUIDLeast", 9805346747102734221));

        attr->AddItem(TagPtr(attrCompound));
        attrCompound->AddItem(attrAttrName);
        attrCompound->AddItem(attrName);
        attrCompound->AddItem(attrSlot);
        attrCompound->AddItem(attrOperation);
        attrCompound->AddItem(attrAmount);
        attrCompound->AddItem(attrMost);
        attrCompound->AddItem(attrLeast);

        nbt.GetRoot().AddItem(TagPtr(attr));
        nbt.GetRoot().SetName(L"tag");

        Minecraft::Slot slot(397, 1, 3, nbt);
        //Minecraft::Slot slot(346, 1, 0, nbt);
        //Minecraft::Slot slot(349, 1, 2, nbt);

        m_Creator = new CreativeCreator(&m_Dispatcher, &m_Connection, 15, slot);
        std::cout << "Created skull creator" << std::endl;*/
    }

    ~Client() {
        m_Connection.UnregisterListener(this);
    }

    void OnSocketStateChange(Network::Socket::Status newState) {
        m_Connected = (newState == Network::Socket::Status::Connected);
    }

    int Run() {
        std::string host = "192.168.2.88";

        if (!m_Connection.Connect(host, 25565)) {
            std::cerr << "Failed to connect to server " << host << std::endl;
            return -1;
        }

        m_Connection.Login("testplayer", "");

        while (m_Connected) {
            m_Connection.CreatePacket();
            /*try {
                m_Connection.CreatePacket();
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }*/

            m_Follower.UpdatePosition();
            m_PlayerController.Update();
        }

        std::cout << "Disconnected from server." << std::endl;

        return 0;
    }
};

int main(void) {
    Client client;

    return client.Run();
}
