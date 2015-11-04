#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <array>
#include <vector>
#include <json/json.h>
#include "HTTPClient.h"
#include "Hash.h"
#include "Yggdrasil.h"
#include "Network/Network.h"
#include "DataBuffer.h"
#include "Encryption.h"
#include "Packets/Packet.h"
#include "Packets/PacketHandler.h"
#include "Packets/PacketDispatcher.h"
#include "Packets/PacketFactory.h"
#include "Protocol.h"
#include "Compression.h"
#include "World.h"
#include "PlayerManager.h"
#include "Entity/EntityManager.h"
#include <memory>
#include <regex>
#include <chrono>

#ifdef max
#undef max
#endif

/** This is all junk code just to get things working */

// TODO: Switch to using vectors in packets instead of individual variables
// TODO: Maybe redesign the way the protocol is setup to allow for different versions

class ConnectionListener {
public:
    virtual ~ConnectionListener() { }

    virtual void OnSocketStateChange(Network::Socket::Status newStatus) { }
};

class Connection : public Minecraft::Packets::PacketHandler, public ObserverSubject<ConnectionListener> {
private:
    Minecraft::EncryptionStrategy* m_Encrypter;
    Minecraft::CompressionStrategy* m_Compressor;
    Minecraft::Protocol::State m_ProtocolState;
    std::shared_ptr<Network::Socket> m_Socket;
    Minecraft::Yggdrasil m_Yggdrasil;
    std::string m_Server;
    u16 m_Port;
    std::string m_Username;
    std::string m_Password;
    Minecraft::DataBuffer m_HandleBuffer;

public:
    Connection(Minecraft::Packets::PacketDispatcher* dispatcher)
        : Minecraft::Packets::PacketHandler(dispatcher), 
          m_Encrypter(new Minecraft::EncryptionStrategyNone()), 
          m_Compressor(new Minecraft::CompressionNone()),
          m_Socket(new Network::TCPSocket())
    {
        using namespace Minecraft;

        dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::Disconnect, this);
        dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::EncryptionRequest, this);
        dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::LoginSuccess, this);
        dispatcher->RegisterHandler(Protocol::State::Login, Protocol::Login::SetCompression, this);

        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::KeepAlive, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::JoinGame, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Chat, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnPosition, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::UpdateHealth, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerPositionAndLook, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnPlayer, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::SpawnMob, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityRelativeMove, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityMetadata, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::SetExperience, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::EntityProperties, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::MapChunkBulk, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::ChunkData, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::SetSlot, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::WindowItems, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Statistics, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerListItem, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerAbilities, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PluginMessage, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Disconnect, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::ServerDifficulty, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::WorldBorder, this);

        m_Socket->SetBlocking(false);
    }

    ~Connection() {
        GetDispatcher()->UnregisterHandler(this);
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChunkDataPacket* packet) {
        auto column = packet->GetChunkColumn();
        auto x = column->GetMetadata().x;
        auto z = column->GetMetadata().z;

        std::cout << "Received chunk data for chunk " << x << ", " << z << std::endl;
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

        Json::StyledWriter writer;
        std::string out = writer.write(root);
        std::cout << out << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityMetadataPacket* packet) {
        const auto& metadata = packet->GetMetadata();

        //std::cout << "Received entity metadata" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnMobPacket* packet) {
        const auto& metadata = packet->GetMetadata();
        
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
            const Minecraft::NBT::NBT& nbt = slot.GetNBT();

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
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::KeepAlivePacket* packet) {
        Minecraft::Packets::Outbound::KeepAlivePacket response(packet->GetAliveId());
        SendPacket(&response);
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet) {
        s32 x = (s32)packet->GetX();
        s32 y = (s32)packet->GetY(); 
        s32 z = (s32)packet->GetZ();
        Minecraft::Position pos(x, y , z);

        std::cout << "Pos: " << pos << std::endl;

        // I guess the client has to figure this out from chunk data?
        bool onGround = true;

        using namespace Minecraft::Packets;

        // Used to verify position
        Outbound::PlayerPositionAndLookPacket response(packet->GetX(), packet->GetY(), packet->GetZ(),
            packet->GetYaw(), packet->GetPitch(), onGround);

        SendPacket(&response);

        Outbound::ClientStatusPacket status(Outbound::ClientStatusPacket::Action::PerformRespawn);
        SendPacket(&status);
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

        m_Socket->Disconnect();

        NotifyListeners(&ConnectionListener::OnSocketStateChange, m_Socket->GetStatus());
    }

    void AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey) {
        std::cout << "Doing client auth\n";
        
        try {
            if (!m_Yggdrasil.Authenticate(m_Username, m_Password)) {
                std::cerr << "Failed to authenticate" << std::endl;
                std::abort();
            }
        } catch (const Minecraft::YggdrasilException& e) {
            std::cerr << e.what() << std::endl;
            std::abort();
        }

        try {
            if (!m_Yggdrasil.JoinServer(serverId, sharedSecret, pubkey)) {
                std::cerr << "Failed to join server through Yggdrasil." << std::endl;
                std::abort();
            }
        } catch (const Minecraft::YggdrasilException& e) {
            std::cerr << e.what() << std::endl;
            std::abort();
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::EncryptionRequestPacket* packet) {
        std::string pubkey = packet->GetPublicKey();
        std::string verify = packet->GetVerifyToken();

        std::cout << "Encryption request received\n";

        Minecraft::EncryptionStrategyAES* aesEncrypter = new Minecraft::EncryptionStrategyAES(pubkey, verify);
        Minecraft::Packets::Outbound::EncryptionResponsePacket* encResp = aesEncrypter->GenerateResponsePacket();

        AuthenticateClient(packet->GetServerId().GetUTF16(), aesEncrypter->GetSharedSecret(), pubkey);

        SendPacket(encResp);

        delete m_Encrypter;
        m_Encrypter = aesEncrypter;
    }

    void HandlePacket(Minecraft::Packets::Inbound::LoginSuccessPacket* packet) {
        std::cout << "Successfully logged in. Username: ";
        std::wcout << packet->GetUsername() << std::endl;

        m_ProtocolState = Minecraft::Protocol::State::Play;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetCompressionPacket* packet) {
        delete m_Compressor;
        m_Compressor = new Minecraft::CompressionZ(packet->GetMaxPacketSize());
    }

    void HandlePacket(Minecraft::Packets::Inbound::JoinGamePacket* packet) {
        std::cout << "Joining game with entity id of " << packet->GetEntityId() << std::endl;
        std::cout << "Game difficulty: " << (int)packet->GetDifficulty() << ", Dimension: " << (int)packet->GetDimension() << std::endl;
        std::wcout << "Level type: " << packet->GetLevelType() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::PluginMessagePacket* packet) {
        std::wcout << "Plugin message received on channel " << packet->GetChannel() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::ServerDifficultyPacket* packet) {
        std::wcout << "New server difficulty: " << (int)packet->GetDifficulty() << std::endl;
    }

    bool Connect(const std::string& server, u16 port) {
        bool result = false;

        m_Server = server;
        m_Port = port;

        if (isdigit(m_Server.at(0))) {
            Network::IPAddress addr(m_Server);

            result = m_Socket->Connect(addr, m_Port);
        } else {
            std::cout << "Resolving server dns\n";
            auto addrs = Network::Dns::Resolve(m_Server);
            std::cout << "Finished resolving: " << addrs.size() << std::endl;
            if (addrs.size() == 0) return false;

            for (auto addr : addrs) {
                std::cout << "Trying to connect to " << addr << std::endl;
                if (m_Socket->Connect(addr, m_Port)) {
                    result = true;
                    break;
                }
            }
        }

        if (result)
            NotifyListeners(&ConnectionListener::OnSocketStateChange, m_Socket->GetStatus());

        return result;
    }

    Minecraft::Packets::Packet* CreatePacket(Minecraft::DataBuffer& buffer) {
        std::size_t readOffset = buffer.GetReadOffset();
        Minecraft::VarInt length;
        buffer >> length;

        if (buffer.GetRemaining() < (u32)length.GetInt()) {
            buffer.SetReadOffset(readOffset);
            return nullptr;
        }

        Minecraft::DataBuffer decompressed = m_Compressor->Decompress(buffer, length.GetInt());
        
        return Minecraft::Packets::PacketFactory::CreatePacket(m_ProtocolState, decompressed, length.GetInt());
    }

    void CreatePacket() {
        Minecraft::DataBuffer buffer = m_Socket->Receive(4096);

        if (m_Socket->GetStatus() != Network::Socket::Connected) {
            NotifyListeners(&ConnectionListener::OnSocketStateChange, m_Socket->GetStatus());
            return;
        }

        m_HandleBuffer << m_Encrypter->Decrypt(buffer);

        Minecraft::Packets::Packet* packet = nullptr;

        do {
            try {
                packet = CreatePacket(m_HandleBuffer);
                if (packet) {
                    this->GetDispatcher()->Dispatch(packet);
                    Minecraft::Packets::PacketFactory::FreePacket(packet);
                } else {
                    break;
                }
            } catch (const std::exception&) {
                //std::cerr << e.what() << std::endl;
                // Temporary until protocol is finished
            }
        } while (!m_HandleBuffer.IsFinished() && m_HandleBuffer.GetSize() > 0);

        if (m_HandleBuffer.IsFinished())
            m_HandleBuffer = Minecraft::DataBuffer();
        else if (m_HandleBuffer.GetReadOffset() != 0)
            m_HandleBuffer = Minecraft::DataBuffer(m_HandleBuffer, m_HandleBuffer.GetReadOffset());
    }

    void Login(const std::string& username, const std::string& password) {
        Minecraft::Packets::Outbound::HandshakePacket handshake(47, m_Server, m_Port, Minecraft::Protocol::State::Login);
        SendPacket(&handshake);

        Minecraft::Packets::Outbound::LoginStartPacket loginStart(username);
        SendPacket(&loginStart);

        m_Username = username;
        m_Password = password;

        m_ProtocolState = Minecraft::Protocol::State::Login;
    }

    void SendPacket(Minecraft::Packets::Packet* packet) {
        Minecraft::DataBuffer packetBuffer = packet->Serialize();
        Minecraft::DataBuffer compressed = m_Compressor->Compress(packetBuffer);
        Minecraft::DataBuffer encrypted = m_Encrypter->Encrypt(compressed);

        m_Socket->Send(encrypted);
    }
};

