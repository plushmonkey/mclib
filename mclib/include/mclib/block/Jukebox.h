#ifndef MCLIB_BLOCK_JUKEBOX_H_
#define MCLIB_BLOCK_JUKEBOX_H_

#include <mclib/block/BlockEntity.h>

#include <unordered_map>

namespace mc {
namespace block {

class Jukebox : public BlockEntity {
private:
    s32 m_RecordId;
    inventory::Slot m_RecordItem;

public:
    MCLIB_API Jukebox(BlockEntityType type, Vector3i position)
        : BlockEntity(type, position) {}
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    inline s32 GetRecordId() const noexcept { return m_RecordId; }
    inline inventory::Slot GetItem() const noexcept { return m_RecordItem; }
};

}  // namespace block
}  // namespace mc

#endif
