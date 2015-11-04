#ifndef BLOCK_H_
#define BLOCK_H_

#include "Types.h"

#include <map>
#include <string>

namespace Minecraft {

class Block {
private:
    std::wstring m_Name;
    bool m_Solid;
    u16 m_Data;

public:
    u16 GetType() const {
        return m_Data >> 4;
    }
    u16 GetMeta() const {
        return m_Data & 15;
    }

    bool IsSolid() const {
        return m_Solid;
    }

    Block(const std::wstring& name, s32 data, bool solid = true) : m_Name(name), m_Data((u16)data), m_Solid(solid) { }
    Block(const std::wstring& name, s32 type, s32 meta, bool solid = true)
        : m_Name(name), m_Data((u16)((type << 4) | (meta & 15))), m_Solid(solid)
    { 

    }
};
typedef Block* BlockPtr;

class BlockRegistry {
private:
    std::map<u16, BlockPtr> m_Blocks;

    BlockRegistry() { }
public:
    static BlockRegistry& GetInstance() {
        static BlockRegistry registry;
        return registry;
    }

    BlockPtr GetBlock(u16 data) {
        if (m_Blocks.find(data) == m_Blocks.end()) {
            data &= ~15; // Return basic version if the meta type can't be found
            if (m_Blocks.find(data) == m_Blocks.end())
                return nullptr;
        }
        return m_Blocks[data];
    }

    BlockPtr GetBlock(u16 type, u16 meta) {
        u16 data = (type << 4) | (meta & 15);
        return GetBlock(data);
    }

    void RegisterBlock(BlockPtr block) {
        u16 type = block->GetType();
        u16 meta = block->GetMeta();
        u16 data = (type << 4) | (meta & 15);

        m_Blocks[data] = block;
    }

    void RegisterVanillaBlocks();
};

} // ns Minecraft

#endif