s64 GetTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

class PlayerController : public Minecraft::PlayerListener {
private:
    Minecraft::PlayerManager& m_PlayerManager;
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;
    Connection* m_Connection;
    Minecraft::World& m_World;

public:
    PlayerController(Connection* connection, Minecraft::World& world, Minecraft::PlayerManager& playerManager)
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

    bool IsSolidBlock(u16 type) const {
        return type != 0 && type != 78;
    }

    bool ClearPath(Vector3d target) {
        double dist = target.Distance(m_Position);

        Vector3d toTarget = target - m_Position;
        Vector3d n = Vector3Normalize(toTarget);

        Vector3d side = n.Cross(Vector3d(0, 1, 0));

        const double CheckWidth = 0.3;

        Vector3d right = m_Position + side * CheckWidth;
        Vector3d left = m_Position - side * CheckWidth;

        auto check = [&](Vector3d start, Vector3d target) -> bool {
            Vector3d toTarget = target - start;
            Vector3d n = Vector3Normalize(toTarget);

            for (s32 i = 0; i < (int)std::ceil(dist); ++i) {
                Vector3d delta(i * n.x, 0, i * n.z);
                Vector3d checkAbove = start + delta + Vector3d(0, 1, 0);
                Vector3d checkBelow = start + delta + Vector3d(0, 0, 0);
                Vector3d checkFloor = start + delta + Vector3d(0, -1, 0);

                Minecraft::BlockPtr aboveBlock = m_World.GetBlock(checkAbove);
                Minecraft::BlockPtr belowBlock = m_World.GetBlock(checkBelow);
                Minecraft::BlockPtr floorBlock = m_World.GetBlock(checkFloor);

                if (!aboveBlock || IsSolidBlock(aboveBlock->GetType()))
                    return false;
                if (!belowBlock || IsSolidBlock(belowBlock->GetType()))
                    return false;
                if (!floorBlock || !IsSolidBlock(floorBlock->GetType()))
                    return false;
            }
            return true;
        };

        if (!check(right, target + side * CheckWidth)) return false;
        if (!check(left, target - side * CheckWidth)) return false;
        
        return true;
    }

