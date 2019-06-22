#ifndef MCLIB_BLOCK_BLOCK_H_
#define MCLIB_BLOCK_BLOCK_H_

#include <mclib/common/AABB.h>
#include <mclib/common/Types.h>
#include <mclib/protocol/ProtocolState.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace mc {
namespace block {

class Block {
protected:
    std::string m_Name;
    u32 m_Data;
    bool m_Solid;
    AABB m_BoundingBox;

public:
    Block(const std::string& name, u32 data, bool solid = true)
        : m_Name(name), m_Data(data), m_Solid(solid) {}

    Block(const std::string& name, u32 type, bool solid, const AABB& bounds)
        : m_Name(name), m_Data(type), m_Solid(solid), m_BoundingBox(bounds) {}
    virtual ~Block() {}

    Block(const Block& other) = delete;
    Block& operator=(const Block& rhs) = delete;
    Block(Block&& other) = delete;
    Block& operator=(Block&& rhs) = delete;

    bool operator==(const Block& other) noexcept {
        return m_Data == other.m_Data;
    }

    virtual std::string GetName() const { return m_Name; }

    u32 GetType() const noexcept { return m_Data; }

    bool IsSolid() const noexcept { return m_Solid; }

    bool IsOpaque() const noexcept {
        return m_BoundingBox.min != mc::Vector3d(0, 0, 0) ||
               m_BoundingBox.max != mc::Vector3d(0, 0, 0);
    }

    void SetBoundingBox(const AABB& bound) noexcept { m_BoundingBox = bound; }

    virtual AABB GetBoundingBox() const noexcept { return m_BoundingBox; }

    // at is the world position of this block. Used to get the world bounding
    // box
    virtual AABB GetBoundingBox(Vector3i at) const {
        Vector3d atd = ToVector3d(at);
        AABB bounds = m_BoundingBox;
        bounds.min += atd;
        bounds.max += atd;
        return bounds;
    }

    virtual AABB GetBoundingBox(Vector3d at) const {
        return GetBoundingBox(ToVector3i(at));
    }

    virtual std::pair<bool, AABB> CollidesWith(Vector3i at, const AABB& other) {
        AABB boundingBox = GetBoundingBox() + at;
        return std::make_pair(boundingBox.Intersects(other), boundingBox);
    }

    // Returns the raw unmodified-by-position bounding boxes.
    virtual std::vector<AABB> GetBoundingBoxes() {
        return std::vector<AABB>(1, m_BoundingBox);
    }

    friend class BlockRegistry;
};
typedef Block* BlockPtr;

class BlockRegistry {
private:
    std::unordered_map<u32, BlockPtr> m_Blocks;
    std::unordered_map<std::string, BlockPtr> m_BlockNames;

    BlockRegistry() {}

public:
    static MCLIB_API BlockRegistry* GetInstance();

    MCLIB_API ~BlockRegistry();

    BlockPtr GetBlock(u32 data) const {
        auto iter = m_Blocks.find(data);

        if (iter == m_Blocks.end()) {
            data &=
                ~15;  // Return basic version if the meta type can't be found
            iter = m_Blocks.find(data);
            if (iter == m_Blocks.end()) return nullptr;
        }
        return iter->second;
    }

    BlockPtr GetBlock(u16 type, u16 meta) const {
        u16 data = (type << 4) | (meta & 15);
        return GetBlock(data);
    }

    BlockPtr MCLIB_API GetBlock(const std::string& name) const;

    void RegisterBlock(BlockPtr block) {
        m_Blocks[block->GetType()] = block;
        m_BlockNames[block->GetName()] = block;
    }

    void MCLIB_API RegisterVanillaBlocks(protocol::Version protocolVersion);
    void MCLIB_API ClearRegistry();
};

}  // namespace block
}  // namespace mc

#endif
