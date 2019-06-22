#ifndef MCLIB_BLOCK_FURNACE_H_
#define MCLIB_BLOCK_FURNACE_H_

#include <mclib/block/BlockEntity.h>
#include <mclib/block/InventoryBlock.h>
#include <mclib/common/Nameable.h>

#include <unordered_map>

namespace mc {
namespace block {

class Furnace : public BlockEntity, public InventoryBlock, public Nameable {
private:
    s16 m_BurnTime;
    s16 m_CookTime;
    s16 m_CookTimeTotal;

public:
    MCLIB_API Furnace(BlockEntityType type, Vector3i position)
        : BlockEntity(type, position),
          m_BurnTime(0),
          m_CookTime(0),
          m_CookTimeTotal(0) {}
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    inventory::Slot GetSmeltingSlot() const noexcept;
    inventory::Slot GetFuelSlot() const noexcept;
    inventory::Slot GetResultSlot() const noexcept;

    // Number of ticks left before the current fuel runs out.
    s16 GetBurnTime() const noexcept { return m_BurnTime; }
    // Number of ticks the item has been smelting for. The item finishes
    // smelting when this value reaches 200 (10 seconds). Is reset to 0 if
    // BurnTime reaches 0.
    s16 GetCookTime() const noexcept { return m_CookTime; }
    // Number of ticks It takes for the item to be smelted.
    s16 GetCookTimeTotal() const noexcept { return m_CookTimeTotal; }
};

}  // namespace block
}  // namespace mc

#endif
