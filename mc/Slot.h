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

    s16 GetItemId() const { return m_ItemId; }
    u8 GetItemCount() const { return m_ItemCount; }
    s16 GetItemDamage() const { return m_ItemDamage; }
    const NBT::NBT& GetNBT() const { return m_NBT; }

    friend DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
    friend DataBuffer& operator>>(DataBuffer& in, Slot& slot);
};

DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
DataBuffer& operator>>(DataBuffer& in, Slot& slot);

} // ns Minecraft

#endif
