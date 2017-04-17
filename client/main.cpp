#include <mclib/common/Common.h>
#include <mclib/core/Client.h>
#include <mclib/util/Forge.h>
#include <mclib/util/Hash.h>
#include <mclib/util/Utility.h>

#include <thread>
#include <iostream>
#include <chrono>
#include <fstream>

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
#pragma comment(lib, "../lib/jsoncpp/lib/jsoncppd-msvc-2017.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
#pragma comment(lib, "../lib/jsoncpp/lib/jsoncpp-msvc-2017.lib")
#endif

s64 GetTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

enum class Face {
    Bottom = 0,
    Top,
    North,
    South,
    West,
    East
};

using mc::Vector3i;
using mc::Vector3d;

class BlockPlacer : public mc::protocol::packets::PacketHandler, public mc::world::WorldListener, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    mc::util::PlayerController* m_PlayerController;
    mc::world::World* m_World;
    Vector3i m_Target;
    s64 m_LastUpdate;
    mc::inventory::Slot m_HeldItem;

public:
    BlockPlacer(mc::protocol::packets::PacketDispatcher* dispatcher, mc::core::Client* client, mc::util::PlayerController* pc, mc::world::World* world) 
        : mc::protocol::packets::PacketHandler(dispatcher), m_Client(client), m_PlayerController(pc), m_World(world), m_LastUpdate(GetTime())
    {
        m_Target = mc::Vector3i(-2, 62, 275);
        world->RegisterListener(this);
        client->RegisterListener(this);

        using namespace mc::protocol;
        dispatcher->RegisterHandler(State::Play, play::WindowItems, this);
        dispatcher->RegisterHandler(State::Play, play::SetSlot, this);
    }

    ~BlockPlacer() {
        GetDispatcher()->UnregisterHandler(this);
        m_World->UnregisterListener(this);
        m_Client->UnregisterListener(this);
    }

    void HandlePacket(mc::protocol::packets::in::WindowItemsPacket* packet) {
        auto slots = packet->GetSlots();
        m_HeldItem = slots[36];
    }

    void HandlePacket(mc::protocol::packets::in::SetSlotPacket* packet) {
        if (packet->GetWindowId() != 0) return;

        if (packet->GetSlotIndex() == 36)
            m_HeldItem = packet->GetSlot();
    }

    void OnTick() {
        s64 time = GetTime();
        if (time - m_LastUpdate < 5000) return;
        m_LastUpdate = time;

        if (m_PlayerController->GetPosition() == Vector3d(0, 0, 0)) return;
        if (!m_World->GetChunk(m_Target)) return;

        m_PlayerController->LookAt(ToVector3d(m_Target));

        if (m_HeldItem.GetItemId() != -1) {
            mc::block::BlockPtr block = m_World->GetBlock(m_Target + Vector3i(0, 1, 0)).GetBlock();
            
            if (!block || block->GetType() == 0) {
                mc::protocol::packets::out::PlayerBlockPlacementPacket blockPlacePacket(m_Target, (u8)Face::Top, mc::Hand::Main, mc::Vector3f(0.5, 0, 0.5));

                m_Client->GetConnection()->SendPacket(&blockPlacePacket);
                std::wcout << "Placing block" << std::endl;
            } else {
                using namespace mc::protocol::packets::out;
                {
                    PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::StartedDigging;
                    PlayerDiggingPacket packet(status, m_Target + Vector3i(0, 1, 0), (u8)Face::West);

                    m_Client->GetConnection()->SendPacket(&packet);
                }

                std::wcout << "Destroying block" << std::endl;

                {
                    PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::FinishedDigging;
                    PlayerDiggingPacket packet(status, m_Target + Vector3i(0, 1, 0), (u8)Face::West);

                    m_Client->GetConnection()->SendPacket(&packet);
                }
            }
        }
    }
};

class TextureGrabber : public mc::protocol::packets::PacketHandler {
private:
    bool ContainsTextureURL(const Json::Value& root) {
        if (!root.isMember("textures")) return false;
        if (!root["textures"].isMember("SKIN")) return false;
        return root["textures"]["SKIN"].isMember("url");
    }

public:
    TextureGrabber(mc::protocol::packets::PacketDispatcher* dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher)
    {
        using namespace mc::protocol;

        dispatcher->RegisterHandler(State::Play, play::PlayerListItem, this);
    }

