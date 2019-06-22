#ifndef MCLIB_BLOCK_SIGN_H_
#define MCLIB_BLOCK_SIGN_H_

#include <mclib/block/BlockEntity.h>

namespace mc {
namespace block {

class Sign : public BlockEntity {
private:
    std::array<std::wstring, 4> m_Text;

public:
    MCLIB_API Sign(BlockEntityType type, Vector3i position)
        : BlockEntity(type, position) {}
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    MCLIB_API const std::wstring& GetText(std::size_t index) const;
};

}  // namespace block
}  // namespace mc

#endif
