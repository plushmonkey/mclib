#include "Slot.h"
#include "DataBuffer.h"

namespace Minecraft {

DataBuffer& operator<<(DataBuffer& out, const Slot& slot) {
    out << slot.m_ItemId;
    if (slot.m_ItemId == -1) return out;

    out << slot.m_ItemCount;
    out << slot.m_ItemDamage;

    u8 hasNBT = slot.m_NBT.HasData();

    if (hasNBT)
        out << slot.m_NBT;
    else
        out << (u8)0;
    return out;
}

DataBuffer& operator>>(DataBuffer& in, Slot& slot) {
    in >> slot.m_ItemId;

    if (slot.m_ItemId == -1) {
        slot.m_ItemCount = 0;
        slot.m_ItemDamage = 0;
        return in;
    }

    in >> slot.m_ItemCount;
    in >> slot.m_ItemDamage;

    u8 hasNBT;
    in >> hasNBT;

    if (hasNBT) {
        in.SetReadOffset(in.GetReadOffset() - 1);
        in >> slot.m_NBT;
    }
}

} // ns Minecraft
