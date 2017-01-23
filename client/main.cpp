#include "../mclib/Common.h"
#include "../mclib/Client.h"
#include "../mclib/Hash.h"
#include "../mclib/Utility.h"
#include <thread>
#include <iostream>
#include <chrono>

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
#pragma comment(lib, "../lib/jsoncpp/lib/jsoncpp-MDd.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
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

class BlockPlacer : public Minecraft::Packets::PacketHandler, public Minecraft::WorldListener, public ClientListener {
private:
    Client* m_Client;
    PlayerController* m_PlayerController;
    Minecraft::World* m_World;
    Vector3i m_Target;
    s64 m_LastUpdate;
    Minecraft::Slot m_HeldItem;

public:
    BlockPlacer(Minecraft::Packets::PacketDispatcher* dispatcher, Client* client, PlayerController* pc, Minecraft::World* world) 
        : Minecraft::Packets::PacketHandler(dispatcher), m_Client(client), m_PlayerController(pc), m_World(world), m_LastUpdate(GetTime())
    {
        m_Target = Vector3i(-2, 62, 275);
        world->RegisterListener(this);
        client->RegisterListener(this);

        using namespace Minecraft::Protocol;
        dispatcher->RegisterHandler(State::Play, Play::WindowItems, this);
        dispatcher->RegisterHandler(State::Play, Play::SetSlot, this);
    }

    ~BlockPlacer() {
        GetDispatcher()->UnregisterHandler(this);
        m_World->UnregisterListener(this);
        m_Client->UnregisterListener(this);
    }

