#ifndef MCLIB_BLOCK_BLOCK_H_
#define MCLIB_BLOCK_BLOCK_H_

#include <mclib/common/AABB.h>
#include <mclib/common/Types.h>

#include <unordered_map>
#include <string>
#include <vector>

namespace mc {
namespace block {

class BlockState;

class Block {
protected:
    std::wstring m_Name;
    u16 m_Data;
    bool m_Solid;
    AABB m_BoundingBox;

    Block(const std::wstring& name, s32 data, bool solid = true) : m_Name(name), m_Data((u16)data), m_Solid(solid) { }
    Block(const std::wstring& name, s32 type, s32 meta, bool solid = true)
        : m_Name(name), m_Data((u16)((type << 4) | (meta & 15))), m_Solid(solid)
    {

    }

    Block(const std::wstring& name, s32 type, s32 meta, bool solid, const AABB& bounds)
        : m_Name(name), m_Data((u16)((type << 4) | (meta & 15))), m_Solid(solid), m_BoundingBox(bounds)
    {

    }

public:
    virtual ~Block() { }

    Block(const Block& other) = delete;
    Block& operator=(const Block& rhs) = delete;
    Block(Block&& other) = delete;
    Block& operator=(Block&& rhs) = delete;

    bool operator==(const Block& other) noexcept {
        return m_Data == other.m_Data;
    }

    virtual std::wstring GetName() const { return m_Name; }

    u16 GetData() const noexcept {
        return m_Data;
    }

    u16 GetType() const noexcept {
        return m_Data >> 4;
    }

    u16 GetMeta() const noexcept {
        return m_Data & 15;
    }

    bool IsSolid() const noexcept {
        return m_Solid;
    }

    void SetBoundingBox(const AABB& bound) noexcept {
        m_BoundingBox = bound;
    }

    AABB GetBoundingBox() const noexcept {
        return m_BoundingBox;
    }

    virtual AABB GetBoundingBox(const BlockState& state) const noexcept {
        return m_BoundingBox;
    }

    // at is the world position of this block. Used to get the world bounding box
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

    virtual std::pair<bool, AABB> CollidesWith(const BlockState& blockState, Vector3i at, const AABB& other) {
        AABB boundingBox = GetBoundingBox(blockState) + at;
        return std::make_pair(boundingBox.Intersects(other), boundingBox);
    }

    // Returns the raw unmodified-by-position bounding boxes.
    virtual std::vector<AABB> GetBoundingBoxes(const BlockState& blockState) {
        return std::vector<AABB>(1, m_BoundingBox);
    }

    friend class BlockRegistry;
};
typedef Block* BlockPtr;

class BlockState {
private:
    BlockPtr m_Block;
    Vector3i m_Position;
    u16 m_Data;

public:
    BlockState(BlockPtr block, u16 data, Vector3i position) noexcept : m_Block(block), m_Data(data), m_Position(position) { }

    bool operator==(const BlockState& other) { return m_Block == other.m_Block && m_Data == other.m_Data && m_Position == other.m_Position; }

    BlockPtr GetBlock() const noexcept { return m_Block; }
    Vector3i GetPosition() const noexcept { return m_Position; }
    u16 GetData() const noexcept { return m_Data; }
};

class BlockRegistry {
private:
    std::unordered_map<u16, BlockPtr> m_Blocks;

    BlockRegistry() { }
public:
    static MCLIB_API BlockRegistry* GetInstance();

    MCLIB_API ~BlockRegistry();

    BlockPtr GetBlock(u16 data) const {
        auto iter = m_Blocks.find(data);

        if (iter == m_Blocks.end()) {
            data &= ~15; // Return basic version if the meta type can't be found
            iter = m_Blocks.find(data);
            if (iter == m_Blocks.end())
                return nullptr;
        }
        return iter->second;
    }

    BlockPtr GetBlock(u16 type, u16 meta) const {
        u16 data = (type << 4) | (meta & 15);
        return GetBlock(data);
    }

    void RegisterBlock(BlockPtr block) {
        u16 type = block->GetType();
        u16 meta = block->GetMeta();
        u16 data = (type << 4) | (meta & 15);

        m_Blocks[data] = block;
    }

    void MCLIB_API RegisterVanillaBlocks();
    void MCLIB_API ClearRegistry();
};

} // ns block
} // ns mc

#endif
