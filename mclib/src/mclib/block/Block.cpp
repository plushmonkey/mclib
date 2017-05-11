#include <mclib/block/Block.h>

// Maybe do virtual getboundingbox based on the position inside of the block.
// This will allow aabb to work easily with stairs

namespace mc {
namespace block {

class StoneBlock : public Block {
public:
    StoneBlock(u16 meta) : Block(L"Stone", 1, meta, true) { }

    std::wstring GetName() const {
        u16 meta = GetMeta();

        static const std::wstring names[] = { L"Stone",
            L"Granite", L"Polished Granite",
            L"Diorite", L"Polished Diorite",
            L"Andesite", L"Polished Andesite" };

        return names[meta];
    }

    static void RegisterBlocks(BlockRegistry* registry) {
        for (int i = 0; i < 7; ++i)
            registry->RegisterBlock(new StoneBlock(i));
    }
};

class DirtBlock : public Block {
public:
    DirtBlock(u16 meta) : Block(L"Dirt", 3, meta, true) { }

    std::wstring GetName() const {
        u16 meta = GetMeta();

        static const std::wstring names[] = { L"Dirt", L"Coarse Dirt", L"Podzol" };

        return names[meta];
    }

    static void RegisterBlocks(BlockRegistry* registry) {
        for (int i = 0; i < 3; ++i)
            registry->RegisterBlock(new DirtBlock(i));
    }
};

class WoodPlanksBlock : public Block {
public:
    WoodPlanksBlock(u16 meta) : Block(L"Wood Planks", 5, meta, true) { }

    std::wstring GetName() const {
        u16 meta = GetMeta();

        static const std::wstring names[] = { L"Oak Wood Planks", L"Spruce Wood Planks",
            L"Birch Wood Planks", L"Jungle Wood Planks",
            L"Acacia Wood Planks", L"Dark Oak Wood Planks" };

        return names[meta];
    }

    static void RegisterBlocks(BlockRegistry* registry) {
        for (int i = 0; i < 6; ++i)
            registry->RegisterBlock(new WoodPlanksBlock(i));
    }
};

class SaplingBlock : public Block {
public:
    SaplingBlock(u16 meta) : Block(L"Sapling", 6, meta, false) { }

    std::wstring GetName() const {
        u16 meta = GetMeta();
        meta &= 7;

        static const std::wstring names[] = { L"Oak Sapling", L"Spruce Sapling",
            L"Birch Sapling", L"Jungle Sapling",
            L"Acacia Sapling", L"Dark Oak Sapling",
        };

        return names[meta];
    }

    static void RegisterBlocks(BlockRegistry* registry) {
        for (int i = 0; i < 6; ++i)
            registry->RegisterBlock(new SaplingBlock(i));
    }
};

class SandBlock : public Block {
public:
    SandBlock(u16 meta) : Block(L"Sand", 12, meta, true) { }

    std::wstring GetName() const {
        u16 meta = GetMeta();

        static const std::wstring names[] = { L"Sand", L"Red Sand" };

        return names[meta];
    }

    static void RegisterBlocks(BlockRegistry* registry) {
        for (int i = 0; i < 2; ++i)
            registry->RegisterBlock(new SandBlock(i));
    }
};

class StairsBlock : public Block {
public:
    enum class Direction { East, West, South, North };

private:
    std::pair<AABB, AABB> GetHalfBounds(const BlockState& blockState) {
        static const AABB LowerHalfBounds(Vector3d(0, 0, 0), Vector3d(1, 0.5, 1));
        static const AABB UpperHalfBounds(Vector3d(0, 0.5, 0), Vector3d(1, 1, 1));
        static const AABB WestEastHalfBounds(Vector3d(0, 0.0, 0), Vector3d(0.5, 1, 1));
        static const AABB NorthSouthHalfBounds(Vector3d(0, 0.0, 0), Vector3d(1, 1, 0.5));

        bool flipped = (blockState.GetData() & 0x4) != 0;
        Direction direction = static_cast<Direction>(blockState.GetData() & 3);

        AABB vertPart;

        switch (direction) {
            case Direction::East:
                vertPart = WestEastHalfBounds + Vector3d(0.5, 0.0, 0.0);
                break;
            case Direction::West:
                vertPart = WestEastHalfBounds;
                break;
            case Direction::South:
                vertPart = NorthSouthHalfBounds + Vector3d(0.0, 0.0, 0.5);
                break;
            case Direction::North:
                vertPart = NorthSouthHalfBounds;
                break;
        }

        AABB horizPart = flipped ? UpperHalfBounds : LowerHalfBounds;

        return std::make_pair(horizPart, vertPart);
    }

public:
    StairsBlock(std::wstring name, u16 type, u16 meta) : Block(name, type, meta, true) {
        static const AABB FullSolidBounds(Vector3d(0, 0, 0), Vector3d(1, 1, 1));
        m_BoundingBox = FullSolidBounds;
    }

    virtual std::vector<AABB> GetBoundingBoxes(const BlockState& blockState) override {
        std::vector<AABB> boundingBoxes;
        
        auto pair = GetHalfBounds(blockState);

        boundingBoxes.push_back(pair.first);
        boundingBoxes.push_back(pair.second);

        return boundingBoxes;
    }