    void OnClientSpawn(Minecraft::PlayerPtr player) {
        m_Yaw = player->GetEntity()->GetYaw();
        m_Pitch = player->GetEntity()->GetPitch();
        m_Position = player->GetEntity()->GetPosition();
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

    void Update() {
        if (m_Position == Vector3d(0, 0, 0)) return;
        static s64 lastSend = 0;
        static s64 lastPosOutput = 0;

        if (GetTime() - lastSend >= 50) {
            bool onGround = true;

            const float CheckWidth = 0.4f;

            std::vector<Vector3d> belowchecks = {
                Vector3d(m_Position.x + CheckWidth, m_Position.y - 1, m_Position.z),
                Vector3d(m_Position.x - CheckWidth, m_Position.y - 1, m_Position.z),

                Vector3d(m_Position.x, m_Position.y - 1, m_Position.z + CheckWidth),
                Vector3d(m_Position.x, m_Position.y - 1, m_Position.z - CheckWidth),

                Vector3d(m_Position.x + CheckWidth, m_Position.y - 1, m_Position.z + CheckWidth),
                Vector3d(m_Position.x - CheckWidth, m_Position.y - 1, m_Position.z - CheckWidth),

                Vector3d(m_Position.x + CheckWidth, m_Position.y - 1, m_Position.z - CheckWidth),
                Vector3d(m_Position.x - CheckWidth, m_Position.y - 1, m_Position.z + CheckWidth),

            };

            bool fall = true;
            for (u32 i = 0; i < belowchecks.size(); ++i) {
                Minecraft::BlockPtr check = m_World.GetBlock(belowchecks[i]);

                if (!check || (check && check->GetType() != 0)) {
                    fall = false;
                    break;
                }
            }

            if (fall) {
                std::cout << "Falling\n";
                m_Position.y--;
                onGround = false;
            }

            if (GetTime() - lastPosOutput >= 2000) {
                Minecraft::BlockPtr below = m_World.GetBlock(m_Position - Vector3d(0, 1.0, 0));
                //  if (below)
                  //  std::cout << "Standing on " << below->GetType() << std::endl;

                lastPosOutput = GetTime();
            }

            Minecraft::Packets::Outbound::PlayerPositionAndLookPacket response(m_Position.x, m_Position.y, m_Position.z,
                m_Yaw, m_Pitch, onGround);

            m_Connection->SendPacket(&response);

            lastSend = GetTime();
        }
    }

    Vector3d GetPosition() const { return m_Position; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }

    void Move(Vector3d delta) { m_Position += delta; }
    void SetYaw(float yaw) { m_Yaw = yaw; }
    void SetPitch(float pitch) { m_Pitch = pitch; }
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
        double dist = std::sqrt(toFollowing.x * toFollowing.x + toFollowing.z * toFollowing.z);

        if (toFollowing.y < .1) {
            if (!m_PlayerController.ClearPath(m_Following->GetEntity()->GetPosition()))
                return;

            if (dist >= 1.0) {
                Vector3d n = Vector3Normalize(toFollowing);
                double change = dt / 1000.0;

                n *= WalkingSpeed * change;
                m_PlayerController.Move(n);
            } else {
            //    m_PlayerController.Attack(m_Following->GetEntity()->GetEntityId());
            }
        }
    }

