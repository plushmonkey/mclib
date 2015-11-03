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

std::string PlayerName = "testplayer";
std::string PlayerPassword = "";

class Connection : public Minecraft::Packets::PacketHandler {
private:
    Minecraft::EncryptionStrategy* m_Encrypter;
    Minecraft::CompressionStrategy* m_Compressor;
    Minecraft::Protocol::State m_ProtocolState;
    std::shared_ptr<Network::Socket> m_Socket;
    Minecraft::Yggdrasil m_Yggdrasil;
    std::string m_Server;
    u16 m_Port;
    Minecraft::World m_World;

public:
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;

    Connection(const std::string& server, u16 port, Minecraft::Packets::PacketDispatcher* dispatcher)
        : Minecraft::Packets::PacketHandler(dispatcher), 
          m_Server(server), 
          m_Port(port), 
          m_Encrypter(new Minecraft::EncryptionStrategyNone()), 
          m_Compressor(new Minecraft::CompressionNone()),
          m_Socket(new Network::TCPSocket()), 
          m_World(dispatcher),
          m_Position(0, 0, 0)
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
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::SetSlot, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::WindowItems, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Statistics, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerListItem, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PlayerAbilities, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::PluginMessage, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::Disconnect, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::ServerDifficulty, this);
        dispatcher->RegisterHandler(Protocol::State::Play, Protocol::Play::WorldBorder, this);
    }

    ~Connection() {
        GetDispatcher()->UnregisterHandler(this);
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
        Send(&response);
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

        Send(&response);

        Outbound::ClientStatusPacket status(Outbound::ClientStatusPacket::Action::PerformRespawn);
        Send(&status);

        m_Position = Vector3d(packet->GetX(), packet->GetY(), packet->GetZ());
        m_Yaw = packet->GetYaw();
        m_Pitch = packet->GetPitch();
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
        std::abort();
    }

    void AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey) {
        std::cout << "Doing client auth\n";
        
        try {
            if (!m_Yggdrasil.Authenticate(PlayerName, PlayerPassword)) {
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

        Send(encResp);

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

    bool Connect() {
        if (isdigit(m_Server.at(0))) {
            Network::IPAddress addr(m_Server);

            return m_Socket->Connect(addr, m_Port);
        } else {
            std::cout << "Resolving server dns\n";
            auto addrs = Network::Dns::Resolve(m_Server);
            std::cout << "Finished resolving: " << addrs.size() << std::endl;
            if (addrs.size() == 0) return false;

            for (auto addr : addrs) {
                std::cout << "Trying to connect to " << addr << std::endl;
                if (m_Socket->Connect(addr, m_Port))
                    return true;
            }
        }

        return false;
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

    s64 GetTime() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void UpdatePosition() {
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
                //m_Position.y--;
                //onGround = false;
            }

            if (GetTime() - lastPosOutput >= 2000) {
                Minecraft::BlockPtr below = m_World.GetBlock(m_Position - Vector3d(0, 1.0, 0));
              //  if (below)
                //    std::cout << "Standing on " << below->GetType() << std::endl;

                lastPosOutput = GetTime();
            }

            Minecraft::Packets::Outbound::PlayerPositionAndLookPacket response(m_Position.x, m_Position.y, m_Position.z,
                m_Yaw, m_Pitch, onGround);

            Send(&response);

            lastSend = GetTime();
        }
    }

    void Run() {
        m_Socket->SetBlocking(false);

        if (!Connect()) {
            std::cerr << "Couldn't connect\n";
            return;
        }

        Minecraft::Packets::Outbound::HandshakePacket handshake(47, m_Server, m_Port, Minecraft::Protocol::State::Login);
        Send(&handshake);

        Minecraft::Packets::Outbound::LoginStartPacket loginStart(PlayerName);
        Send(&loginStart);

        m_ProtocolState = Minecraft::Protocol::State::Login;

        Minecraft::DataBuffer toHandle;

        while (true) {
            Minecraft::DataBuffer buffer = m_Socket->Receive(4096);

            if (m_Socket->GetStatus() != Network::Socket::Connected) break;

            UpdatePosition();

            toHandle << m_Encrypter->Decrypt(buffer);

            Minecraft::Packets::Packet* packet = nullptr;

            do {
                try {
                    packet = CreatePacket(toHandle);
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
            } while (!toHandle.IsFinished() && toHandle.GetSize() > 0);

            if (toHandle.IsFinished())
                toHandle = Minecraft::DataBuffer();
            else if (toHandle.GetReadOffset() != 0)
                toHandle = Minecraft::DataBuffer(toHandle, toHandle.GetReadOffset());
        }
    }

    void Send(Minecraft::Packets::Packet* packet) {
        Minecraft::DataBuffer packetBuffer = packet->Serialize();
        Minecraft::DataBuffer compressed = m_Compressor->Compress(packetBuffer);
        Minecraft::DataBuffer encrypted = m_Encrypter->Encrypt(compressed);

        m_Socket->Send(encrypted);
    }
};

#include <cassert>

class PlayerFollower : public Minecraft::PlayerListener {
private:
    Connection* m_Connection;
    Minecraft::PlayerManager& m_PlayerManager;
    Minecraft::EntityManager& m_EntityManager;
    Minecraft::PlayerPtr m_Following;
    
public:
    PlayerFollower(Minecraft::Packets::PacketDispatcher* dispatcher, Connection* connection, Minecraft::PlayerManager& playerManager, Minecraft::EntityManager& emanager)
        : m_Connection(connection),
          m_PlayerManager(playerManager),
          m_EntityManager(emanager)
    {
        m_PlayerManager.RegisterListener(this);
    }

    ~PlayerFollower() {
        m_PlayerManager.UnregisterListener(this);
    }

    void UpdateRotation() {
        FindClosestPlayer();

        if (!m_Following || !m_Following->GetEntity()) return;

        Vector3d toFollowing = m_Following->GetEntity()->GetPosition() - m_Connection->m_Position;

        double dist = std::sqrt(toFollowing.x * toFollowing.x + toFollowing.z * toFollowing.z);
        double pitch = -std::atan2(toFollowing.y, dist);
        double yaw = -std::atan2(toFollowing.x, toFollowing.z);

        m_Connection->m_Yaw = (float)(yaw * 180 / 3.14);
        m_Connection->m_Pitch = (float)(pitch * 180 / 3.14);
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

            double dist = entity->GetPosition().Distance(m_Connection->m_Position);

            if (dist < closest) {
                closest = dist;
                m_Following = player;
            }
        }

        if (m_Following) {
            auto entity = m_Following->GetEntity();
            if (entity) {
                double dist = m_Connection->m_Position.Distance(entity->GetPosition());
                std::wcout << L"Tracking " << m_Following->GetName() << " dist: " << dist << std::endl;
            }
        } else {
            std::wcout << L"Not tracking anyone" << std::endl;
        }
    }

    void OnPlayerJoin(Minecraft::PlayerPtr player) {
        UpdateRotation();
    }

    void OnPlayerLeave(Minecraft::PlayerPtr player) {
        UpdateRotation();
    }

    void OnPlayerSpawn(Minecraft::PlayerPtr player) {
        UpdateRotation();
    }

    void OnPlayerDestroy(Minecraft::PlayerPtr player, Minecraft::EntityId eid) {
        auto playerEntity = player->GetEntity();

        assert(playerEntity == nullptr);

        Minecraft::EntityPtr entity = m_EntityManager.GetEntity(eid);

        assert(entity != nullptr);

        UpdateRotation();
    }

    void OnPlayerMove(Minecraft::PlayerPtr player, Vector3d oldPos, Vector3d newPos) {
        UpdateRotation();
    }
};

class Client {
private:
    Minecraft::Packets::PacketDispatcher m_Dispatcher;
    Connection m_Connection;
    Minecraft::EntityManager m_EntityManager;
    Minecraft::PlayerManager m_PlayerManager;
    PlayerFollower m_Follower;

public:
    Client() 
        : m_Dispatcher(),
          //m_Connection("play.mysticempire.net", 25565, &m_Dispatcher),
          m_Connection("192.168.2.3", 25565, &m_Dispatcher),
          m_EntityManager(&m_Dispatcher),
          m_PlayerManager(&m_Dispatcher, &m_EntityManager),
          m_Follower(&m_Dispatcher, &m_Connection, m_PlayerManager, m_EntityManager)
    {

    }

    void Run() {
        m_Connection.Run();
    }
};

int main(void) {
    Client client;

    client.Run();

    return 0;
}
