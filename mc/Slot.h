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
    Slot() { }

    friend DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
    friend DataBuffer& operator>>(DataBuffer& in, Slot& slot);
};

DataBuffer& operator<<(DataBuffer& out, const Slot& slot);
DataBuffer& operator>>(DataBuffer& in, Slot& slot);

} // ns Minecraft

#endif
