#ifndef SLOT_H_
#define SLOT_H_

#include "Types.h"
#include "NBT.h"

namespace Minecraft {

class DataBuffer;

class Slot {
private:
    s16 m_ItemId;
    u8 m_ItemCount;
    s16 m_ItemDamage;
    NBT::NBT m_NBT;


public:
    Slot() : m_ItemId(-1), m_ItemCount(0), m_ItemDamage(0) { }
    Slot(s16 itemId, u8 itemCount, s16 itemDamage)
        : m_ItemId(itemId), m_ItemCount(itemCount), m_ItemDamage(itemDamage), m_NBT()
    { }

    Slot(s16 itemId, u8 itemCount, s16 itemDamage, NBT::NBT nbt) 
        : m_ItemId(itemId), m_ItemCount(itemCount), m_ItemDamage(itemDamage), m_NBT(nbt)
    { }

    s16 GetItemId() const { return m_ItemId; }
    u8 GetItemCount() const { return m_ItemCount; }
    s16 GetItemDamage() const { return m_ItemDamage; }
    const NBT::NBT& GetNBT() const { return m_NBT; }

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, Slot& slot);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, Slot& slot);

} // ns Minecraft

#endif
