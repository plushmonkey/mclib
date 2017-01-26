#ifndef MCLIB_BLOCK_ENTITY_H_
#define MCLIB_BLOCK_ENTITY_H_

#include "mclib.h"
#include "Vector.h"
#include "NBT.h"

#include <string>
#include <memory>
#include <array>

namespace Minecraft {

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
    NBT::NBT m_NBT;

protected:
    virtual MCLIB_API bool ImportNBT(NBT::NBT* nbt) { return true; }

public:
    MCLIB_API BlockEntity(BlockEntityType type, Vector3i position);

    MCLIB_API BlockEntityType GetType() const { return m_Type; }
    MCLIB_API Vector3i GetPosition() const { return m_Position; }
    MCLIB_API NBT::NBT* GetNBT() { return &m_NBT; }

    MCLIB_API static std::shared_ptr<BlockEntity> CreateFromNBT(NBT::NBT* nbt);
};
typedef std::shared_ptr<BlockEntity> BlockEntityPtr;

class SignBlockEntity : public BlockEntity {
private:
    std::array<std::wstring, 4> m_Text;

public:
    MCLIB_API SignBlockEntity(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
    MCLIB_API bool ImportNBT(NBT::NBT* nbt);

    MCLIB_API const std::wstring& GetText(std::size_t index) const;
};

} // ns Minecraft

#endif
