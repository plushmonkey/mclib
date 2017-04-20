#include <mclib/inventory/Slot.h>

#include <mclib/common/DataBuffer.h>

namespace mc {
namespace inventory {

Slot Slot::FromNBT(nbt::TagCompound& compound) {
    u8 count = 0;
    s16 id = -1;
    s16 damage = 0;
    nbt::NBT nbt;

    for (auto iter = compound.begin(); iter != compound.end(); ++iter) {
        auto tag = iter->second;
        if (tag->GetName() == L"Count") {
            count = ((nbt::TagByte*)tag.get())->GetValue();
        } else if (tag->GetName() == L"Damage") {
            damage = ((nbt::TagShort*)tag.get())->GetValue();
        } else if (tag->GetName() == L"id") {
            id = ((nbt::TagShort*)tag.get())->GetValue();
        } else if (tag->GetName() == L"tag") {
            nbt::TagCompound* newRoot = ((nbt::TagCompound*)tag.get());

            nbt.SetRoot(*newRoot);
        }
    }

    return Slot(id, count, damage, nbt);
}

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
    return in;
}

} // ns inventory
} // ns mc
