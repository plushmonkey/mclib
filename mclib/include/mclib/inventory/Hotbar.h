#ifndef MCLIB_INVENTORY_HOTBAR_H_
#define MCLIB_INVENTORY_HOTBAR_H_

#include <mclib/core/Connection.h>
#include <mclib/inventory/Slot.h>
#include <mclib/protocol/packets/PacketHandler.h>

namespace mc {
namespace inventory {

class InventoryManager;

class Hotbar : public protocol::packets::PacketHandler {
private:
    core::Connection* m_Connection;
    InventoryManager* m_InventoryManager;
    s32 m_SelectedSlot;

public:
    MCLIB_API Hotbar(protocol::packets::PacketDispatcher* dispatcher,
                     core::Connection* connection,
                     InventoryManager* inventoryManager);
    MCLIB_API ~Hotbar();

    Hotbar(const Hotbar& rhs) = delete;
    Hotbar& operator=(const Hotbar& rhs) = delete;
    Hotbar(Hotbar&& rhs) = delete;
    Hotbar& operator=(Hotbar&& rhs) = delete;

    MCLIB_API void HandlePacket(
        protocol::packets::in::HeldItemChangePacket* packet);

    // Sends a packet telling the server that the client is changing slots.
    // Slot should be between 0 and 8.
    MCLIB_API void SelectSlot(s32 hotbarIndex);
    MCLIB_API s32 GetSelectedSlot() const noexcept { return m_SelectedSlot; }

    MCLIB_API Slot GetCurrentItem();
    // Index should be between 0 and 8.
    MCLIB_API Slot GetItem(s32 hotbarIndex);
};

}  // namespace inventory
}  // namespace mc

#endif
