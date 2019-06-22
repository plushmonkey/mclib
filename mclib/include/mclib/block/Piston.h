#ifndef MCLIB_BLOCK_PISTON_H_
#define MCLIB_BLOCK_PISTON_H_

#include <mclib/block/BlockEntity.h>
#include <mclib/common/Types.h>

#include <unordered_map>

namespace mc {
namespace block {

// Used for both the piston and the block being moved
class Piston : public BlockEntity {
private:
    // Id of the block being moved.
    s32 m_BlockId;
    // Data of the block being moved.
    s32 m_BlockData;
    // Direction that the block will be pushed.
    Face m_Facing;
    // How far the block has been moved.
    float m_Progress;
    // True if this is the block being pushed
    bool m_Extending;
    // True if this is the piston.
    bool m_Source;

public:
    MCLIB_API Piston(BlockEntityType type, Vector3i position)
        : BlockEntity(type, position) {}
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    inline s32 GetBlockId() const noexcept { return m_BlockId; }
    inline s32 GetBlockData() const noexcept { return m_BlockData; }
    inline Face GetFacing() const noexcept { return m_Facing; }
    inline float GetProgress() const noexcept { return m_Progress; }
    inline bool IsExtending() const noexcept { return m_Extending; }
    inline bool IsSource() const noexcept { return m_Source; }
};

}  // namespace block
}  // namespace mc

#endif
