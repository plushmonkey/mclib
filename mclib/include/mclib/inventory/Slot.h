#ifndef MCLIB_INVENTORY_SLOT_H_
#define MCLIB_INVENTORY_SLOT_H_

#include <mclib/mclib.h>
#include <mclib/common/Types.h>
#include <mclib/nbt/NBT.h>

namespace mc {

class DataBuffer;

namespace inventory {

class Slot {
private:
    s16 m_ItemId;
    u8 m_ItemCount;
    s16 m_ItemDamage;
    nbt::NBT m_NBT;


public:
    Slot() : m_ItemId(-1), m_ItemCount(0), m_ItemDamage(0) { }
    Slot(s16 itemId, u8 itemCount, s16 itemDamage)
        : m_ItemId(itemId), m_ItemCount(itemCount), m_ItemDamage(itemDamage), m_NBT()
    { }

    Slot(s16 itemId, u8 itemCount, s16 itemDamage, nbt::NBT nbt)
        : m_ItemId(itemId), m_ItemCount(itemCount), m_ItemDamage(itemDamage), m_NBT(nbt)
    { }

    s16 GetItemId() const { return m_ItemId; }
    u8 GetItemCount() const { return m_ItemCount; }
    s16 GetItemDamage() const { return m_ItemDamage; }
    const nbt::NBT& GetNBT() const { return m_NBT; }

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, Slot& slot);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, Slot& slot);

} // ns inventory
} // ns mc

#endif
