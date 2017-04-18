#ifndef MCLIB_INVENTORY_INVENTORY_H_
#define MCLIB_INVENTORY_INVENTORY_H_

#include <mclib/core/Connection.h>
#include <mclib/inventory/Slot.h>
#include <mclib/protocol/packets/PacketHandler.h>

namespace mc {
namespace inventory {

class Inventory {
public:
    static const s32 HOTBAR_SLOT_START;
    static const s32 PLAYER_INVENTORY_ID;

    using ItemMap = std::map<s32, Slot>;

private:
    core::Connection* m_Connection;
    ItemMap m_Items;
    int m_WindowId;
    s32 m_SelectedHotbarIndex;

public:
    MCLIB_API Inventory(core::Connection* connection, int windowId);

    MCLIB_API Slot GetItem(s32 index) const;
    MCLIB_API const ItemMap& GetItems() const noexcept { return m_Items; }

    // Returns item slot index. Returns -1 if none are found.
    MCLIB_API s32 FindItemById(s32 itemId) const;

    MCLIB_API bool Contains(s32 itemId) const;
    MCLIB_API bool Contains(Slot item) const;
    MCLIB_API bool ContainsAtLeast(s32 itemId, s32 amount) const;
    MCLIB_API bool ContainsAtLeast(Slot item, s32 amount) const;

    friend class InventoryManager;
};

class InventoryManager : public protocol::packets::PacketHandler {
private:
    core::Connection* m_Connection;
    std::map<s32, std::unique_ptr<Inventory>> m_Inventories;

    void SetSlot(s32 windowId, s32 slotIndex, const Slot& slot);

public:
    MCLIB_API InventoryManager(protocol::packets::PacketDispatcher* dispatcher, core::Connection* connection);
    MCLIB_API ~InventoryManager();

    MCLIB_API void HandlePacket(protocol::packets::in::SetSlotPacket* packet);
    MCLIB_API void HandlePacket(protocol::packets::in::WindowItemsPacket* packet);

    MCLIB_API Inventory* GetInventory(s32 windowId);
    MCLIB_API Inventory* GetPlayerInventory();
};

} // ns inventory
} // ns mc

#endif