    void UpdateRotation() {
        if (!m_Following || !m_Following->GetEntity()) return;

        Vector3d toFollowing = m_Following->GetEntity()->GetPosition() - m_PlayerController.GetPosition();

        double dist = std::sqrt(toFollowing.x * toFollowing.x + toFollowing.z * toFollowing.z);
        double pitch = -std::atan2(toFollowing.y, dist);
        double yaw = -std::atan2(toFollowing.x, toFollowing.z);

        m_PlayerController.SetYaw((float)(yaw * 180 / 3.14));
        m_PlayerController.SetPitch((float)(pitch * 180 / 3.14));
    }

    void FindClosestPlayer() {
        double closest = std::numeric_limits<double>::max();

        m_Following = nullptr;

        for (auto& kv : m_PlayerManager) {
            Minecraft::PlayerPtr player = kv.second;

            auto entity = player->GetEntity();

            if (!entity) continue;

            Minecraft::EntityId peid = m_EntityManager.GetPlayerEntity().lock()->GetEntityId();
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
            } else {
                std::wcout << L"Not tracking anyone" << std::endl;
            }
        }
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

class Client : public ConnectionListener {
private:
    Minecraft::Packets::PacketDispatcher m_Dispatcher;
    Connection m_Connection;
    Minecraft::EntityManager m_EntityManager;
    Minecraft::PlayerManager m_PlayerManager;
    Minecraft::World m_World;
    PlayerFollower m_Follower;
    PlayerController m_PlayerController;
    bool m_Connected;

public:
    Client() 
        : m_Dispatcher(),
          m_Connection(&m_Dispatcher),
          m_EntityManager(&m_Dispatcher),
          m_PlayerManager(&m_Dispatcher, &m_EntityManager),
          m_World(&m_Dispatcher),
          m_Follower(&m_Dispatcher, m_PlayerManager, m_EntityManager, m_PlayerController),
          m_PlayerController(&m_Connection, m_World, m_PlayerManager),
          m_Connected(false)
    {
        m_Connection.RegisterListener(this);
    }

    ~Client() {
        m_Connection.UnregisterListener(this);
    }

    void OnSocketStateChange(Network::Socket::Status newState) {
        m_Connected = (newState == Network::Socket::Status::Connected);
    }

    int Run() {
        std::string host = "192.168.2.88";
        //std::string host = "play.mysticempire.net";

        if (!m_Connection.Connect(host, 25565)) {
            std::cerr << "Failed to connect to server " << host << std::endl;
            return -1;
        }

        m_Connection.Login("testplayer", "");

        while (m_Connected) {
            m_Connection.CreatePacket();

            m_Follower.UpdatePosition();
            m_PlayerController.Update();
        }

        return 0;
    }
};

int main(void) {
    Client client;

    return client.Run();
}
