#include <mclib/inventory/Hotbar.h>

#include <mclib/inventory/Inventory.h>
#include <mclib/protocol/packets/PacketDispatcher.h>

namespace mc {
namespace inventory {

Hotbar::Hotbar(protocol::packets::PacketDispatcher* dispatcher,
               core::Connection* connection, InventoryManager* inventoryManager)
    : protocol::packets::PacketHandler(dispatcher),
      m_Connection(connection),
      m_InventoryManager(inventoryManager),
      m_SelectedSlot(0) {
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::HeldItemChange, this);
}

Hotbar::~Hotbar() { GetDispatcher()->UnregisterHandler(this); }

void Hotbar::SelectSlot(s32 hotbarIndex) {
    m_SelectedSlot = hotbarIndex;

    protocol::packets::out::HeldItemChangePacket heldItemPacket(hotbarIndex);
    m_Connection->SendPacket(&heldItemPacket);
}

void Hotbar::HandlePacket(protocol::packets::in::HeldItemChangePacket* packet) {
    m_SelectedSlot = packet->GetSlot();
}

Slot Hotbar::GetCurrentItem() { return GetItem(m_SelectedSlot); }

Slot Hotbar::GetItem(s32 hotbarIndex) {
    Inventory* inventory = m_InventoryManager->GetPlayerInventory();
    if (inventory == nullptr) return Slot();

    return inventory->GetItem(Inventory::HOTBAR_SLOT_START + hotbarIndex);
}

}  // namespace inventory
}  // namespace mc