    void HandlePacket(Minecraft::Packets::Inbound::WindowItemsPacket* packet) {
        auto slots = packet->GetSlots();
        m_HeldItem = slots[36];
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetSlotPacket* packet) {
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
            Minecraft::BlockPtr block = m_World->GetBlock(m_Target + Vector3i(0, 1, 0));
            
            if (!block || block->GetType() == 0) {
                Minecraft::Packets::Outbound::PlayerBlockPlacementPacket blockPlacePacket(m_Target, (u8)Face::Top, Minecraft::Hand::Main, Vector3f(0.5, 0, 0.5));

                m_Client->GetConnection()->SendPacket(&blockPlacePacket);
                std::wcout << "Placing block" << std::endl;
            } else {
                using namespace Minecraft::Packets::Outbound;
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

class TextureGrabber : public Minecraft::Packets::PacketHandler {
private:
    bool ContainsTextureURL(const Json::Value& root) {
        if (!root.isMember("textures")) return false;
        if (!root["textures"].isMember("SKIN")) return false;
        return root["textures"]["SKIN"].isMember("url");
    }

public:
    TextureGrabber(Minecraft::Packets::PacketDispatcher* dispatcher)
        : Minecraft::Packets::PacketHandler(dispatcher)
    {
        using namespace Minecraft::Protocol;

        dispatcher->RegisterHandler(State::Play, Play::PlayerListItem, this);
    }

    ~TextureGrabber() {
        GetDispatcher()->UnregisterHandler(this);
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerListItemPacket* packet) {
        using namespace Minecraft::Packets::Inbound;

        PlayerListItemPacket::Action action = packet->GetAction();

        if (action == PlayerListItemPacket::Action::AddPlayer) {
            auto actionDataList = packet->GetActionData();

            for (auto actionData : actionDataList) {
                auto properties = actionData->properties;

                auto iter = properties.find(L"textures");

                if (iter == properties.end()) continue;

                std::wstring encoded = iter->second;
                std::string decoded = Base64Decode(std::string(encoded.begin(), encoded.end()));

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

                CurlHTTPClient http;

                HTTPResponse resp = http.Get(url);

                if (resp.status == 200) {
                    std::wcout << L"Saving texture for " << name << std::endl;

                    std::string body = resp.body;

                    std::ofstream out(name + L".png", std::ios::out | std::ios::binary);

                    out.write(body.c_str(), body.size());
                }
            }
        }
    }
};

class SneakEnforcer : public Minecraft::PlayerListener, public ClientListener {
private:
    Client* m_Client;
    Minecraft::PlayerManager* m_PlayerManager;
    Minecraft::Connection* m_Connection;
    s64 m_StartTime;

public:
    SneakEnforcer(Client* client) 
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

    void OnClientSpawn(Minecraft::PlayerPtr player) override {
        using namespace Minecraft::Packets::Outbound;
        EntityActionPacket::Action action = EntityActionPacket::Action::StartSneak;

        EntityActionPacket packet(player->GetEntity()->GetEntityId(), action);
        m_Connection->SendPacket(&packet);
    }
};

class Logger : public Minecraft::Packets::PacketHandler, public ClientListener {
private:
    Client* m_Client;

public:
    Logger(Client* client, Minecraft::Packets::PacketDispatcher* dispatcher)
        : Minecraft::Packets::PacketHandler(dispatcher), m_Client(client)
    {
        using namespace Minecraft::Protocol;

        m_Client->RegisterListener(this);

        dispatcher->RegisterHandler(State::Play, Play::Chat, this);
        dispatcher->RegisterHandler(State::Play, Play::EntityLookAndRelativeMove, this);
        dispatcher->RegisterHandler(State::Play, Play::BlockChange, this);
    }

    ~Logger() {
        GetDispatcher()->UnregisterHandler(this);
        m_Client->UnregisterListener(this);
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChatPacket* packet) {
        std::string message = ParseChatNode(packet->GetChatData());

        std::cout << message << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityLookAndRelativeMovePacket* packet) {
        Vector3d delta = ToVector3d(packet->GetDelta()) / (128.0 * 32.0);

        //std::cout << delta << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::BlockChangePacket* packet) {
        Vector3i pos = packet->GetPosition();
        s32 blockId = packet->GetBlockId();

        std::cout << "Block changed at " << pos << " to " << blockId << std::endl;
    }

    void OnTick() override {
        Minecraft::PlayerPtr player = m_Client->GetPlayerManager()->GetPlayerByName(L"testplayer");
        if (!player) return;

        Minecraft::EntityPtr entity = player->GetEntity();
        if (!entity) return;

        //Vector3d check = entity->GetPosition() + Vector3d(3, 0, 0);
        //Minecraft::BlockPtr block = m_Client->GetWorld()->GetBlock(check);
        //if (block)
            //std::cout << check << " = " << block->GetData() << std::endl;
        struct BlockAssertion {
            Vector3d pos;
            std::wstring type;
            BlockAssertion(Vector3d p, std::wstring t)
                : pos(p), type(t) { }
        };

        std::vector<BlockAssertion> assertions = {
            BlockAssertion(Vector3d(67, 72, 244), L"Air"),
            BlockAssertion(Vector3d(67, 71, 244), L"Grass"),
            BlockAssertion(Vector3d(67, 72, 243), L"Grass"),
            BlockAssertion(Vector3d(54, 72, 244), L"Air"),
            BlockAssertion(Vector3d(54, 61, 251), L"Granite"),
            BlockAssertion(Vector3d(6, 11, 85), L"Still Water"),
            BlockAssertion(Vector3d(9, 11, 85), L"Still Water"),
        };

        for (int i = 0; i < 20; ++i) {
            Minecraft::BlockPtr block = m_Client->GetWorld()->GetBlock(Vector3i(6 + i, 11, 85));
        }

        for (BlockAssertion assertion : assertions) {
            Minecraft::BlockPtr block = m_Client->GetWorld()->GetBlock(assertion.pos);
            if (!block) continue;
            if (block->GetName().compare(assertion.type) != 0) {
                std::wcout << L"Block at " << assertion.pos << L" is not " << assertion.type << L" instead is " << block->GetName() << std::endl;
            }
        }
    }
};

int main(void) {
    Minecraft::BlockRegistry::GetInstance()->RegisterVanillaBlocks();
    Minecraft::Packets::PacketDispatcher dispatcher;

    Client client(&dispatcher);
    client.GetPlayerController()->SetHandleFall(true);

    Logger logger(&client, &dispatcher);
    SneakEnforcer sneakEnforcer(&client);

    try {
        client.Login("127.0.0.1", 25565, "testplayer", "");
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return 1;
    }
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
