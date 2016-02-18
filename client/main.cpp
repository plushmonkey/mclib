#include "../mclib/Common.h"
#include "../mclib/Client.h"
#include <thread>
#include <iostream>
#include <chrono>

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
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
                Minecraft::Packets::Outbound::PlayerBlockPlacementPacket blockPlacePacket(m_Target, (u8)Face::Top, m_HeldItem, Vector3i(8, 0, 8));

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

int main(void) {
    Minecraft::BlockRegistry::GetInstance()->RegisterVanillaBlocks();
    Minecraft::Packets::PacketDispatcher dispatcher;

    Client client(&dispatcher);
    BlockPlacer blockPlacer(&dispatcher, &client, client.GetPlayerController(), client.GetWorld());

    try {
        client.Login("192.168.2.202", 25565, "testplayer", "");
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return 1;
    }
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
