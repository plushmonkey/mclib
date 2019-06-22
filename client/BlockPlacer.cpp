#include "BlockPlacer.h"

#include <mclib/util/Utility.h>

#include <iostream>

namespace example {

BlockPlacer::BlockPlacer(mc::protocol::packets::PacketDispatcher* dispatcher,
                         mc::core::Client* client,
                         mc::util::PlayerController* pc,
                         mc::world::World* world)
    : mc::protocol::packets::PacketHandler(dispatcher),
      m_Client(client),
      m_PlayerController(pc),
      m_World(world),
      m_LastUpdate(mc::util::GetTime()) {
    m_Target = mc::Vector3i(-2, 62, 275);
    world->RegisterListener(this);
    client->RegisterListener(this);

    using namespace mc::protocol;
    dispatcher->RegisterHandler(State::Play, play::WindowItems, this);
    dispatcher->RegisterHandler(State::Play, play::SetSlot, this);
}

BlockPlacer::~BlockPlacer() {
    GetDispatcher()->UnregisterHandler(this);
    m_World->UnregisterListener(this);
    m_Client->UnregisterListener(this);
}

void BlockPlacer::HandlePacket(
    mc::protocol::packets::in::WindowItemsPacket* packet) {
    auto slots = packet->GetSlots();
    m_HeldItem = slots[36];
}

void BlockPlacer::HandlePacket(
    mc::protocol::packets::in::SetSlotPacket* packet) {
    if (packet->GetWindowId() != 0) return;

    if (packet->GetSlotIndex() == 36) m_HeldItem = packet->GetSlot();
}

void BlockPlacer::OnTick() {
    s64 time = mc::util::GetTime();
    if (time - m_LastUpdate < 5000) return;
    m_LastUpdate = time;

    if (m_PlayerController->GetPosition() == mc::Vector3d(0, 0, 0)) return;
    if (!m_World->GetChunk(m_Target)) return;

    m_PlayerController->LookAt(ToVector3d(m_Target));

    if (m_HeldItem.GetItemId() != -1) {
        mc::block::BlockPtr block =
            m_World->GetBlock(m_Target + mc::Vector3i(0, 1, 0));

        if (!block || block->GetType() == 0) {
            mc::protocol::packets::out::PlayerBlockPlacementPacket
                blockPlacePacket(m_Target, mc::Face::Top, mc::Hand::Main,
                                 mc::Vector3f(0.5, 0, 0.5));

            m_Client->GetConnection()->SendPacket(&blockPlacePacket);
            std::wcout << "Placing block" << std::endl;
        } else {
            using namespace mc::protocol::packets::out;
            {
                PlayerDiggingPacket::Status status =
                    PlayerDiggingPacket::Status::StartedDigging;
                PlayerDiggingPacket packet(
                    status, m_Target + mc::Vector3i(0, 1, 0), mc::Face::West);

                m_Client->GetConnection()->SendPacket(&packet);
            }

            std::wcout << "Destroying block" << std::endl;

            {
                PlayerDiggingPacket::Status status =
                    PlayerDiggingPacket::Status::FinishedDigging;
                PlayerDiggingPacket packet(
                    status, m_Target + mc::Vector3i(0, 1, 0), mc::Face::West);

                m_Client->GetConnection()->SendPacket(&packet);
            }
        }
    }
}

}  // namespace example
