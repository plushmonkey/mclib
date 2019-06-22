#include "Logger.h"

#include <mclib/util/Utility.h>

#include <iostream>

namespace example {

Logger::Logger(mc::core::Client* client,
               mc::protocol::packets::PacketDispatcher* dispatcher)
    : mc::protocol::packets::PacketHandler(dispatcher), m_Client(client) {
    using namespace mc::protocol;

    m_Client->RegisterListener(this);

    dispatcher->RegisterHandler(State::Login, login::Disconnect, this);

    dispatcher->RegisterHandler(State::Play, play::Chat, this);
    dispatcher->RegisterHandler(State::Play, play::Disconnect, this);
    dispatcher->RegisterHandler(State::Play, play::EntityLookAndRelativeMove,
                                this);
    dispatcher->RegisterHandler(State::Play, play::BlockChange, this);
    dispatcher->RegisterHandler(State::Play, play::MultiBlockChange, this);
}

Logger::~Logger() {
    GetDispatcher()->UnregisterHandler(this);
    m_Client->UnregisterListener(this);
}

void Logger::HandlePacket(mc::protocol::packets::in::ChatPacket* packet) {
    std::string message = mc::util::ParseChatNode(packet->GetChatData());

    if (!message.empty()) std::cout << message << std::endl;

    if (message.find("!selected") != std::string::npos) {
        mc::inventory::Slot item = m_Client->GetHotbar().GetCurrentItem();

        std::cout << "Selected item: " << item.GetItemId() << ":"
                  << item.GetItemDamage() << " (" << item.GetItemCount() << ")"
                  << std::endl;
    } else if (message.find("!select") != std::string::npos) {
        std::string amountStr = message.substr(message.find("!select ") + 8);
        int slotIndex = strtol(amountStr.c_str(), nullptr, 10);

        if (slotIndex >= 0 && slotIndex < 9) {
            m_Client->GetHotbar().SelectSlot(slotIndex);
        } else {
            std::cout << "Bad slot index." << std::endl;
        }
    } else if (message.find("!find ") != std::string::npos) {
        std::string toFind = message.substr(message.find("!find ") + 6);

        s32 itemId = strtol(toFind.c_str(), nullptr, 10);
        mc::inventory::Inventory* inv =
            m_Client->GetInventoryManager()->GetPlayerInventory();
        if (inv) {
            bool contained = inv->Contains(itemId);

            std::cout << "Contains " << itemId << ": " << std::boolalpha
                      << contained << std::endl;
        }
    }
}

void Logger::HandlePacket(
    mc::protocol::packets::in::EntityLookAndRelativeMovePacket* packet) {
    mc::Vector3d delta = mc::ToVector3d(packet->GetDelta()) / (128.0 * 32.0);

    // std::cout << delta << std::endl;
}

void Logger::HandlePacket(
    mc::protocol::packets::in::BlockChangePacket* packet) {
    mc::Vector3i pos = packet->GetPosition();
    s32 blockId = packet->GetBlockId();

    std::cout << "Block changed at " << pos << " to " << blockId << std::endl;
}

void Logger::HandlePacket(
    mc::protocol::packets::in::MultiBlockChangePacket* packet) {
    auto chunkX = packet->GetChunkX();
    auto chunkZ = packet->GetChunkZ();

    for (const auto& change : packet->GetBlockChanges()) {
        mc::Vector3i pos(chunkX + change.x, change.y + chunkZ + change.z);

        std::cout << "Block changed at " << pos << " to " << change.blockData
                  << std::endl;
    }
}

void Logger::HandlePacket(mc::protocol::packets::in::DisconnectPacket* packet) {
    std::wcout << L"Disconnected: " << packet->GetReason() << std::endl;
}

void Logger::OnTick() {
    mc::core::PlayerPtr player =
        m_Client->GetPlayerManager()->GetPlayerByName(L"testplayer");
    if (!player) return;

    mc::entity::EntityPtr entity = player->GetEntity();
    if (!entity) return;
}

}  // namespace example
