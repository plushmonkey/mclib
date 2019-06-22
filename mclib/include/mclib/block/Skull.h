#ifndef MCLIB_BLOCK_SKULL_H_
#define MCLIB_BLOCK_SKULL_H_

#include <mclib/block/BlockEntity.h>
#include <mclib/common/UUID.h>

#include <vector>

namespace mc {
namespace block {

enum class SkullType {
    Skeleton,
    WitherSkeleton,
    Zombie,
    Head,
    Creeper,
    Dragon
};

MCLIB_API std::string to_string(SkullType type);
MCLIB_API std::wstring to_wstring(SkullType type);

class Skull : public BlockEntity {
public:
    struct Texture {
        std::wstring signature;
        std::wstring value;
    };

private:
    SkullType m_Type;
    u8 m_Rotation;
    UUID m_OwnerUUID;
    std::wstring m_OwnerName;
    std::vector<Texture> m_Textures;

public:
    MCLIB_API Skull(BlockEntityType type, Vector3i position)
        : BlockEntity(type, position) {}
    MCLIB_API bool ImportNBT(nbt::NBT* nbt);

    inline SkullType GetType() const noexcept { return m_Type; }
    inline u8 GetRotation() const noexcept { return m_Rotation; }
    inline UUID GetOwnerUUID() const noexcept { return m_OwnerUUID; }
    inline const std::wstring& GetOwnerName() const noexcept {
        return m_OwnerName;
    }
    inline const std::vector<Texture>& GetTextures() const noexcept {
        return m_Textures;
    }
};

}  // namespace block
}  // namespace mc

#endif