    virtual std::pair<bool, AABB> CollidesWith(const BlockState& blockState, Vector3i at, const AABB& other) override {
        auto pair = GetHalfBounds(blockState);

        AABB horizPart = pair.first + at;
        AABB vertPart = pair.second + at;

        bool vertIntersect = vertPart.Intersects(other);

        if (vertIntersect)
            return std::make_pair(vertIntersect, vertPart);

        return std::make_pair(horizPart.Intersects(other), horizPart);
    }
};

class EndRodBlock : public Block {
private:
    static const AABB BoundsX;
    static const AABB BoundsY;
    static const AABB BoundsZ;

public:
    EndRodBlock(std::wstring name, u16 type, u16 meta) : Block(name, type, meta, true) {
        m_BoundingBox = BoundsX;
    }

    virtual AABB GetBoundingBox(const BlockState& state) const noexcept {
        u16 data = state.GetData() & 15;
        Face face = static_cast<Face>(data);

        switch (face) {
            case Face::Bottom:
            case Face::Top:
                return BoundsY;
            case Face::North:
            case Face::South:
                return BoundsZ;
            case Face::West:
            case Face::East:
                return BoundsX;
        }
        return m_BoundingBox;
    }
};

const AABB EndRodBlock::BoundsX = AABB(Vector3d(0.0, 0.375, 0.375), Vector3d(1.0, 0.625, 0.625));
const AABB EndRodBlock::BoundsY = AABB(Vector3d(0.375, 0.0, 0.375), Vector3d(0.625, 1.0, 0.625));
const AABB EndRodBlock::BoundsZ = AABB(Vector3d(0.375, 0.375, 0.0), Vector3d(0.625, 0.625, 1.0));

BlockRegistry* BlockRegistry::GetInstance() {
    static BlockRegistry registry;
    return &registry;
}

BlockRegistry::~BlockRegistry() {
    ClearRegistry();
}

void BlockRegistry::RegisterVanillaBlocks() {
    const AABB FullSolidBounds(Vector3d(0, 0, 0), Vector3d(1, 1, 1));
    const AABB LowerHalfBounds(Vector3d(0, 0, 0), Vector3d(1, 0.5, 1));
    const AABB UpperHalfBounds(Vector3d(0, 0.5, 0), Vector3d(1, 1, 1));
    const AABB CarpetBounds(Vector3d(0, 0, 0), Vector3d(1, 0.0625, 1));

    this->RegisterBlock(new Block(L"Air", 0, 0, false));

    StoneBlock::RegisterBlocks(this);

    this->RegisterBlock(new Block(L"Grass", 2, 0, true));

    DirtBlock::RegisterBlocks(this);

    this->RegisterBlock(new Block(L"Cobblestone", 4, 0, true));

    WoodPlanksBlock::RegisterBlocks(this);
    SaplingBlock::RegisterBlocks(this);

    this->RegisterBlock(new Block(L"Bedrock", 7, 0, true));

    // TODO: Handle the data values for these
    this->RegisterBlock(new Block(L"Flowing Water", 8, 0, false));
    this->RegisterBlock(new Block(L"Still Water", 9, 0, false));
    this->RegisterBlock(new Block(L"Flowing Lava", 10, 0, false));
    this->RegisterBlock(new Block(L"Still Lava", 11, 0, false));

    SandBlock::RegisterBlocks(this);

    this->RegisterBlock(new Block(L"Gravel", 13, 0, true));
    this->RegisterBlock(new Block(L"Gold Ore", 14, 0, true));
    this->RegisterBlock(new Block(L"Iron Ore", 15, 0, true));
    this->RegisterBlock(new Block(L"Coal Ore", 16, 0, true));
    this->RegisterBlock(new Block(L"Oak Wood", 17, 0, true));
    this->RegisterBlock(new Block(L"Spruce Wood", 17, 1, true));
    this->RegisterBlock(new Block(L"Birch Wood", 17, 2, true));
    this->RegisterBlock(new Block(L"Jungle Wood", 17, 3, true));
    this->RegisterBlock(new Block(L"Oak Leaves", 18, 0, true));
    this->RegisterBlock(new Block(L"Spruce Leaves", 18, 1, true));
    this->RegisterBlock(new Block(L"Birch Leaves", 18, 2, true));
    this->RegisterBlock(new Block(L"Jungle Leaves", 18, 3, true));
    this->RegisterBlock(new Block(L"Sponge", 19, 0, true));
    this->RegisterBlock(new Block(L"Wet Sponge", 19, 1, true));
    this->RegisterBlock(new Block(L"Glass", 20, 0, true));
    this->RegisterBlock(new Block(L"Lapis Lazuli Ore", 21, 0, true));
    this->RegisterBlock(new Block(L"Lapis Lazuli Block", 22, 0, true));
    this->RegisterBlock(new Block(L"Dispenser", 23, 0, true));
    this->RegisterBlock(new Block(L"Sandstone", 24, 0, true));
    this->RegisterBlock(new Block(L"Chiseled Sandstone", 24, 1, true));
    this->RegisterBlock(new Block(L"Smooth Sandstone", 24, 2, true));
    this->RegisterBlock(new Block(L"Note Block", 25, 0, true));
    this->RegisterBlock(new Block(L"Bed", 26, 0, true));
    this->RegisterBlock(new Block(L"Powered Rail", 27, 0, false));
    this->RegisterBlock(new Block(L"Detector Rail", 28, 0, false));
    this->RegisterBlock(new Block(L"Sticky Piston", 29, 0, true));
    this->RegisterBlock(new Block(L"Cobweb", 30, 0, false));
    this->RegisterBlock(new Block(L"Dead Shrub", 31, 0, false));
    this->RegisterBlock(new Block(L"Grass", 31, 1, false));
    this->RegisterBlock(new Block(L"Fern", 31, 2, false));
    this->RegisterBlock(new Block(L"Dead Bush", 32, 0, false));
    this->RegisterBlock(new Block(L"Piston", 33, 0, true));
    this->RegisterBlock(new Block(L"Piston Head", 34, 0, true));
    this->RegisterBlock(new Block(L"White Wool", 35, 0, true));
    this->RegisterBlock(new Block(L"Orange Wool", 35, 1, true));
    this->RegisterBlock(new Block(L"Magenta Wool", 35, 2, true));
    this->RegisterBlock(new Block(L"Light Blue Wool", 35, 3, true));
    this->RegisterBlock(new Block(L"Yellow Wool", 35, 4, true));
    this->RegisterBlock(new Block(L"Lime Wool", 35, 5, true));
    this->RegisterBlock(new Block(L"Pink Wool", 35, 6, true));
    this->RegisterBlock(new Block(L"Gray Wool", 35, 7, true));
    this->RegisterBlock(new Block(L"Light Gray Wool", 35, 8, true));
    this->RegisterBlock(new Block(L"Cyan Wool", 35, 9, true));
    this->RegisterBlock(new Block(L"Purple Wool", 35, 10, true));
    this->RegisterBlock(new Block(L"Blue Wool", 35, 11, true));
    this->RegisterBlock(new Block(L"Brown Wool", 35, 12, true));
    this->RegisterBlock(new Block(L"Green Wool", 35, 13, true));
    this->RegisterBlock(new Block(L"Red Wool", 35, 14, true));
    this->RegisterBlock(new Block(L"Black Wool", 35, 15, true));
    this->RegisterBlock(new Block(L"Dandelion", 37, 0, false));
    this->RegisterBlock(new Block(L"Poppy", 38, 0, false));
    this->RegisterBlock(new Block(L"Blue Orchid", 38, 1, false));
    this->RegisterBlock(new Block(L"Allium", 38, 2, false));
    this->RegisterBlock(new Block(L"Azure Bluet", 38, 3, false));
    this->RegisterBlock(new Block(L"Red Tulip", 38, 4, false));
    this->RegisterBlock(new Block(L"Orange Tulip", 38, 5, false));
    this->RegisterBlock(new Block(L"White Tulip", 38, 6, false));
    this->RegisterBlock(new Block(L"Pink Tulip", 38, 7, false));
    this->RegisterBlock(new Block(L"Oxeye Daisy", 38, 8, false));
    this->RegisterBlock(new Block(L"Brown Mushroom", 39, 0, false));
    this->RegisterBlock(new Block(L"Red Mushroom", 40, 0, false));
    this->RegisterBlock(new Block(L"Gold Block", 41, 0, true));
    this->RegisterBlock(new Block(L"Iron Block", 42, 0, true));
    this->RegisterBlock(new Block(L"Double Stone Slab", 43, 0, true));
    this->RegisterBlock(new Block(L"Double Sandstone Slab", 43, 1, true));
    this->RegisterBlock(new Block(L"Double Wooden Slab", 43, 2, true));
    this->RegisterBlock(new Block(L"Double Cobblestone Slab", 43, 3, true));
    this->RegisterBlock(new Block(L"Double Brick Slab", 43, 4, true));
    this->RegisterBlock(new Block(L"Double Stone Brick Slab", 43, 5, true));
    this->RegisterBlock(new Block(L"Double Nether Brick Slab", 43, 6, true));
    this->RegisterBlock(new Block(L"Double Quartz Slab", 43, 7, true));


    // Lower Slabs
    this->RegisterBlock(new Block(L"Stone Slab", 44, 0, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Sandstone Slab", 44, 1, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Wooden Slab", 44, 2, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Cobblestone Slab", 44, 3, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Brick Slab", 44, 4, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Stone Brick Slab", 44, 5, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Nether Brick Slab", 44, 6, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Quartz Slab", 44, 7, true, LowerHalfBounds));

    // Upper slabs
    this->RegisterBlock(new Block(L"Upper Stone Slab", 44, 8, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"Upper Sandstone Slab", 44, 9, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"Upper Wooden Slab", 44, 10, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"Upper Cobblestone Slab", 44, 11, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"Upper Bricks Slab", 44, 12, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"Upper Stone Brick Slab", 44, 13, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"Upper Nether Brick Slab", 44, 14, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"Upper Quartz Slab", 44, 15, true, UpperHalfBounds));

    this->RegisterBlock(new Block(L"Bricks", 45, 0, true));
    this->RegisterBlock(new Block(L"TNT", 46, 0, true));
    this->RegisterBlock(new Block(L"Bookshelf", 47, 0, true));
    this->RegisterBlock(new Block(L"Moss Stone", 48, 0, true));
    this->RegisterBlock(new Block(L"Obsidian", 49, 0, true));
    this->RegisterBlock(new Block(L"Torch", 50, 0, false));
    this->RegisterBlock(new Block(L"Fire", 51, 0, false));
    this->RegisterBlock(new Block(L"Monster Spawner", 52, 0, true));
    this->RegisterBlock(new StairsBlock(L"Oak Wood Stairs", 53, 0));
    this->RegisterBlock(new Block(L"Chest", 54, 0, true));
    this->RegisterBlock(new Block(L"Redstone Wire", 55, 0, false));
    this->RegisterBlock(new Block(L"Diamond Ore", 56, 0, true));
    this->RegisterBlock(new Block(L"Diamond Block", 57, 0, true));
    this->RegisterBlock(new Block(L"Crafting Table", 58, 0, true));
    this->RegisterBlock(new Block(L"Wheat Crops", 59, 0, false));
    this->RegisterBlock(new Block(L"Farmland", 60, 0, true));
    this->RegisterBlock(new Block(L"Furnace", 61, 0, true));
    this->RegisterBlock(new Block(L"Burning Furnace", 62, 0, true));
    this->RegisterBlock(new Block(L"Standing Sign Block", 63, 0, false));
    this->RegisterBlock(new Block(L"Oak Door Block", 64, 0, true));
    this->RegisterBlock(new Block(L"Ladder", 65, 0, false));
    this->RegisterBlock(new Block(L"Rail", 66, 0, false));
    this->RegisterBlock(new StairsBlock(L"Cobblestone Stairs", 67, 0));
    this->RegisterBlock(new Block(L"Wall-mounted Sign Block", 68, 0, false));
    this->RegisterBlock(new Block(L"Lever", 69, 0, false));
    this->RegisterBlock(new Block(L"Stone Pressure Plate", 70, 0, false));
    this->RegisterBlock(new Block(L"Iron Door Block", 71, 0, true));
    this->RegisterBlock(new Block(L"Wooden Pressure Plate", 72, 0, false));
    this->RegisterBlock(new Block(L"Redstone Ore", 73, 0, true));
    this->RegisterBlock(new Block(L"Glowing Redstone Ore", 74, 0, true));
    this->RegisterBlock(new Block(L"Redstone Torch (off)", 75, 0, false));
    this->RegisterBlock(new Block(L"Redstone Torch (on)", 76, 0, false));
    this->RegisterBlock(new Block(L"Stone Button", 77, 0, false));
    this->RegisterBlock(new Block(L"Snow", 78, 0, false));
    this->RegisterBlock(new Block(L"Ice", 79, 0, true));
    this->RegisterBlock(new Block(L"Snow Block", 80, 0, true));
    this->RegisterBlock(new Block(L"Cactus", 81, 0, true));
    this->RegisterBlock(new Block(L"Clay", 82, 0, true));
    this->RegisterBlock(new Block(L"Sugar Canes", 83, 0, false));
    this->RegisterBlock(new Block(L"Jukebox", 84, 0, true));
    this->RegisterBlock(new Block(L"Oak Fence", 85, 0, true));
    this->RegisterBlock(new Block(L"Pumpkin", 86, 0, true));
    this->RegisterBlock(new Block(L"Netherrack", 87, 0, true));
    this->RegisterBlock(new Block(L"Soul Sand", 88, 0, true));
    this->RegisterBlock(new Block(L"Glowstone", 89, 0, true));
    this->RegisterBlock(new Block(L"Nether Portal", 90, 0, false));
    this->RegisterBlock(new Block(L"Jack o'Lantern", 91, 0, true));
    this->RegisterBlock(new Block(L"Cake Block", 92, 0, false));
    this->RegisterBlock(new Block(L"Redstone Repeater Block (off)", 93, 0, false));
    this->RegisterBlock(new Block(L"Redstone Repeater Block (on)", 94, 0, false));
    this->RegisterBlock(new Block(L"White Stained Glass", 95, 0, true));
    this->RegisterBlock(new Block(L"Orange Stained Glass", 95, 1, true));
    this->RegisterBlock(new Block(L"Magenta Stained Glass", 95, 2, true));
    this->RegisterBlock(new Block(L"Light Blue Stained Glass", 95, 3, true));
    this->RegisterBlock(new Block(L"Yellow Stained Glass", 95, 4, true));
    this->RegisterBlock(new Block(L"Lime Stained Glass", 95, 5, true));
    this->RegisterBlock(new Block(L"Pink Stained Glass", 95, 6, true));
    this->RegisterBlock(new Block(L"Gray Stained Glass", 95, 7, true));
    this->RegisterBlock(new Block(L"Light Gray Stained Glass", 95, 8, true));
    this->RegisterBlock(new Block(L"Cyan Stained Glass", 95, 9, true));
    this->RegisterBlock(new Block(L"Purple Stained Glass", 95, 10, true));
    this->RegisterBlock(new Block(L"Blue Stained Glass", 95, 11, true));
    this->RegisterBlock(new Block(L"Brown Stained Glass", 95, 12, true));
    this->RegisterBlock(new Block(L"Green Stained Glass", 95, 13, true));
    this->RegisterBlock(new Block(L"Red Stained Glass", 95, 14, true));
    this->RegisterBlock(new Block(L"Black Stained Glass", 95, 15, true));
    this->RegisterBlock(new Block(L"Wooden Trapdoor", 96, 0, true));
    this->RegisterBlock(new Block(L"Stone Monster Egg", 97, 0, true));
    this->RegisterBlock(new Block(L"Cobblestone Monster Egg", 97, 1, true));
    this->RegisterBlock(new Block(L"Stone Brick Monster Egg", 97, 2, true));
    this->RegisterBlock(new Block(L"Mossy Stone Brick Monster Egg", 97, 3, true));
    this->RegisterBlock(new Block(L"Cracked Stone Brick Monster Egg", 97, 4, true));
    this->RegisterBlock(new Block(L"Chiseled Stone Brick Monster Egg", 97, 5, true));
    this->RegisterBlock(new Block(L"Stone Bricks", 98, 0, true));
    this->RegisterBlock(new Block(L"Mossy Stone Bricks", 98, 1, true));
    this->RegisterBlock(new Block(L"Cracked Stone Bricks", 98, 2, true));
    this->RegisterBlock(new Block(L"Chiseled Stone Bricks", 98, 3, true));
    this->RegisterBlock(new Block(L"Brown Mushroom Block", 99, 0, true));
    this->RegisterBlock(new Block(L"Red Mushroom Block", 100, 0, true));
    this->RegisterBlock(new Block(L"Iron Bars", 101, 0, true));
    this->RegisterBlock(new Block(L"Glass Pane", 102, 0, true));
    this->RegisterBlock(new Block(L"Melon Block", 103, 0, true));
    this->RegisterBlock(new Block(L"Pumpkin Stem", 104, 0, false));
    this->RegisterBlock(new Block(L"Melon Stem", 105, 0, false));
    this->RegisterBlock(new Block(L"Vines", 106, 0, false));
    this->RegisterBlock(new Block(L"Oak Fence Gate", 107, 0, true));
    this->RegisterBlock(new StairsBlock(L"Brick Stairs", 108, 0));
    this->RegisterBlock(new Block(L"Stone Brick Stairs", 109, 0, true));
    this->RegisterBlock(new Block(L"Mycelium", 110, 0, true));
    this->RegisterBlock(new Block(L"Lily Pad", 111, 0, true));
    this->RegisterBlock(new Block(L"Nether Brick", 112, 0, true));
    this->RegisterBlock(new Block(L"Nether Brick Fence", 113, 0, true));
    this->RegisterBlock(new StairsBlock(L"Nether Brick Stairs", 114, 0));
    this->RegisterBlock(new Block(L"Nether Wart", 115, 0, false));
    this->RegisterBlock(new Block(L"Enchantment Table", 116, 0, true));
    this->RegisterBlock(new Block(L"Brewing Stand", 117, 0, true));
    this->RegisterBlock(new Block(L"Cauldron", 118, 0, true));
    this->RegisterBlock(new Block(L"End Portal", 119, 0, false));
    this->RegisterBlock(new Block(L"End Portal Frame", 120, 0, true));
    this->RegisterBlock(new Block(L"End Stone", 121, 0, true));
    this->RegisterBlock(new Block(L"Dragon Egg", 122, 0, true));
    this->RegisterBlock(new Block(L"Redstone Lamp (inactive)", 123, 0, true));
    this->RegisterBlock(new Block(L"Redstone Lamp (active)", 124, 0, true));
    this->RegisterBlock(new Block(L"Double Oak Wood Slab", 125, 0, true));
    this->RegisterBlock(new Block(L"Double Spruce Wood Slab", 125, 1, true));
    this->RegisterBlock(new Block(L"Double Birch Wood Slab", 125, 2, true));
    this->RegisterBlock(new Block(L"Double Jungle Wood Slab", 125, 3, true));
    this->RegisterBlock(new Block(L"Double Acacia Wood Slab", 125, 4, true));
    this->RegisterBlock(new Block(L"Double Dark Oak Wood Slab", 125, 5, true));
    this->RegisterBlock(new Block(L"Oak Wood Slab", 126, 0, true));
    this->RegisterBlock(new Block(L"Spruce Wood Slab", 126, 1, true));
    this->RegisterBlock(new Block(L"Birch Wood Slab", 126, 2, true));
    this->RegisterBlock(new Block(L"Jungle Wood Slab", 126, 3, true));
    this->RegisterBlock(new Block(L"Acacia Wood Slab", 126, 4, true));
    this->RegisterBlock(new Block(L"Dark Oak Wood Slab", 126, 5, true));
    this->RegisterBlock(new Block(L"Cocoa", 127, 0, true));
    this->RegisterBlock(new Block(L"Sandstone Stairs", 128, 0, true));
    this->RegisterBlock(new Block(L"Emerald Ore", 129, 0, true));
    this->RegisterBlock(new Block(L"Ender Chest", 130, 0, true));
    this->RegisterBlock(new Block(L"Tripwire Hook", 131, 0, false));
    this->RegisterBlock(new Block(L"Tripwire", 132, 0, false));
    this->RegisterBlock(new Block(L"Emerald Block", 133, 0, true));
    this->RegisterBlock(new StairsBlock(L"Spruce Wood Stairs", 134, 0));
    this->RegisterBlock(new StairsBlock(L"Birch Wood Stairs", 135, 0));
    this->RegisterBlock(new StairsBlock(L"Jungle Wood Stairs", 136, 0));
    this->RegisterBlock(new Block(L"Command Block", 137, 0, true));
    this->RegisterBlock(new Block(L"Beacon", 138, 0, true));
    this->RegisterBlock(new Block(L"Cobblestone Wall", 139, 0, true));
    this->RegisterBlock(new Block(L"Mossy Cobblestone Wall", 139, 1, true));
    this->RegisterBlock(new Block(L"Flower Pot", 140, 0, true));
    this->RegisterBlock(new Block(L"Carrots", 141, 0, false));
    this->RegisterBlock(new Block(L"Potatoes", 142, 0, false));
    this->RegisterBlock(new Block(L"Wooden Button", 143, 0, false));
    this->RegisterBlock(new Block(L"Mob Head", 144, 0, true));
    this->RegisterBlock(new Block(L"Anvil", 145, 0, true));
    this->RegisterBlock(new Block(L"Trapped Chest", 146, 0, true));
    this->RegisterBlock(new Block(L"Weighted Pressure Plate (light)", 147, 0, false));
    this->RegisterBlock(new Block(L"Weighted Pressure Plate (heavy)", 148, 0, false));
    this->RegisterBlock(new Block(L"Redstone Comparator (inactive)", 149, 0, false));
    this->RegisterBlock(new Block(L"Redstone Comparator (active)", 150, 0, false));
    this->RegisterBlock(new Block(L"Daylight Sensor", 151, 0, false));
    this->RegisterBlock(new Block(L"Redstone Block", 152, 0, true));
    this->RegisterBlock(new Block(L"Nether Quartz Ore", 153, 0, true));
    this->RegisterBlock(new Block(L"Hopper", 154, 0, true));
    this->RegisterBlock(new Block(L"Quartz Block", 155, 0, true));
    this->RegisterBlock(new Block(L"Chiseled Quartz Block", 155, 1, true));
    this->RegisterBlock(new Block(L"Pillar Quartz Block", 155, 2, true));
    this->RegisterBlock(new StairsBlock(L"Quartz Stairs", 156, 0));
    this->RegisterBlock(new Block(L"Activator Rail", 157, 0, false));
    this->RegisterBlock(new Block(L"Dropper", 158, 0, true));
    this->RegisterBlock(new Block(L"White Stained Clay", 159, 0, true));
    this->RegisterBlock(new Block(L"Orange Stained Clay", 159, 1, true));
    this->RegisterBlock(new Block(L"Magenta Stained Clay", 159, 2, true));
    this->RegisterBlock(new Block(L"Light Blue Stained Clay", 159, 3, true));
    this->RegisterBlock(new Block(L"Yellow Stained Clay", 159, 4, true));
    this->RegisterBlock(new Block(L"Lime Stained Clay", 159, 5, true));
    this->RegisterBlock(new Block(L"Pink Stained Clay", 159, 6, true));
    this->RegisterBlock(new Block(L"Gray Stained Clay", 159, 7, true));
    this->RegisterBlock(new Block(L"Light Gray Stained Clay", 159, 8, true));
    this->RegisterBlock(new Block(L"Cyan Stained Clay", 159, 9, true));
    this->RegisterBlock(new Block(L"Purple Stained Clay", 159, 10, true));
    this->RegisterBlock(new Block(L"Blue Stained Clay", 159, 11, true));
    this->RegisterBlock(new Block(L"Brown Stained Clay", 159, 12, true));
    this->RegisterBlock(new Block(L"Green Stained Clay", 159, 13, true));
    this->RegisterBlock(new Block(L"Red Stained Clay", 159, 14, true));
    this->RegisterBlock(new Block(L"Black Stained Clay", 159, 15, true));
    this->RegisterBlock(new Block(L"White Stained Glass Pane", 160, 0, true));
    this->RegisterBlock(new Block(L"Orange Stained Glass Pane", 160, 1, true));
    this->RegisterBlock(new Block(L"Magenta Stained Glass Pane", 160, 2, true));
    this->RegisterBlock(new Block(L"Light Blue Stained Glass Pane", 160, 3, true));
    this->RegisterBlock(new Block(L"Yellow Stained Glass Pane", 160, 4, true));
    this->RegisterBlock(new Block(L"Lime Stained Glass Pane", 160, 5, true));
    this->RegisterBlock(new Block(L"Pink Stained Glass Pane", 160, 6, true));
    this->RegisterBlock(new Block(L"Gray Stained Glass Pane", 160, 7, true));
    this->RegisterBlock(new Block(L"Light Gray Stained Glass Pane", 160, 8, true));
    this->RegisterBlock(new Block(L"Cyan Stained Glass Pane", 160, 9, true));
    this->RegisterBlock(new Block(L"Purple Stained Glass Pane", 160, 10, true));
    this->RegisterBlock(new Block(L"Blue Stained Glass Pane", 160, 11, true));
    this->RegisterBlock(new Block(L"Brown Stained Glass Pane", 160, 12, true));
    this->RegisterBlock(new Block(L"Green Stained Glass Pane", 160, 13, true));
    this->RegisterBlock(new Block(L"Red Stained Glass Pane", 160, 14, true));
    this->RegisterBlock(new Block(L"Black Stained Glass Pane", 160, 15, true));
    this->RegisterBlock(new Block(L"Acacia Leaves", 161, 0, true));
    this->RegisterBlock(new Block(L"Dark Oak Leaves", 161, 1, true));
    this->RegisterBlock(new Block(L"Acacia Wood", 162, 0, true));
    this->RegisterBlock(new Block(L"Dark Oak Wood", 162, 1, true));
    this->RegisterBlock(new StairsBlock(L"Acacia Wood Stairs", 163, 0));
    this->RegisterBlock(new StairsBlock(L"Dark Oak Wood Stairs", 164, 0));
    this->RegisterBlock(new Block(L"Slime Block", 165, 0, true));
    this->RegisterBlock(new Block(L"Barrier", 166, 0, true));
    this->RegisterBlock(new Block(L"Iron Trapdoor", 167, 0, true));
    this->RegisterBlock(new Block(L"Prismarine", 168, 0, true));
    this->RegisterBlock(new Block(L"Prismarine Bricks", 168, 1, true));
    this->RegisterBlock(new Block(L"Dark Prismarine", 168, 2, true));
    this->RegisterBlock(new Block(L"Sea Lantern", 169, 0, true));
    this->RegisterBlock(new Block(L"Hay Bale", 170, 0, true));

    this->RegisterBlock(new Block(L"White Carpet", 171, 0, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Orange Carpet", 171, 1, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Magenta Carpet", 171, 2, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Light Blue Carpet", 171, 3, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Yellow Carpet", 171, 4, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Lime Carpet", 171, 5, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Pink Carpet", 171, 6, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Gray Carpet", 171, 7, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Light Gray Carpet", 171, 8, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Cyan Carpet", 171, 9, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Purple Carpet", 171, 10, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Blue Carpet", 171, 11, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Brown Carpet", 171, 12, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Green Carpet", 171, 13, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Red Carpet", 171, 14, true, CarpetBounds));
    this->RegisterBlock(new Block(L"Black Carpet", 171, 15, true, CarpetBounds));

    this->RegisterBlock(new Block(L"Hardened Clay", 172, 0, true));
    this->RegisterBlock(new Block(L"Block of Coal", 173, 0, true));
    this->RegisterBlock(new Block(L"Packed Ice", 174, 0, true));
    this->RegisterBlock(new Block(L"Sunflower", 175, 0, false));
    this->RegisterBlock(new Block(L"Lilac", 175, 1, false));
    this->RegisterBlock(new Block(L"Double Tallgrass", 175, 2, false));
    this->RegisterBlock(new Block(L"Large Fern", 175, 3, false));
    this->RegisterBlock(new Block(L"Rose Bush", 175, 4, false));
    this->RegisterBlock(new Block(L"Peony", 175, 5, false));
    this->RegisterBlock(new Block(L"Free-standing Banner", 176, 0, false));
    this->RegisterBlock(new Block(L"Wall-mounted Banner", 177, 0, false));
    this->RegisterBlock(new Block(L"Inverted Daylight Sensor", 178, 0, false));
    this->RegisterBlock(new Block(L"Red Sandstone", 179, 0, true));
    this->RegisterBlock(new Block(L"Chiseled Red Sandstone", 179, 1, true));
    this->RegisterBlock(new Block(L"Smooth Red Sandstone", 179, 2, true));
    this->RegisterBlock(new StairsBlock(L"Red Sandstone Stairs", 180, 0));
    this->RegisterBlock(new Block(L"Double Red Sandstone Slab", 181, 0, true));
    this->RegisterBlock(new Block(L"Red Sandstone Slab", 182, 0, true));
    this->RegisterBlock(new Block(L"Spruce Fence Gate", 183, 0, true));
    this->RegisterBlock(new Block(L"Birch Fence Gate", 184, 0, true));
    this->RegisterBlock(new Block(L"Jungle Fence Gate", 185, 0, true));
    this->RegisterBlock(new Block(L"Dark Oak Fence Gate", 186, 0, true));
    this->RegisterBlock(new Block(L"Acacia Fence Gate", 187, 0, true));
    this->RegisterBlock(new Block(L"Spruce Fence", 188, 0, true));
    this->RegisterBlock(new Block(L"Birch Fence", 189, 0, true));
    this->RegisterBlock(new Block(L"Jungle Fence", 190, 0, true));
    this->RegisterBlock(new Block(L"Dark Oak Fence", 191, 0, true));
    this->RegisterBlock(new Block(L"Acacia Fence", 192, 0, true));
    this->RegisterBlock(new Block(L"Spruce Door Block", 193, 0, true));
    this->RegisterBlock(new Block(L"Birch Door Block", 194, 0, true));
    this->RegisterBlock(new Block(L"Jungle Door Block", 195, 0, true));
    this->RegisterBlock(new Block(L"Acacia Door Block", 196, 0, true));
    this->RegisterBlock(new Block(L"Dark Oak Door Block", 197, 0, true));

    this->RegisterBlock(new EndRodBlock(L"End Rod", 198, 0));

    this->RegisterBlock(new Block(L"Chorus Plant", 199, 0, true));
    this->RegisterBlock(new Block(L"Chorus Plant", 200, 0, true));
    this->RegisterBlock(new Block(L"Purpur Block", 201, 0, true));
    this->RegisterBlock(new Block(L"Purpur Pillar", 202, 0, true));
    this->RegisterBlock(new StairsBlock(L"Purpur Stairs", 203, 0));
    this->RegisterBlock(new Block(L"Purpur Double Slab", 204, 0, true));
    this->RegisterBlock(new Block(L"Purpur Slab", 205, 0, true, LowerHalfBounds));
    this->RegisterBlock(new Block(L"Purpur Slab", 205, 8, true, UpperHalfBounds));
    this->RegisterBlock(new Block(L"End Stone Bricks", 206, 0, true));
    this->RegisterBlock(new Block(L"Beetroot Block", 207, 0, false));
    this->RegisterBlock(new Block(L"Grass Path", 208, 0, true));
    this->RegisterBlock(new Block(L"End Gateway", 209, 0, false));
    this->RegisterBlock(new Block(L"Repeating Command Block", 210, 0, true));
    this->RegisterBlock(new Block(L"Chain Command Block", 211, 0, true));
    this->RegisterBlock(new Block(L"Frosted Ice", 212, 0, true));
    this->RegisterBlock(new Block(L"Magma Block", 213, 0, true));
    this->RegisterBlock(new Block(L"Nether Wart Block", 214, 0, true));
    this->RegisterBlock(new Block(L"Red Nether Brick", 215, 0, true));
    this->RegisterBlock(new Block(L"Bone Block", 216, 0, true));
    this->RegisterBlock(new Block(L"Structure Void", 217, 0, false));
    this->RegisterBlock(new Block(L"Observer", 218, 0, true));

    this->RegisterBlock(new Block(L"White Shulker Box", 219, 0, true));
    this->RegisterBlock(new Block(L"Orange Shulker Box", 220, 0, true));
    this->RegisterBlock(new Block(L"Magenta Shulker Box", 221, 0, true));
    this->RegisterBlock(new Block(L"Light Blue Shulker Box", 222, 0, true));
    this->RegisterBlock(new Block(L"Yellow Shulker Box", 223, 0, true));
    this->RegisterBlock(new Block(L"Lime Shulker Box", 224, 0, true));
    this->RegisterBlock(new Block(L"Pink Shulker Box", 225, 0, true));
    this->RegisterBlock(new Block(L"Gray Shulker Box", 226, 0, true));
    this->RegisterBlock(new Block(L"Light Gray Shulker Box", 227, 0, true));
    this->RegisterBlock(new Block(L"Cyan Shulker Box", 228, 0, true));
    this->RegisterBlock(new Block(L"Purple Shulker Box", 229, 0, true));
    this->RegisterBlock(new Block(L"Blue Shulker Box", 230, 0, true));
    this->RegisterBlock(new Block(L"Brown Shulker Box", 231, 0, true));
    this->RegisterBlock(new Block(L"Green Shulker Box", 232, 0, true));
    this->RegisterBlock(new Block(L"Red Shulker Box", 233, 0, true));
    this->RegisterBlock(new Block(L"Black Shulker Box", 234, 0, true));

    this->RegisterBlock(new Block(L"White Glazed Terracotta", 235, 0, true));
    this->RegisterBlock(new Block(L"Orange Glazed Terracotta", 236, 0, true));
    this->RegisterBlock(new Block(L"Magenta Glazed Terracotta", 237, 0, true));
    this->RegisterBlock(new Block(L"Light Blue Glazed Terracotta", 238, 0, true));
    this->RegisterBlock(new Block(L"Yellow Glazed Terracotta", 239, 0, true));
    this->RegisterBlock(new Block(L"Lime Glazed Terracotta", 240, 0, true));
    this->RegisterBlock(new Block(L"Pink Glazed Terracotta", 241, 0, true));
    this->RegisterBlock(new Block(L"Gray Glazed Terracotta", 242, 0, true));
    this->RegisterBlock(new Block(L"Light Gray Glazed Terracotta", 243, 0, true));
    this->RegisterBlock(new Block(L"Cyan Glazed Terracotta", 244, 0, true));
    this->RegisterBlock(new Block(L"Purple Glazed Terracotta", 245, 0, true));
    this->RegisterBlock(new Block(L"Blue Glazed Terracotta", 246, 0, true));
    this->RegisterBlock(new Block(L"Brown Glazed Terracotta", 247, 0, true));
    this->RegisterBlock(new Block(L"Green Glazed Terracotta", 248, 0, true));
    this->RegisterBlock(new Block(L"Red Glazed Terracotta", 249, 0, true));
    this->RegisterBlock(new Block(L"Black Glazed Terracotta", 250, 0, true));

    this->RegisterBlock(new Block(L"White Concrete", 251, 0, true));
    this->RegisterBlock(new Block(L"Orange Concrete", 251, 1, true));
    this->RegisterBlock(new Block(L"Magenta Concrete", 251, 2, true));
    this->RegisterBlock(new Block(L"Light Blue Concrete", 251, 3, true));
    this->RegisterBlock(new Block(L"Yellow Concrete", 251, 4, true));
    this->RegisterBlock(new Block(L"Lime Concrete", 251, 5, true));
    this->RegisterBlock(new Block(L"Pink Concrete", 251, 6, true));
    this->RegisterBlock(new Block(L"Gray Concrete", 251, 7, true));
    this->RegisterBlock(new Block(L"Light Gray Concrete", 251, 8, true));
    this->RegisterBlock(new Block(L"Cyan Concrete", 251, 9, true));
    this->RegisterBlock(new Block(L"Purple Concrete", 251, 10, true));
    this->RegisterBlock(new Block(L"Blue Concrete", 251, 11, true));
    this->RegisterBlock(new Block(L"Brown Concrete", 251, 12, true));
    this->RegisterBlock(new Block(L"Green Concrete", 251, 13, true));
    this->RegisterBlock(new Block(L"Red Concrete", 251, 14, true));
    this->RegisterBlock(new Block(L"Black Concrete", 251, 15, true));

    this->RegisterBlock(new Block(L"White Concrete Powder", 252, 0, true));
    this->RegisterBlock(new Block(L"Orange Concrete Powder", 252, 1, true));
    this->RegisterBlock(new Block(L"Magenta Concrete Powder", 252, 2, true));
    this->RegisterBlock(new Block(L"Light Blue Concrete Powder", 252, 3, true));
    this->RegisterBlock(new Block(L"Yellow Concrete Powder", 252, 4, true));
    this->RegisterBlock(new Block(L"Lime Concrete Powder", 252, 5, true));
    this->RegisterBlock(new Block(L"Pink Concrete Powder", 252, 6, true));
    this->RegisterBlock(new Block(L"Gray Concrete Powder", 252, 7, true));
    this->RegisterBlock(new Block(L"Light Gray Concrete Powder", 252, 8, true));
    this->RegisterBlock(new Block(L"Cyan Concrete Powder", 252, 9, true));
    this->RegisterBlock(new Block(L"Purple Concrete Powder", 252, 10, true));
    this->RegisterBlock(new Block(L"Blue Concrete Powder", 252, 11, true));
    this->RegisterBlock(new Block(L"Brown Concrete Powder", 252, 12, true));
    this->RegisterBlock(new Block(L"Green Concrete Powder", 252, 13, true));
    this->RegisterBlock(new Block(L"Red Concrete Powder", 252, 14, true));
    this->RegisterBlock(new Block(L"Black Concrete Powder", 252, 15, true));

    this->RegisterBlock(new Block(L"Structure Block", 255, 0, true));

    // Set full boudning box on fully solid blocks
    for (auto kv : m_Blocks) {
        AABB bounds = kv.second->GetBoundingBox();
        if (kv.second->IsSolid() && (bounds.max - bounds.min).Length() == 0)
            kv.second->SetBoundingBox(FullSolidBounds);
    }


}

void BlockRegistry::ClearRegistry() {
    for (auto& pair : m_Blocks) {
        delete pair.second;
    }
    m_Blocks.clear();
}

} // ns block
} // ns mc
