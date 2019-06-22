#ifndef MCLIB_BLOCK_BREWINGSTAND_H_
#define MCLIB_BLOCK_BREWINGSTAND_H_

#include <mclib/block/BlockEntity.h>
#include <mclib/block/InventoryBlock.h>
#include <mclib/common/Nameable.h>

#include <unordered_map>

namespace mc {
namespace block {

class BrewingStand : public BlockEntity,
                     public InventoryBlock,
                     public Nameable {
private:
    s32 m_BrewTime;
    u8 m_Fuel;

public:
    MCLIB_API BrewingStand(BlockEntityType type, Vector3i position)
        : BlockEntity(type, position) {}
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    inventory::Slot GetLeftSlot() const noexcept;
    inventory::Slot GetMiddleSlot() const noexcept;
    inventory::Slot GetRightSlot() const noexcept;
    inventory::Slot GetIngredientSlot() const noexcept;
    inventory::Slot GetFuelSlot() const noexcept;

    u8 GetBrewTime() const noexcept { return m_BrewTime; }
    u8 GetFuel() const noexcept { return m_Fuel; }
};

}  // namespace block
}  // namespace mc

#endif