    ~TextureGrabber() {
        GetDispatcher()->UnregisterHandler(this);
    }

    void HandlePacket(mc::protocol::packets::in::PlayerListItemPacket* packet) {
        using namespace mc::protocol::packets::in;

        PlayerListItemPacket::Action action = packet->GetAction();

        if (action == PlayerListItemPacket::Action::AddPlayer) {
            auto actionDataList = packet->GetActionData();

            for (auto actionData : actionDataList) {
                auto properties = actionData->properties;

                auto iter = properties.find(L"textures");

                if (iter == properties.end()) continue;

                std::wstring encoded = iter->second;
                std::string decoded = mc::util::Base64Decode(std::string(encoded.begin(), encoded.end()));

                Json::Value root;
                Json::Reader reader;

                std::wstring name = actionData->name;

                if (!reader.parse(decoded, root)) {
                    std::wcerr << L"Failed to parse decoded data for " << name;
                    continue;
                }

                if (!ContainsTextureURL(root)) {
                    std::wcerr << L"No texture found for " << name;
                    continue;
                }

                std::string url = root["textures"]["SKIN"]["url"].asString();

                std::wcout << L"Fetching skin for " << name << std::endl;

                mc::util::CurlHTTPClient http;

                mc::util::HTTPResponse resp = http.Get(url);

                if (resp.status == 200) {
                    std::wcout << L"Saving texture for " << name << std::endl;

                    std::string body = resp.body;

                    std::string filename = std::string(name.begin(), name.end()) + ".png";
                    std::ofstream out(filename, std::ios::out | std::ios::binary);

                    out.write(body.c_str(), body.size());
                }
            }
        }
    }
};

class SneakEnforcer : public mc::core::PlayerListener, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    mc::core::PlayerManager* m_PlayerManager;
    mc::core::Connection* m_Connection;
    s64 m_StartTime;

public:
    SneakEnforcer(mc::core::Client* client) 
        : m_Client(client), 
          m_PlayerManager(client->GetPlayerManager()), 
          m_Connection(client->GetConnection()),
          m_StartTime(GetTime())
    {
        m_PlayerManager->RegisterListener(this);
        m_Client->RegisterListener(this);
    }

    ~SneakEnforcer() {
        m_PlayerManager->UnregisterListener(this);
        m_Client->UnregisterListener(this);
    }

    void OnTick() override {
        s64 ticks = GetTime() - m_StartTime;
        float pitch = (((float)std::sin(ticks * 3 * 3.14 / 1000) * 0.5f + 0.5f) * 360.0f) - 180.0f;
        pitch = (pitch / 5.5f) + 130.0f;

        m_Client->GetPlayerController()->SetPitch(pitch);
    }

    void OnClientSpawn(mc::core::PlayerPtr player) override {
        using namespace mc::protocol::packets::out;
        EntityActionPacket::Action action = EntityActionPacket::Action::StartSneak;

        EntityActionPacket packet(player->GetEntity()->GetEntityId(), action);
        m_Connection->SendPacket(&packet);
    }
};

class Logger : public mc::protocol::packets::PacketHandler, public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;

