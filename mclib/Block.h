#ifndef BLOCK_H_
#define BLOCK_H_

#include "AABB.h"
#include "Types.h"

#include <unordered_map>
#include <string>

namespace Minecraft {

class Block {
protected:
    std::wstring m_Name;
    u16 m_Data;
    bool m_Solid;
    AABB m_BoundingBox;

public:
    Block(const std::wstring& name, s32 data, bool solid = true) : m_Name(name), m_Data((u16)data), m_Solid(solid) { }
    Block(const std::wstring& name, s32 type, s32 meta, bool solid = true)
        : m_Name(name), m_Data((u16)((type << 4) | (meta & 15))), m_Solid(solid)
    { 

    }

    Block(const std::wstring& name, s32 type, s32 meta, bool solid, const AABB& bounds)
        : m_Name(name), m_Data((u16)((type << 4) | (meta & 15))), m_Solid(solid), m_BoundingBox(bounds)
    {

    }
	
	bool operator==(const Block& other) {
		return m_Data == other.m_Data;
	}

    virtual std::wstring GetName() const { return m_Name; }

    u16 GetType() const {
        return m_Data >> 4;
    }

    u16 GetMeta() const {
        return m_Data & 15;
    }

    bool IsSolid() const {
        return m_Solid;
    }

    void SetBoundingBox(const AABB& bound) {
        m_BoundingBox = bound;
    }

    AABB GetBoundingBox() const {
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

    virtual bool CollidesWith(Vector3d at, const AABB& other) {
        return GetBoundingBox(ToVector3i(at)).Intersects(other);
    }
};
typedef Block* BlockPtr;

class BlockRegistry {
private:
    std::unordered_map<u16, BlockPtr> m_Blocks;

    BlockRegistry() { }
public:
    static MCLIB_API BlockRegistry* GetInstance();

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
};

} // ns Minecraft

#endif
