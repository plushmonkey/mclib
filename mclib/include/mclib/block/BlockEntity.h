#ifndef MCLIB_BLOCK_BLOCK_ENTITY_H_
#define MCLIB_BLOCK_BLOCK_ENTITY_H_

#include <mclib/mclib.h>
#include <mclib/common/Vector.h>
#include <mclib/nbt/NBT.h>
#include <mclib/inventory/Slot.h>

#include <string>
#include <memory>
#include <unordered_map>
#include <array>
#include <vector>

namespace mc {
namespace block {

enum class BlockEntityType {
    Banner,
    Beacon,
    Cauldron,
    BrewingStand,
    Chest,
    Comparator,
    CommandBlock,
    DaylightSensor,
    Dispenser,
    Dropper,
    EnchantingTable,
    EnderChest,
    EndGateway,
    EndPortal,
    FlowerPot,
    Furnace,
    Hopper,
    Jukebox,
    MobSpawner,
    Noteblock,
    Piston,
    Sign,
    Skull,
    StructureBlock,

    Unknown
};

class BlockEntity {
private:
    BlockEntityType m_Type;
    Vector3i m_Position;
    nbt::NBT m_NBT;

protected:
    virtual MCLIB_API bool ImportNBT(nbt::NBT* nbt) { return true; }

public:
    MCLIB_API BlockEntity(BlockEntityType type, Vector3i position) noexcept;

    MCLIB_API BlockEntityType GetType() const noexcept { return m_Type; }
    MCLIB_API Vector3i GetPosition() const noexcept { return m_Position; }
    MCLIB_API nbt::NBT* GetNBT() noexcept { return &m_NBT; }

    MCLIB_API static std::unique_ptr<BlockEntity> CreateFromNBT(nbt::NBT* nbt);
};
using BlockEntityPtr = std::shared_ptr<BlockEntity>;

class SignBlockEntity : public BlockEntity {
private:
    std::array<std::wstring, 4> m_Text;

public:
    MCLIB_API SignBlockEntity(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    MCLIB_API const std::wstring& GetText(std::size_t index) const;
};

class ChestBlockEntity : public BlockEntity {
public:
    using ItemMap = std::unordered_map<u8, inventory::Slot>;
private:
    std::wstring m_Name;
    std::wstring m_Lock;
    ItemMap m_Items;
    std::wstring m_LootTable;
    s64 m_LootTableSeed;

public:
    MCLIB_API ChestBlockEntity(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);
    
    const std::wstring& GetName() const noexcept { return m_Name; }
    const std::wstring& GetLock() const noexcept { return m_Lock; }
    const ItemMap& GetItems() const noexcept { return m_Items; }
    const std::wstring& GetLootTable() const noexcept { return m_LootTable; }
    s64 GetLootTableSeed() const noexcept { return m_LootTableSeed; }
};


} // ns block
} // ns mc

#endif