public:
    Logger(mc::core::Client* client, mc::protocol::packets::PacketDispatcher* dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher), m_Client(client)
    {
        using namespace mc::protocol;

        m_Client->RegisterListener(this);

        dispatcher->RegisterHandler(State::Login, login::Disconnect, this);

        dispatcher->RegisterHandler(State::Play, play::Chat, this);
        dispatcher->RegisterHandler(State::Play, play::Disconnect, this);
        dispatcher->RegisterHandler(State::Play, play::EntityLookAndRelativeMove, this);
        dispatcher->RegisterHandler(State::Play, play::BlockChange, this);
    }

    ~Logger() {
        GetDispatcher()->UnregisterHandler(this);
        m_Client->UnregisterListener(this);
    }

    void HandlePacket(mc::protocol::packets::in::ChatPacket* packet) {
        std::string message = mc::util::ParseChatNode(packet->GetChatData());

        std::cout << message << std::endl;
    }

    void HandlePacket(mc::protocol::packets::in::EntityLookAndRelativeMovePacket* packet) {
        Vector3d delta = mc::ToVector3d(packet->GetDelta()) / (128.0 * 32.0);

        //std::cout << delta << std::endl;
    }

    void HandlePacket(mc::protocol::packets::in::BlockChangePacket* packet) {
        Vector3i pos = packet->GetPosition();
        s32 blockId = packet->GetBlockId();

        std::cout << "Block changed at " << pos << " to " << blockId << std::endl;
    }

    void HandlePacket(mc::protocol::packets::in::DisconnectPacket* packet) {
        std::wcout << L"Disconnected: " << packet->GetReason() << std::endl;
    }

    void OnTick() override {
        mc::core::PlayerPtr player = m_Client->GetPlayerManager()->GetPlayerByName(L"testplayer");
        if (!player) return;

        mc::entity::EntityPtr entity = player->GetEntity();
        if (!entity) return;
    }
};

// Spam block dig packet to stress test WorldEdit.
// Sends no arm swing packet.
class BlockDigStressTest : public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    mc::util::PlayerController* m_PlayerController;
    mc::world::World* m_World;
    Vector3i m_Target;
    mc::inventory::Slot m_HeldItem;
    s32 m_DigPerTick;

public:
    BlockDigStressTest(mc::core::Client* client, s32 digPerTick)
        : m_Client(client), 
          m_PlayerController(client->GetPlayerController()), 
          m_World(client->GetWorld()), 
          m_DigPerTick(digPerTick)
    {
        client->RegisterListener(this);
    }

    ~BlockDigStressTest() {
        m_Client->UnregisterListener(this);
    }

    void OnTick() {
        const Vector3i offset(0, 2, 0);

        m_Target = ToVector3i(m_PlayerController->GetPosition()) + offset;

        if (m_Target == offset) return;
        if (!m_World->GetChunk(m_Target)) return;

        m_PlayerController->LookAt(ToVector3d(m_Target));

        mc::block::BlockPtr block = m_World->GetBlock(m_Target + Vector3i(0, 1, 0)).GetBlock();

        if (block == nullptr) {
            std::cerr << "Block is nullptr" << std::endl;
            return;
        }

        std::cout << "Sending " << m_DigPerTick << " block break packets." << std::endl;

        using namespace mc::protocol::packets::out;

        for (s32 i = 0; i < m_DigPerTick; ++i) {
            PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::StartedDigging;
            PlayerDiggingPacket packet(status, m_Target + Vector3i(0, 1, 0), (u8)Face::West);

            m_Client->GetConnection()->SendPacket(&packet);

            status = PlayerDiggingPacket::Status::FinishedDigging;
            packet = PlayerDiggingPacket(status, m_Target + Vector3i(0, 1, 0), (u8)Face::West);

            m_Client->GetConnection()->SendPacket(&packet);
    }
    }
};

int main(void) {
    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks();
    mc::protocol::packets::PacketDispatcher dispatcher;
    mc::protocol::Version version = mc::protocol::Version::Minecraft_1_11_2;

    mc::core::Client gameClient(&dispatcher, version);
    mc::util::ForgeHandler forgeHandler(&dispatcher, gameClient.GetConnection());

    const std::string server("127.0.0.1");
    const u16 port = 25565;

    {
        mc::core::Client pingClient(&dispatcher, version);

        try {
            pingClient.Ping(server, port);
        } catch (std::exception& e) {
            std::wcout << e.what() << std::endl;
            return 1;
        }

        std::cout << "Pinging server." << std::endl;

        while (!forgeHandler.HasModInfo()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    gameClient.GetPlayerController()->SetHandleFall(true);
    gameClient.GetConnection()->GetSettings()
        .SetMainHand(mc::MainHand::Right)
        .SetViewDistance(20);

    Logger logger(&gameClient, &dispatcher);
    //BlockDigStressTest stressTest(&gameClient, 100);
    try {
        std::cout << "Logging in." << std::endl;
        gameClient.Login(server, port, "testplayer", "", true);
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
