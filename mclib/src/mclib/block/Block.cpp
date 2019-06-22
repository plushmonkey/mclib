#include <mclib/block/Block.h>

namespace {

void Register1_12() {
    mc::block::BlockRegistry* registry =
        mc::block::BlockRegistry::GetInstance();

    using mc::block::Block;

    const mc::AABB FullSolidBounds(mc::Vector3d(0, 0, 0),
                                   mc::Vector3d(1, 1, 1));
    const mc::AABB LowerHalfBounds(mc::Vector3d(0, 0, 0),
                                   mc::Vector3d(1, 0.5, 1));
    const mc::AABB UpperHalfBounds(mc::Vector3d(0, 0.5, 0),
                                   mc::Vector3d(1, 1, 1));
    const mc::AABB CarpetBounds(mc::Vector3d(0, 0, 0),
                                mc::Vector3d(1, 0.0625, 1));

    // TODO: Collisions

    registry->RegisterBlock(new Block("minecraft:air", 0, false));
    registry->RegisterBlock(new Block("minecraft:stone", 16, true));
    registry->RegisterBlock(new Block("minecraft:stone", 17, true));
    registry->RegisterBlock(new Block("minecraft:stone", 18, true));
    registry->RegisterBlock(new Block("minecraft:stone", 19, true));
    registry->RegisterBlock(new Block("minecraft:stone", 20, true));
    registry->RegisterBlock(new Block("minecraft:stone", 21, true));
    registry->RegisterBlock(new Block("minecraft:stone", 22, true));
    registry->RegisterBlock(new Block("minecraft:grass", 32, true));
    registry->RegisterBlock(new Block("minecraft:dirt", 48, true));
    registry->RegisterBlock(new Block("minecraft:dirt", 49, true));
    registry->RegisterBlock(new Block("minecraft:dirt", 50, true));
    registry->RegisterBlock(new Block("minecraft:cobblestone", 64, true));
    registry->RegisterBlock(new Block("minecraft:planks", 80, true));
    registry->RegisterBlock(new Block("minecraft:planks", 81, true));
    registry->RegisterBlock(new Block("minecraft:planks", 82, true));
    registry->RegisterBlock(new Block("minecraft:planks", 83, true));
    registry->RegisterBlock(new Block("minecraft:planks", 84, true));
    registry->RegisterBlock(new Block("minecraft:planks", 85, true));
    registry->RegisterBlock(new Block("minecraft:sapling", 96, true));
    registry->RegisterBlock(new Block("minecraft:sapling", 97, true));
    registry->RegisterBlock(new Block("minecraft:sapling", 98, true));
    registry->RegisterBlock(new Block("minecraft:sapling", 99, true));
    registry->RegisterBlock(new Block("minecraft:sapling", 100, true));
    registry->RegisterBlock(new Block("minecraft:sapling", 101, true));
    registry->RegisterBlock(new Block("minecraft:bedrock", 112, true));
    registry->RegisterBlock(new Block("minecraft:flowing_water", 128, true));
    registry->RegisterBlock(new Block("minecraft:water", 144, true));
    registry->RegisterBlock(new Block("minecraft:flowing_lava", 160, true));
    registry->RegisterBlock(new Block("minecraft:lava", 176, true));
    registry->RegisterBlock(new Block("minecraft:sand", 192, true));
    registry->RegisterBlock(new Block("minecraft:sand", 193, true));
    registry->RegisterBlock(new Block("minecraft:gravel", 208, true));
    registry->RegisterBlock(new Block("minecraft:gold_ore", 224, true));
    registry->RegisterBlock(new Block("minecraft:iron_ore", 240, true));
    registry->RegisterBlock(new Block("minecraft:coal_ore", 256, true));
    registry->RegisterBlock(new Block("minecraft:log", 272, true));
    registry->RegisterBlock(new Block("minecraft:log", 273, true));
    registry->RegisterBlock(new Block("minecraft:log", 274, true));
    registry->RegisterBlock(new Block("minecraft:log", 275, true));
    registry->RegisterBlock(new Block("minecraft:leaves", 288, true));
    registry->RegisterBlock(new Block("minecraft:leaves", 289, true));
    registry->RegisterBlock(new Block("minecraft:leaves", 290, true));
    registry->RegisterBlock(new Block("minecraft:leaves", 291, true));
    registry->RegisterBlock(new Block("minecraft:sponge", 304, true));
    registry->RegisterBlock(new Block("minecraft:sponge", 305, true));
    registry->RegisterBlock(new Block("minecraft:glass", 320, true));
    registry->RegisterBlock(new Block("minecraft:lapis_ore", 336, true));
    registry->RegisterBlock(new Block("minecraft:lapis_block", 352, true));
    registry->RegisterBlock(new Block("minecraft:dispenser", 368, true));
    registry->RegisterBlock(new Block("minecraft:sandstone", 384, true));
    registry->RegisterBlock(new Block("minecraft:sandstone", 385, true));
    registry->RegisterBlock(new Block("minecraft:sandstone", 386, true));
    registry->RegisterBlock(new Block("minecraft:noteblock", 400, true));
    registry->RegisterBlock(new Block("minecraft:bed", 416, true));
    registry->RegisterBlock(new Block("minecraft:golden_rail", 432, true));
    registry->RegisterBlock(new Block("minecraft:detector_rail", 448, true));
    registry->RegisterBlock(new Block("minecraft:sticky_piston", 464, true));
    registry->RegisterBlock(new Block("minecraft:web", 480, true));
    registry->RegisterBlock(new Block("minecraft:tallgrass", 496, true));
    registry->RegisterBlock(new Block("minecraft:tallgrass", 497, true));
    registry->RegisterBlock(new Block("minecraft:tallgrass", 498, true));
    registry->RegisterBlock(new Block("minecraft:deadbush", 512, true));
    registry->RegisterBlock(new Block("minecraft:piston", 528, true));
    registry->RegisterBlock(new Block("minecraft:piston_head", 544, true));
    registry->RegisterBlock(new Block("minecraft:wool", 560, true));
    registry->RegisterBlock(new Block("minecraft:wool", 561, true));
    registry->RegisterBlock(new Block("minecraft:wool", 562, true));
    registry->RegisterBlock(new Block("minecraft:wool", 563, true));
    registry->RegisterBlock(new Block("minecraft:wool", 564, true));
    registry->RegisterBlock(new Block("minecraft:wool", 565, true));
    registry->RegisterBlock(new Block("minecraft:wool", 566, true));
    registry->RegisterBlock(new Block("minecraft:wool", 567, true));
    registry->RegisterBlock(new Block("minecraft:wool", 568, true));
    registry->RegisterBlock(new Block("minecraft:wool", 569, true));
    registry->RegisterBlock(new Block("minecraft:wool", 570, true));
    registry->RegisterBlock(new Block("minecraft:wool", 571, true));
    registry->RegisterBlock(new Block("minecraft:wool", 572, true));
    registry->RegisterBlock(new Block("minecraft:wool", 573, true));
    registry->RegisterBlock(new Block("minecraft:wool", 574, true));
    registry->RegisterBlock(new Block("minecraft:wool", 575, true));
    registry->RegisterBlock(new Block("minecraft:yellow_flower", 592, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 608, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 609, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 610, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 611, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 612, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 613, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 614, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 615, true));
    registry->RegisterBlock(new Block("minecraft:red_flower", 616, true));
    registry->RegisterBlock(new Block("minecraft:brown_mushroom", 624, true));
    registry->RegisterBlock(new Block("minecraft:red_mushroom", 640, true));
    registry->RegisterBlock(new Block("minecraft:gold_block", 656, true));
    registry->RegisterBlock(new Block("minecraft:iron_block", 672, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 688, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 689, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 690, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 691, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 692, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 693, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 694, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab", 695, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 704, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 705, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 706, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 707, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 708, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 709, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 710, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab", 711, true));
    registry->RegisterBlock(new Block("minecraft:brick_block", 720, true));
    registry->RegisterBlock(new Block("minecraft:tnt", 736, true));
    registry->RegisterBlock(new Block("minecraft:bookshelf", 752, true));
    registry->RegisterBlock(
        new Block("minecraft:mossy_cobblestone", 768, true));
    registry->RegisterBlock(new Block("minecraft:obsidian", 784, true));
    registry->RegisterBlock(new Block("minecraft:torch", 800, true));
    registry->RegisterBlock(new Block("minecraft:fire", 816, true));
    registry->RegisterBlock(new Block("minecraft:mob_spawner", 832, true));
    registry->RegisterBlock(new Block("minecraft:oak_stairs", 848, true));
    registry->RegisterBlock(new Block("minecraft:chest", 864, true));
    registry->RegisterBlock(new Block("minecraft:redstone_wire", 880, true));
    registry->RegisterBlock(new Block("minecraft:diamond_ore", 896, true));
    registry->RegisterBlock(new Block("minecraft:diamond_block", 912, true));
    registry->RegisterBlock(new Block("minecraft:crafting_table", 928, true));
    registry->RegisterBlock(new Block("minecraft:wheat", 944, true));
    registry->RegisterBlock(new Block("minecraft:farmland", 960, true));
    registry->RegisterBlock(new Block("minecraft:furnace", 976, true));
    registry->RegisterBlock(new Block("minecraft:lit_furnace", 992, true));
    registry->RegisterBlock(new Block("minecraft:standing_sign", 1008, true));
    registry->RegisterBlock(new Block("minecraft:wooden_door", 1024, true));
    registry->RegisterBlock(new Block("minecraft:ladder", 1040, true));
    registry->RegisterBlock(new Block("minecraft:rail", 1056, true));
    registry->RegisterBlock(new Block("minecraft:stone_stairs", 1072, true));
    registry->RegisterBlock(new Block("minecraft:wall_sign", 1088, true));
    registry->RegisterBlock(new Block("minecraft:lever", 1104, true));
    registry->RegisterBlock(
        new Block("minecraft:stone_pressure_plate", 1120, true));
    registry->RegisterBlock(new Block("minecraft:iron_door", 1136, true));
    registry->RegisterBlock(
        new Block("minecraft:wooden_pressure_plate", 1152, true));
    registry->RegisterBlock(new Block("minecraft:redstone_ore", 1168, true));
    registry->RegisterBlock(
        new Block("minecraft:lit_redstone_ore", 1184, true));
    registry->RegisterBlock(
        new Block("minecraft:unlit_redstone_torch", 1200, true));
    registry->RegisterBlock(new Block("minecraft:redstone_torch", 1216, true));
    registry->RegisterBlock(new Block("minecraft:stone_button", 1232, true));
    registry->RegisterBlock(new Block("minecraft:snow_layer", 1248, true));
    registry->RegisterBlock(new Block("minecraft:ice", 1264, true));
    registry->RegisterBlock(new Block("minecraft:snow", 1280, true));
    registry->RegisterBlock(new Block("minecraft:cactus", 1296, true));
    registry->RegisterBlock(new Block("minecraft:clay", 1312, true));
    registry->RegisterBlock(new Block("minecraft:reeds", 1328, true));
    registry->RegisterBlock(new Block("minecraft:jukebox", 1344, true));
    registry->RegisterBlock(new Block("minecraft:fence", 1360, true));
    registry->RegisterBlock(new Block("minecraft:pumpkin", 1376, true));
    registry->RegisterBlock(new Block("minecraft:netherrack", 1392, true));
    registry->RegisterBlock(new Block("minecraft:soul_sand", 1408, true));
    registry->RegisterBlock(new Block("minecraft:glowstone", 1424, true));
    registry->RegisterBlock(new Block("minecraft:portal", 1440, true));
    registry->RegisterBlock(new Block("minecraft:lit_pumpkin", 1456, true));
    registry->RegisterBlock(new Block("minecraft:cake", 1472, true));
    registry->RegisterBlock(
        new Block("minecraft:unpowered_repeater", 1488, true));
    registry->RegisterBlock(
        new Block("minecraft:powered_repeater", 1504, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1520, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1521, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1522, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1523, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1524, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1525, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1526, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1527, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1528, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1529, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1530, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1531, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1532, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1533, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1534, true));
    registry->RegisterBlock(new Block("minecraft:stained_glass", 1535, true));
    registry->RegisterBlock(new Block("minecraft:trapdoor", 1536, true));
    registry->RegisterBlock(new Block("minecraft:monster_egg", 1552, true));
    registry->RegisterBlock(new Block("minecraft:monster_egg", 1553, true));
    registry->RegisterBlock(new Block("minecraft:monster_egg", 1554, true));
    registry->RegisterBlock(new Block("minecraft:monster_egg", 1555, true));
    registry->RegisterBlock(new Block("minecraft:monster_egg", 1556, true));
    registry->RegisterBlock(new Block("minecraft:monster_egg", 1557, true));
    registry->RegisterBlock(new Block("minecraft:stonebrick", 1568, true));
    registry->RegisterBlock(new Block("minecraft:stonebrick", 1569, true));
    registry->RegisterBlock(new Block("minecraft:stonebrick", 1570, true));
    registry->RegisterBlock(new Block("minecraft:stonebrick", 1571, true));
    registry->RegisterBlock(
        new Block("minecraft:brown_mushroom_block", 1584, true));
    registry->RegisterBlock(
        new Block("minecraft:red_mushroom_block", 1600, true));
    registry->RegisterBlock(new Block("minecraft:iron_bars", 1616, true));
    registry->RegisterBlock(new Block("minecraft:glass_pane", 1632, true));
    registry->RegisterBlock(new Block("minecraft:melon_block", 1648, true));
    registry->RegisterBlock(new Block("minecraft:pumpkin_stem", 1664, true));
    registry->RegisterBlock(new Block("minecraft:melon_stem", 1680, true));
    registry->RegisterBlock(new Block("minecraft:vine", 1696, true));
    registry->RegisterBlock(new Block("minecraft:fence_gate", 1712, true));
    registry->RegisterBlock(new Block("minecraft:brick_stairs", 1728, true));
    registry->RegisterBlock(
        new Block("minecraft:stone_brick_stairs", 1744, true));
    registry->RegisterBlock(new Block("minecraft:mycelium", 1760, true));
    registry->RegisterBlock(new Block("minecraft:waterlily", 1776, true));
    registry->RegisterBlock(new Block("minecraft:nether_brick", 1792, true));
    registry->RegisterBlock(
        new Block("minecraft:nether_brick_fence", 1808, true));
    registry->RegisterBlock(
        new Block("minecraft:nether_brick_stairs", 1824, true));
    registry->RegisterBlock(new Block("minecraft:nether_wart", 1840, true));
    registry->RegisterBlock(
        new Block("minecraft:enchanting_table", 1856, true));
    registry->RegisterBlock(new Block("minecraft:brewing_stand", 1872, true));
    registry->RegisterBlock(new Block("minecraft:cauldron", 1888, true));
    registry->RegisterBlock(new Block("minecraft:end_portal", 1904, true));
    registry->RegisterBlock(
        new Block("minecraft:end_portal_frame", 1920, true));
    registry->RegisterBlock(new Block("minecraft:end_stone", 1936, true));
    registry->RegisterBlock(new Block("minecraft:dragon_egg", 1952, true));
    registry->RegisterBlock(new Block("minecraft:redstone_lamp", 1968, true));
    registry->RegisterBlock(
        new Block("minecraft:lit_redstone_lamp", 1984, true));
    registry->RegisterBlock(
        new Block("minecraft:double_wooden_slab", 2000, true));
    registry->RegisterBlock(
        new Block("minecraft:double_wooden_slab", 2001, true));
    registry->RegisterBlock(
        new Block("minecraft:double_wooden_slab", 2002, true));
    registry->RegisterBlock(
        new Block("minecraft:double_wooden_slab", 2003, true));
    registry->RegisterBlock(
        new Block("minecraft:double_wooden_slab", 2004, true));
    registry->RegisterBlock(
        new Block("minecraft:double_wooden_slab", 2005, true));
    registry->RegisterBlock(new Block("minecraft:wooden_slab", 2016, true));
    registry->RegisterBlock(new Block("minecraft:wooden_slab", 2017, true));
    registry->RegisterBlock(new Block("minecraft:wooden_slab", 2018, true));
    registry->RegisterBlock(new Block("minecraft:wooden_slab", 2019, true));
    registry->RegisterBlock(new Block("minecraft:wooden_slab", 2020, true));
    registry->RegisterBlock(new Block("minecraft:wooden_slab", 2021, true));
    registry->RegisterBlock(new Block("minecraft:cocoa", 2032, true));
    registry->RegisterBlock(
        new Block("minecraft:sandstone_stairs", 2048, true));
    registry->RegisterBlock(new Block("minecraft:emerald_ore", 2064, true));
    registry->RegisterBlock(new Block("minecraft:ender_chest", 2080, true));
    registry->RegisterBlock(new Block("minecraft:tripwire_hook", 2096, true));
    registry->RegisterBlock(new Block("minecraft:tripwire_hook", 2112, true));
    registry->RegisterBlock(new Block("minecraft:emerald_block", 2128, true));
    registry->RegisterBlock(new Block("minecraft:spruce_stairs", 2144, true));
    registry->RegisterBlock(new Block("minecraft:birch_stairs", 2160, true));
    registry->RegisterBlock(new Block("minecraft:jungle_stairs", 2176, true));
    registry->RegisterBlock(new Block("minecraft:command_block", 2192, true));
    registry->RegisterBlock(new Block("minecraft:beacon", 2208, true));
    registry->RegisterBlock(
        new Block("minecraft:cobblestone_wall", 2224, true));
    registry->RegisterBlock(
        new Block("minecraft:cobblestone_wall", 2225, true));
    registry->RegisterBlock(new Block("minecraft:flower_pot", 2240, true));
    registry->RegisterBlock(new Block("minecraft:carrots", 2256, true));
    registry->RegisterBlock(new Block("minecraft:potatoes", 2272, true));
    registry->RegisterBlock(new Block("minecraft:wooden_button", 2288, true));
    registry->RegisterBlock(new Block("minecraft:skull", 2304, true));
    registry->RegisterBlock(new Block("minecraft:anvil", 2320, true));
    registry->RegisterBlock(new Block("minecraft:trapped_chest", 2336, true));
    registry->RegisterBlock(
        new Block("minecraft:light_weighted_pressure_plate", 2352, true));
    registry->RegisterBlock(
        new Block("minecraft:heavy_weighted_pressure_plate", 2368, true));
    registry->RegisterBlock(
        new Block("minecraft:unpowered_comparator", 2384, true));
    registry->RegisterBlock(
        new Block("minecraft:powered_comparator", 2400, true));
    registry->RegisterBlock(
        new Block("minecraft:daylight_detector", 2416, true));
    registry->RegisterBlock(new Block("minecraft:redstone_block", 2432, true));
    registry->RegisterBlock(new Block("minecraft:quartz_ore", 2448, true));
    registry->RegisterBlock(new Block("minecraft:hopper", 2464, true));
    registry->RegisterBlock(new Block("minecraft:quartz_block", 2480, true));
    registry->RegisterBlock(new Block("minecraft:quartz_block", 2481, true));
    registry->RegisterBlock(new Block("minecraft:quartz_block", 2482, true));
    registry->RegisterBlock(new Block("minecraft:quartz_stairs", 2496, true));
    registry->RegisterBlock(new Block("minecraft:activator_rail", 2512, true));
    registry->RegisterBlock(new Block("minecraft:dropper", 2528, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2544, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2545, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2546, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2547, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2548, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2549, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2550, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2551, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2552, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2553, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2554, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2555, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2556, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2557, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2558, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_hardened_clay", 2559, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2560, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2561, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2562, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2563, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2564, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2565, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2566, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2567, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2568, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2569, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2570, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2571, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2572, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2573, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2574, true));
    registry->RegisterBlock(
        new Block("minecraft:stained_glass_pane", 2575, true));
    registry->RegisterBlock(new Block("minecraft:leaves2", 2576, true));
    registry->RegisterBlock(new Block("minecraft:leaves2", 2577, true));
    registry->RegisterBlock(new Block("minecraft:log2", 2592, true));
    registry->RegisterBlock(new Block("minecraft:log2", 2593, true));
    registry->RegisterBlock(new Block("minecraft:acacia_stairs", 2608, true));
    registry->RegisterBlock(new Block("minecraft:dark_oak_stairs", 2624, true));
    registry->RegisterBlock(new Block("minecraft:slime", 2640, true));
    registry->RegisterBlock(new Block("minecraft:barrier", 2656, true));
    registry->RegisterBlock(new Block("minecraft:iron_trapdoor", 2672, true));
    registry->RegisterBlock(new Block("minecraft:prismarine", 2688, true));
    registry->RegisterBlock(new Block("minecraft:prismarine", 2689, true));
    registry->RegisterBlock(new Block("minecraft:prismarine", 2690, true));
    registry->RegisterBlock(new Block("minecraft:sea_lantern", 2704, true));
    registry->RegisterBlock(new Block("minecraft:hay_block", 2720, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2736, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2737, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2738, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2739, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2740, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2741, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2742, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2743, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2744, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2745, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2746, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2747, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2748, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2749, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2750, true));
    registry->RegisterBlock(new Block("minecraft:carpet", 2751, true));
    registry->RegisterBlock(new Block("minecraft:hardened_clay", 2752, true));
    registry->RegisterBlock(new Block("minecraft:coal_block", 2768, true));
    registry->RegisterBlock(new Block("minecraft:packed_ice", 2784, true));
    registry->RegisterBlock(new Block("minecraft:double_plant", 2800, true));
    registry->RegisterBlock(new Block("minecraft:double_plant", 2801, true));
    registry->RegisterBlock(new Block("minecraft:double_plant", 2802, true));
    registry->RegisterBlock(new Block("minecraft:double_plant", 2803, true));
    registry->RegisterBlock(new Block("minecraft:double_plant", 2804, true));
    registry->RegisterBlock(new Block("minecraft:double_plant", 2805, true));
    registry->RegisterBlock(new Block("minecraft:standing_banner", 2816, true));
    registry->RegisterBlock(new Block("minecraft:wall_banner", 2832, true));
    registry->RegisterBlock(
        new Block("minecraft:daylight_detector_inverted", 2848, true));
    registry->RegisterBlock(new Block("minecraft:red_sandstone", 2864, true));
    registry->RegisterBlock(new Block("minecraft:red_sandstone", 2865, true));
    registry->RegisterBlock(new Block("minecraft:red_sandstone", 2866, true));
    registry->RegisterBlock(
        new Block("minecraft:red_sandstone_stairs", 2880, true));
    registry->RegisterBlock(
        new Block("minecraft:double_stone_slab2", 2896, true));
    registry->RegisterBlock(new Block("minecraft:stone_slab2", 2912, true));
    registry->RegisterBlock(
        new Block("minecraft:spruce_fence_gate", 2928, true));
    registry->RegisterBlock(
        new Block("minecraft:birch_fence_gate", 2944, true));
    registry->RegisterBlock(
        new Block("minecraft:jungle_fence_gate", 2960, true));
    registry->RegisterBlock(
        new Block("minecraft:dark_oak_fence_gate", 2976, true));
    registry->RegisterBlock(
        new Block("minecraft:acacia_fence_gate", 2992, true));
    registry->RegisterBlock(new Block("minecraft:spruce_fence", 3008, true));
    registry->RegisterBlock(new Block("minecraft:birch_fence", 3024, true));
    registry->RegisterBlock(new Block("minecraft:jungle_fence", 3040, true));
    registry->RegisterBlock(new Block("minecraft:dark_oak_fence", 3056, true));
    registry->RegisterBlock(new Block("minecraft:acacia_fence", 3072, true));
    registry->RegisterBlock(new Block("minecraft:spruce_door", 3088, true));
    registry->RegisterBlock(new Block("minecraft:birch_door", 3104, true));
    registry->RegisterBlock(new Block("minecraft:jungle_door", 3120, true));
    registry->RegisterBlock(new Block("minecraft:acacia_door", 3136, true));
    registry->RegisterBlock(new Block("minecraft:dark_oak_door", 3152, true));
    registry->RegisterBlock(new Block("minecraft:end_rod", 3168, true));
    registry->RegisterBlock(new Block("minecraft:chorus_plant", 3184, true));
    registry->RegisterBlock(new Block("minecraft:chorus_flower", 3200, true));
    registry->RegisterBlock(new Block("minecraft:purpur_block", 3216, true));
    registry->RegisterBlock(new Block("minecraft:purpur_pillar", 3232, true));
    registry->RegisterBlock(new Block("minecraft:purpur_stairs", 3248, true));
    registry->RegisterBlock(
        new Block("minecraft:purpur_double_slab", 3264, true));
    registry->RegisterBlock(new Block("minecraft:purpur_slab", 3280, true));
    registry->RegisterBlock(new Block("minecraft:end_bricks", 3296, true));
    registry->RegisterBlock(new Block("minecraft:beetroots", 3312, true));
    registry->RegisterBlock(new Block("minecraft:grass_path", 3328, true));
    registry->RegisterBlock(new Block("minecraft:end_gateway", 3344, true));
    registry->RegisterBlock(
        new Block("minecraft:repeating_command_block", 3360, true));
    registry->RegisterBlock(
        new Block("minecraft:chain_command_block", 3376, true));
    registry->RegisterBlock(new Block("minecraft:frosted_ice", 3392, true));
    registry->RegisterBlock(new Block("minecraft:magma", 3408, true));
    registry->RegisterBlock(
        new Block("minecraft:nether_wart_block", 3424, true));
    registry->RegisterBlock(
        new Block("minecraft:red_nether_brick", 3440, true));
    registry->RegisterBlock(new Block("minecraft:bone_block", 3456, true));
    registry->RegisterBlock(new Block("minecraft:structure_void", 3472, true));
    registry->RegisterBlock(new Block("minecraft:observer", 3488, true));
    registry->RegisterBlock(
        new Block("minecraft:white_shulker_box", 3504, true));
    registry->RegisterBlock(
        new Block("minecraft:orange_shulker_box", 3520, true));
    registry->RegisterBlock(
        new Block("minecraft:magenta_shulker_box", 3536, true));
    registry->RegisterBlock(
        new Block("minecraft:light_blue_shulker_box", 3552, true));
    registry->RegisterBlock(
        new Block("minecraft:yellow_shulker_box", 3568, true));
    registry->RegisterBlock(
        new Block("minecraft:lime_shulker_box", 3584, true));
    registry->RegisterBlock(
        new Block("minecraft:pink_shulker_box", 3600, true));
    registry->RegisterBlock(
        new Block("minecraft:gray_shulker_box", 3616, true));
    registry->RegisterBlock(
        new Block("minecraft:silver_shulker_box", 3632, true));
    registry->RegisterBlock(
        new Block("minecraft:cyan_shulker_box", 3648, true));
    registry->RegisterBlock(
        new Block("minecraft:purple_shulker_box", 3664, true));
    registry->RegisterBlock(
        new Block("minecraft:blue_shulker_box", 3680, true));
    registry->RegisterBlock(
        new Block("minecraft:brown_shulker_box", 3696, true));
    registry->RegisterBlock(
        new Block("minecraft:green_shulker_box", 3712, true));
    registry->RegisterBlock(new Block("minecraft:red_shulker_box", 3728, true));
    registry->RegisterBlock(
        new Block("minecraft:black_shulker_box", 3744, true));
    registry->RegisterBlock(
        new Block("minecraft:white_glazed_terracotta", 3760, true));
    registry->RegisterBlock(
        new Block("minecraft:orange_glazed_terracotta", 3776, true));
    registry->RegisterBlock(
        new Block("minecraft:magenta_glazed_terracotta", 3792, true));
    registry->RegisterBlock(
        new Block("minecraft:light_blue_glazed_terracotta", 3808, true));
    registry->RegisterBlock(
        new Block("minecraft:yellow_glazed_terracotta", 3824, true));
    registry->RegisterBlock(
        new Block("minecraft:lime_glazed_terracotta", 3840, true));
    registry->RegisterBlock(
        new Block("minecraft:pink_glazed_terracotta", 3856, true));
    registry->RegisterBlock(
        new Block("minecraft:gray_glazed_terracotta", 3872, true));
    registry->RegisterBlock(
        new Block("minecraft:light_gray_glazed_terracotta", 3888, true));
    registry->RegisterBlock(
        new Block("minecraft:cyan_glazed_terracotta", 3904, true));
    registry->RegisterBlock(
        new Block("minecraft:purple_glazed_terracotta", 3920, true));
    registry->RegisterBlock(
        new Block("minecraft:blue_glazed_terracotta", 3936, true));
    registry->RegisterBlock(
        new Block("minecraft:brown_glazed_terracotta", 3952, true));
    registry->RegisterBlock(
        new Block("minecraft:green_glazed_terracotta", 3968, true));
    registry->RegisterBlock(
        new Block("minecraft:red_glazed_terracotta", 3984, true));
    registry->RegisterBlock(
        new Block("minecraft:black_glazed_terracotta", 4000, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4016, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4017, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4018, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4019, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4020, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4021, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4022, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4023, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4024, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4025, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4026, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4027, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4028, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4029, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4030, true));
    registry->RegisterBlock(new Block("minecraft:concrete", 4031, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4032, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4033, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4034, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4035, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4036, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4037, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4038, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4039, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4040, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4041, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4042, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4043, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4044, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4045, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4046, true));
    registry->RegisterBlock(new Block("minecraft:concrete_powder", 4047, true));
    registry->RegisterBlock(new Block("minecraft:structure_block", 4080, true));
}

}  // namespace

namespace mc {
namespace block {

BlockRegistry* BlockRegistry::GetInstance() {
    static BlockRegistry registry;
    return &registry;
}

BlockRegistry::~BlockRegistry() { ClearRegistry(); }

void BlockRegistry::RegisterVanillaBlocks(protocol::Version protocolVersion) {
    const AABB FullSolidBounds(Vector3d(0, 0, 0), Vector3d(1, 1, 1));

    if (protocolVersion <= protocol::Version::Minecraft_1_12_2) {
        Register1_12();
    } else {
        const char* blockNames[] = {
            "minecraft:air",
            "minecraft:stone",
            "minecraft:granite",
            "minecraft:polished_granite",
            "minecraft:diorite",
            "minecraft:polished_diorite",
            "minecraft:andesite",
            "minecraft:polished_andesite",
            "minecraft:grass_block",
            "minecraft:dirt",
            "minecraft:coarse_dirt",
            "minecraft:podzol",
            "minecraft:cobblestone",
            "minecraft:oak_planks",
            "minecraft:spruce_planks",
            "minecraft:birch_planks",
            "minecraft:jungle_planks",
            "minecraft:acacia_planks",
            "minecraft:dark_oak_planks",
            "minecraft:oak_sapling",
            "minecraft:spruce_sapling",
            "minecraft:birch_sapling",
            "minecraft:jungle_sapling",
            "minecraft:acacia_sapling",
            "minecraft:dark_oak_sapling",
            "minecraft:bedrock",
            "minecraft:water",
            "minecraft:lava",
            "minecraft:sand",
            "minecraft:red_sand",
            "minecraft:gravel",
            "minecraft:gold_ore",
            "minecraft:iron_ore",
            "minecraft:coal_ore",
            "minecraft:oak_log",
            "minecraft:spruce_log",
            "minecraft:birch_log",
            "minecraft:jungle_log",
            "minecraft:acacia_log",
            "minecraft:dark_oak_log",
            "minecraft:stripped_spruce_log",
            "minecraft:stripped_birch_log",
            "minecraft:stripped_jungle_log",
            "minecraft:stripped_acacia_log",
            "minecraft:stripped_dark_oak_log",
            "minecraft:stripped_oak_log",
            "minecraft:oak_wood",
            "minecraft:spruce_wood",
            "minecraft:birch_wood",
            "minecraft:jungle_wood",
            "minecraft:acacia_wood",
            "minecraft:dark_oak_wood",
            "minecraft:stripped_oak_wood",
            "minecraft:stripped_spruce_wood",
            "minecraft:stripped_birch_wood",
            "minecraft:stripped_jungle_wood",
            "minecraft:stripped_acacia_wood",
            "minecraft:stripped_dark_oak_wood",
            "minecraft:oak_leaves",
            "minecraft:spruce_leaves",
            "minecraft:birch_leaves",
            "minecraft:jungle_leaves",
            "minecraft:acacia_leaves",
            "minecraft:dark_oak_leaves",
            "minecraft:sponge",
            "minecraft:wet_sponge",
            "minecraft:glass",
            "minecraft:lapis_ore",
            "minecraft:lapis_block",
            "minecraft:dispenser",
            "minecraft:sandstone",
            "minecraft:chiseled_sandstone",
            "minecraft:cut_sandstone",
            "minecraft:note_block",
            "minecraft:white_bed",
            "minecraft:orange_bed",
            "minecraft:magenta_bed",
            "minecraft:light_blue_bed",
            "minecraft:yellow_bed",
            "minecraft:lime_bed",
            "minecraft:pink_bed",
            "minecraft:gray_bed",
            "minecraft:light_gray_bed",
            "minecraft:cyan_bed",
            "minecraft:purple_bed",
            "minecraft:blue_bed",
            "minecraft:brown_bed",
            "minecraft:green_bed",
            "minecraft:red_bed",
            "minecraft:black_bed",
            "minecraft:powered_rail",
            "minecraft:detector_rail",
            "minecraft:sticky_piston",
            "minecraft:cobweb",
            "minecraft:grass",
            "minecraft:fern",
            "minecraft:dead_bush",
            "minecraft:seagrass",
            "minecraft:tall_seagrass",
            "minecraft:piston",
            "minecraft:piston_head",
            "minecraft:white_wool",
            "minecraft:orange_wool",
            "minecraft:magenta_wool",
            "minecraft:light_blue_wool",
            "minecraft:yellow_wool",
            "minecraft:lime_wool",
            "minecraft:pink_wool",
            "minecraft:gray_wool",
            "minecraft:light_gray_wool",
            "minecraft:cyan_wool",
            "minecraft:purple_wool",
            "minecraft:blue_wool",
            "minecraft:brown_wool",
            "minecraft:green_wool",
            "minecraft:red_wool",
            "minecraft:black_wool",
            "minecraft:moving_piston",
            "minecraft:dandelion",
            "minecraft:poppy",
            "minecraft:blue_orchid",
            "minecraft:allium",
            "minecraft:azure_bluet",
            "minecraft:red_tulip",
            "minecraft:orange_tulip",
            "minecraft:white_tulip",
            "minecraft:pink_tulip",
            "minecraft:oxeye_daisy",
            "minecraft:brown_mushroom",
            "minecraft:red_mushroom",
            "minecraft:gold_block",
            "minecraft:iron_block",
            "minecraft:bricks",
            "minecraft:tnt",
            "minecraft:bookshelf",
            "minecraft:mossy_cobblestone",
            "minecraft:obsidian",
            "minecraft:torch",
            "minecraft:wall_torch",
            "minecraft:fire",
            "minecraft:spawner",
            "minecraft:oak_stairs",
            "minecraft:chest",
            "minecraft:redstone_wire",
            "minecraft:diamond_ore",
            "minecraft:diamond_block",
            "minecraft:crafting_table",
            "minecraft:wheat",
            "minecraft:farmland",
            "minecraft:furnace",
            "minecraft:sign",
            "minecraft:oak_door",
            "minecraft:ladder",
            "minecraft:rail",
            "minecraft:cobblestone_stairs",
            "minecraft:wall_sign",
            "minecraft:lever",
            "minecraft:stone_pressure_plate",
            "minecraft:iron_door",
            "minecraft:oak_pressure_plate",
            "minecraft:spruce_pressure_plate",
            "minecraft:birch_pressure_plate",
            "minecraft:jungle_pressure_plate",
            "minecraft:acacia_pressure_plate",
            "minecraft:dark_oak_pressure_plate",
            "minecraft:redstone_ore",
            "minecraft:redstone_torch",
            "minecraft:redstone_wall_torch",
            "minecraft:stone_button",
            "minecraft:snow",
            "minecraft:ice",
            "minecraft:snow_block",
            "minecraft:cactus",
            "minecraft:clay",
            "minecraft:sugar_cane",
            "minecraft:jukebox",
            "minecraft:oak_fence",
            "minecraft:pumpkin",
            "minecraft:netherrack",
            "minecraft:soul_sand",
            "minecraft:glowstone",
            "minecraft:nether_portal",
            "minecraft:carved_pumpkin",
            "minecraft:jack_o_lantern",
            "minecraft:cake",
            "minecraft:repeater",
            "minecraft:white_stained_glass",
            "minecraft:orange_stained_glass",
            "minecraft:magenta_stained_glass",
            "minecraft:light_blue_stained_glass",
            "minecraft:yellow_stained_glass",
            "minecraft:lime_stained_glass",
            "minecraft:pink_stained_glass",
            "minecraft:gray_stained_glass",
            "minecraft:light_gray_stained_glass",
            "minecraft:cyan_stained_glass",
            "minecraft:purple_stained_glass",
            "minecraft:blue_stained_glass",
            "minecraft:brown_stained_glass",
            "minecraft:green_stained_glass",
            "minecraft:red_stained_glass",
            "minecraft:black_stained_glass",
            "minecraft:oak_trapdoor",
            "minecraft:spruce_trapdoor",
            "minecraft:birch_trapdoor",
            "minecraft:jungle_trapdoor",
            "minecraft:acacia_trapdoor",
            "minecraft:dark_oak_trapdoor",
            "minecraft:infested_stone",
            "minecraft:infested_cobblestone",
            "minecraft:infested_stone_bricks",
            "minecraft:infested_mossy_stone_bricks",
            "minecraft:infested_cracked_stone_bricks",
            "minecraft:infested_chiseled_stone_bricks",
            "minecraft:stone_bricks",
            "minecraft:mossy_stone_bricks",
            "minecraft:cracked_stone_bricks",
            "minecraft:chiseled_stone_bricks",
            "minecraft:brown_mushroom_block",
            "minecraft:red_mushroom_block",
            "minecraft:mushroom_stem",
            "minecraft:iron_bars",
            "minecraft:glass_pane",
            "minecraft:melon",
            "minecraft:attached_pumpkin_stem",
            "minecraft:attached_melon_stem",
            "minecraft:pumpkin_stem",
            "minecraft:melon_stem",
            "minecraft:vine",
            "minecraft:oak_fence_gate",
            "minecraft:brick_stairs",
            "minecraft:stone_brick_stairs",
            "minecraft:mycelium",
            "minecraft:lily_pad",
            "minecraft:nether_bricks",
            "minecraft:nether_brick_fence",
            "minecraft:nether_brick_stairs",
            "minecraft:nether_wart",
            "minecraft:enchanting_table",
            "minecraft:brewing_stand",
            "minecraft:cauldron",
            "minecraft:end_portal",
            "minecraft:end_portal_frame",
            "minecraft:end_stone",
            "minecraft:dragon_egg",
            "minecraft:redstone_lamp",
            "minecraft:cocoa",
            "minecraft:sandstone_stairs",
            "minecraft:emerald_ore",
            "minecraft:ender_chest",
            "minecraft:tripwire_hook",
            "minecraft:tripwire",
            "minecraft:emerald_block",
            "minecraft:spruce_stairs",
            "minecraft:birch_stairs",
            "minecraft:jungle_stairs",
            "minecraft:command_block",
            "minecraft:beacon",
            "minecraft:cobblestone_wall",
            "minecraft:mossy_cobblestone_wall",
            "minecraft:flower_pot",
            "minecraft:potted_oak_sapling",
            "minecraft:potted_spruce_sapling",
            "minecraft:potted_birch_sapling",
            "minecraft:potted_jungle_sapling",
            "minecraft:potted_acacia_sapling",
            "minecraft:potted_dark_oak_sapling",
            "minecraft:potted_fern",
            "minecraft:potted_dandelion",
            "minecraft:potted_poppy",
            "minecraft:potted_blue_orchid",
            "minecraft:potted_allium",
            "minecraft:potted_azure_bluet",
            "minecraft:potted_red_tulip",
            "minecraft:potted_orange_tulip",
            "minecraft:potted_white_tulip",
            "minecraft:potted_pink_tulip",
            "minecraft:potted_oxeye_daisy",
            "minecraft:potted_red_mushroom",
            "minecraft:potted_brown_mushroom",
            "minecraft:potted_dead_bush",
            "minecraft:potted_cactus",
            "minecraft:carrots",
            "minecraft:potatoes",
            "minecraft:oak_button",
            "minecraft:spruce_button",
            "minecraft:birch_button",
            "minecraft:jungle_button",
            "minecraft:acacia_button",
            "minecraft:dark_oak_button",
            "minecraft:skeleton_wall_skull",
            "minecraft:skeleton_skull",
            "minecraft:wither_skeleton_wall_skull",
            "minecraft:wither_skeleton_skull",
            "minecraft:zombie_wall_head",
            "minecraft:zombie_head",
            "minecraft:player_wall_head",
            "minecraft:player_head",
            "minecraft:creeper_wall_head",
            "minecraft:creeper_head",
            "minecraft:dragon_wall_head",
            "minecraft:dragon_head",
            "minecraft:anvil",
            "minecraft:chipped_anvil",
            "minecraft:damaged_anvil",
            "minecraft:trapped_chest",
            "minecraft:light_weighted_pressure_plate",
            "minecraft:heavy_weighted_pressure_plate",
            "minecraft:comparator",
            "minecraft:daylight_detector",
            "minecraft:redstone_block",
            "minecraft:nether_quartz_ore",
            "minecraft:hopper",
            "minecraft:quartz_block",
            "minecraft:chiseled_quartz_block",
            "minecraft:quartz_pillar",
            "minecraft:quartz_stairs",
            "minecraft:activator_rail",
            "minecraft:dropper",
            "minecraft:white_terracotta",
            "minecraft:orange_terracotta",
            "minecraft:magenta_terracotta",
            "minecraft:light_blue_terracotta",
            "minecraft:yellow_terracotta",
            "minecraft:lime_terracotta",
            "minecraft:pink_terracotta",
            "minecraft:gray_terracotta",
            "minecraft:light_gray_terracotta",
            "minecraft:cyan_terracotta",
            "minecraft:purple_terracotta",
            "minecraft:blue_terracotta",
            "minecraft:brown_terracotta",
            "minecraft:green_terracotta",
            "minecraft:red_terracotta",
            "minecraft:black_terracotta",
            "minecraft:white_stained_glass_pane",
            "minecraft:orange_stained_glass_pane",
            "minecraft:magenta_stained_glass_pane",
            "minecraft:light_blue_stained_glass_pane",
            "minecraft:yellow_stained_glass_pane",
            "minecraft:lime_stained_glass_pane",
            "minecraft:pink_stained_glass_pane",
            "minecraft:gray_stained_glass_pane",
            "minecraft:light_gray_stained_glass_pane",
            "minecraft:cyan_stained_glass_pane",
            "minecraft:purple_stained_glass_pane",
            "minecraft:blue_stained_glass_pane",
            "minecraft:brown_stained_glass_pane",
            "minecraft:green_stained_glass_pane",
            "minecraft:red_stained_glass_pane",
            "minecraft:black_stained_glass_pane",
            "minecraft:acacia_stairs",
            "minecraft:dark_oak_stairs",
            "minecraft:slime_block",
            "minecraft:barrier",
            "minecraft:iron_trapdoor",
            "minecraft:prismarine",
            "minecraft:prismarine_bricks",
            "minecraft:dark_prismarine",
            "minecraft:prismarine_stairs",
            "minecraft:prismarine_brick_stairs",
            "minecraft:dark_prismarine_stairs",
            "minecraft:prismarine_slab",
            "minecraft:prismarine_brick_slab",
            "minecraft:dark_prismarine_slab",
            "minecraft:sea_lantern",
            "minecraft:hay_block",
            "minecraft:white_carpet",
            "minecraft:orange_carpet",
            "minecraft:magenta_carpet",
            "minecraft:light_blue_carpet",
            "minecraft:yellow_carpet",
            "minecraft:lime_carpet",
            "minecraft:pink_carpet",
            "minecraft:gray_carpet",
            "minecraft:light_gray_carpet",
            "minecraft:cyan_carpet",
            "minecraft:purple_carpet",
            "minecraft:blue_carpet",
            "minecraft:brown_carpet",
            "minecraft:green_carpet",
            "minecraft:red_carpet",
            "minecraft:black_carpet",
            "minecraft:terracotta",
            "minecraft:coal_block",
            "minecraft:packed_ice",
            "minecraft:sunflower",
            "minecraft:lilac",
            "minecraft:rose_bush",
            "minecraft:peony",
            "minecraft:tall_grass",
            "minecraft:large_fern",
            "minecraft:white_banner",
            "minecraft:orange_banner",
            "minecraft:magenta_banner",
            "minecraft:light_blue_banner",
            "minecraft:yellow_banner",
            "minecraft:lime_banner",
            "minecraft:pink_banner",
            "minecraft:gray_banner",
            "minecraft:light_gray_banner",
            "minecraft:cyan_banner",
            "minecraft:purple_banner",
            "minecraft:blue_banner",
            "minecraft:brown_banner",
            "minecraft:green_banner",
            "minecraft:red_banner",
            "minecraft:black_banner",
            "minecraft:white_wall_banner",
            "minecraft:orange_wall_banner",
            "minecraft:magenta_wall_banner",
            "minecraft:light_blue_wall_banner",
            "minecraft:yellow_wall_banner",
            "minecraft:lime_wall_banner",
            "minecraft:pink_wall_banner",
            "minecraft:gray_wall_banner",
            "minecraft:light_gray_wall_banner",
            "minecraft:cyan_wall_banner",
            "minecraft:purple_wall_banner",
            "minecraft:blue_wall_banner",
            "minecraft:brown_wall_banner",
            "minecraft:green_wall_banner",
            "minecraft:red_wall_banner",
            "minecraft:black_wall_banner",
            "minecraft:red_sandstone",
            "minecraft:chiseled_red_sandstone",
            "minecraft:cut_red_sandstone",
            "minecraft:red_sandstone_stairs",
            "minecraft:oak_slab",
            "minecraft:spruce_slab",
            "minecraft:birch_slab",
            "minecraft:jungle_slab",
            "minecraft:acacia_slab",
            "minecraft:dark_oak_slab",
            "minecraft:stone_slab",
            "minecraft:sandstone_slab",
            "minecraft:petrified_oak_slab",
            "minecraft:cobblestone_slab",
            "minecraft:brick_slab",
            "minecraft:stone_brick_slab",
            "minecraft:nether_brick_slab",
            "minecraft:quartz_slab",
            "minecraft:red_sandstone_slab",
            "minecraft:purpur_slab",
            "minecraft:smooth_stone",
            "minecraft:smooth_sandstone",
            "minecraft:smooth_quartz",
            "minecraft:smooth_red_sandstone",
            "minecraft:spruce_fence_gate",
            "minecraft:birch_fence_gate",
            "minecraft:jungle_fence_gate",
            "minecraft:acacia_fence_gate",
            "minecraft:dark_oak_fence_gate",
            "minecraft:spruce_fence",
            "minecraft:birch_fence",
            "minecraft:jungle_fence",
            "minecraft:acacia_fence",
            "minecraft:dark_oak_fence",
            "minecraft:spruce_door",
            "minecraft:birch_door",
            "minecraft:jungle_door",
            "minecraft:acacia_door",
            "minecraft:dark_oak_door",
            "minecraft:end_rod",
            "minecraft:chorus_plant",
            "minecraft:chorus_flower",
            "minecraft:purpur_block",
            "minecraft:purpur_pillar",
            "minecraft:purpur_stairs",
            "minecraft:end_stone_bricks",
            "minecraft:beetroots",
            "minecraft:grass_path",
            "minecraft:end_gateway",
            "minecraft:repeating_command_block",
            "minecraft:chain_command_block",
            "minecraft:frosted_ice",
            "minecraft:magma_block",
            "minecraft:nether_wart_block",
            "minecraft:red_nether_bricks",
            "minecraft:bone_block",
            "minecraft:structure_void",
            "minecraft:observer",
            "minecraft:shulker_box",
            "minecraft:white_shulker_box",
            "minecraft:orange_shulker_box",
            "minecraft:magenta_shulker_box",
            "minecraft:light_blue_shulker_box",
            "minecraft:yellow_shulker_box",
            "minecraft:lime_shulker_box",
            "minecraft:pink_shulker_box",
            "minecraft:gray_shulker_box",
            "minecraft:light_gray_shulker_box",
            "minecraft:cyan_shulker_box",
            "minecraft:purple_shulker_box",
            "minecraft:blue_shulker_box",
            "minecraft:brown_shulker_box",
            "minecraft:green_shulker_box",
            "minecraft:red_shulker_box",
            "minecraft:black_shulker_box",
            "minecraft:white_glazed_terracotta",
            "minecraft:orange_glazed_terracotta",
            "minecraft:magenta_glazed_terracotta",
            "minecraft:light_blue_glazed_terracotta",
            "minecraft:yellow_glazed_terracotta",
            "minecraft:lime_glazed_terracotta",
            "minecraft:pink_glazed_terracotta",
            "minecraft:gray_glazed_terracotta",
            "minecraft:light_gray_glazed_terracotta",
            "minecraft:cyan_glazed_terracotta",
            "minecraft:purple_glazed_terracotta",
            "minecraft:blue_glazed_terracotta",
            "minecraft:brown_glazed_terracotta",
            "minecraft:green_glazed_terracotta",
            "minecraft:red_glazed_terracotta",
            "minecraft:black_glazed_terracotta",
            "minecraft:white_concrete",
            "minecraft:orange_concrete",
            "minecraft:magenta_concrete",
            "minecraft:light_blue_concrete",
            "minecraft:yellow_concrete",
            "minecraft:lime_concrete",
            "minecraft:pink_concrete",
            "minecraft:gray_concrete",
            "minecraft:light_gray_concrete",
            "minecraft:cyan_concrete",
            "minecraft:purple_concrete",
            "minecraft:blue_concrete",
            "minecraft:brown_concrete",
            "minecraft:green_concrete",
            "minecraft:red_concrete",
            "minecraft:black_concrete",
            "minecraft:white_concrete_powder",
            "minecraft:orange_concrete_powder",
            "minecraft:magenta_concrete_powder",
            "minecraft:light_blue_concrete_powder",
            "minecraft:yellow_concrete_powder",
            "minecraft:lime_concrete_powder",
            "minecraft:pink_concrete_powder",
            "minecraft:gray_concrete_powder",
            "minecraft:light_gray_concrete_powder",
            "minecraft:cyan_concrete_powder",
            "minecraft:purple_concrete_powder",
            "minecraft:blue_concrete_powder",
            "minecraft:brown_concrete_powder",
            "minecraft:green_concrete_powder",
            "minecraft:red_concrete_powder",
            "minecraft:black_concrete_powder",
            "minecraft:kelp",
            "minecraft:kelp_plant",
            "minecraft:dried_kelp_block",
            "minecraft:turtle_egg",
            "minecraft:dead_tube_coral_block",
            "minecraft:dead_brain_coral_block",
            "minecraft:dead_bubble_coral_block",
            "minecraft:dead_fire_coral_block",
            "minecraft:dead_horn_coral_block",
            "minecraft:tube_coral_block",
            "minecraft:brain_coral_block",
            "minecraft:bubble_coral_block",
            "minecraft:fire_coral_block",
            "minecraft:horn_coral_block",
            "minecraft:dead_tube_coral",
            "minecraft:dead_brain_coral",
            "minecraft:dead_bubble_coral",
            "minecraft:dead_fire_coral",
            "minecraft:dead_horn_coral",
            "minecraft:tube_coral",
            "minecraft:brain_coral",
            "minecraft:bubble_coral",
            "minecraft:fire_coral",
            "minecraft:horn_coral",
            "minecraft:dead_tube_coral_wall_fan",
            "minecraft:dead_brain_coral_wall_fan",
            "minecraft:dead_bubble_coral_wall_fan",
            "minecraft:dead_fire_coral_wall_fan",
            "minecraft:dead_horn_coral_wall_fan",
            "minecraft:tube_coral_wall_fan",
            "minecraft:brain_coral_wall_fan",
            "minecraft:bubble_coral_wall_fan",
            "minecraft:fire_coral_wall_fan",
            "minecraft:horn_coral_wall_fan",
            "minecraft:dead_tube_coral_fan",
            "minecraft:dead_brain_coral_fan",
            "minecraft:dead_bubble_coral_fan",
            "minecraft:dead_fire_coral_fan",
            "minecraft:dead_horn_coral_fan",
            "minecraft:tube_coral_fan",
            "minecraft:brain_coral_fan",
            "minecraft:bubble_coral_fan",
            "minecraft:fire_coral_fan",
            "minecraft:horn_coral_fan",
            "minecraft:sea_pickle",
            "minecraft:blue_ice",
            "minecraft:conduit",
            "minecraft:void_air",
            "minecraft:cave_air",
            "minecraft:bubble_column",
            "minecraft:structure_block",
        };

        unsigned int map[] = {
            0,   1,   2,   3,   4,   5,   6,   7,   8,   8,   9,   10,  11,
            11,  12,  13,  14,  15,  16,  17,  18,  19,  19,  20,  20,  21,
            21,  22,  22,  23,  23,  24,  24,  25,  26,  26,  26,  26,  26,
            26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  27,  27,
            27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,
            27,  28,  29,  30,  31,  32,  33,  34,  34,  34,  35,  35,  35,
            36,  36,  36,  37,  37,  37,  38,  38,  38,  39,  39,  39,  40,
            40,  40,  41,  41,  41,  42,  42,  42,  43,  43,  43,  44,  44,
            44,  45,  45,  45,  46,  46,  46,  47,  47,  47,  48,  48,  48,
            49,  49,  49,  50,  50,  50,  51,  51,  51,  52,  52,  52,  53,
            53,  53,  54,  54,  54,  55,  55,  55,  56,  56,  56,  57,  57,
            57,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,
            58,  58,  59,  59,  59,  59,  59,  59,  59,  59,  59,  59,  59,
            59,  59,  59,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,
            60,  60,  60,  60,  61,  61,  61,  61,  61,  61,  61,  61,  61,
            61,  61,  61,  61,  61,  62,  62,  62,  62,  62,  62,  62,  62,
            62,  62,  62,  62,  62,  62,  63,  63,  63,  63,  63,  63,  63,
            63,  63,  63,  63,  63,  63,  63,  64,  65,  66,  67,  68,  69,
            69,  69,  69,  69,  69,  69,  69,  69,  69,  69,  69,  70,  71,
            72,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,
            73,  73,  73,  73,  73,  73,  73,  74,  74,  74,  74,  74,  74,
            74,  74,  74,  74,  74,  74,  74,  74,  74,  74,  75,  75,  75,
            75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,
            76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,
            76,  76,  76,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,
            77,  77,  77,  77,  77,  77,  78,  78,  78,  78,  78,  78,  78,
            78,  78,  78,  78,  78,  78,  78,  78,  78,  79,  79,  79,  79,
            79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  79,  80,
            80,  80,  80,  80,  80,  80,  80,  80,  80,  80,  80,  80,  80,
            80,  80,  81,  81,  81,  81,  81,  81,  81,  81,  81,  81,  81,
            81,  81,  81,  81,  81,  82,  82,  82,  82,  82,  82,  82,  82,
            82,  82,  82,  82,  82,  82,  82,  82,  83,  83,  83,  83,  83,
            83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  83,  84,  84,
            84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,
            84,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,
            85,  85,  85,  85,  86,  86,  86,  86,  86,  86,  86,  86,  86,
            86,  86,  86,  86,  86,  86,  86,  87,  87,  87,  87,  87,  87,
            87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  88,  88,  88,
            88,  88,  88,  88,  88,  88,  88,  88,  88,  88,  88,  88,  88,
            89,  89,  89,  89,  89,  89,  89,  89,  89,  89,  89,  89,  89,
            89,  89,  89,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,
            90,  90,  91,  91,  91,  91,  91,  91,  91,  91,  91,  91,  91,
            91,  92,  92,  92,  92,  92,  92,  92,  92,  92,  92,  92,  92,
            93,  94,  95,  96,  97,  98,  98,  99,  99,  99,  99,  99,  99,
            99,  99,  99,  99,  99,  99,  100, 100, 100, 100, 100, 100, 100,
            100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
            100, 100, 100, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
            110, 111, 112, 113, 114, 115, 116, 117, 117, 117, 117, 117, 117,
            117, 117, 117, 117, 117, 117, 118, 119, 120, 121, 122, 123, 124,
            125, 126, 127, 128, 129, 130, 131, 132, 133, 133, 134, 135, 136,
            137, 138, 138, 138, 138, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
            139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 140, 141, 141,
            141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
            141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
            141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
            141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
            141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
            141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
            142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
            142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
            143, 143, 143, 143, 143, 143, 143, 144, 145, 146, 147, 147, 147,
            147, 147, 147, 147, 147, 148, 148, 148, 148, 148, 148, 148, 148,
            149, 149, 149, 149, 149, 149, 149, 149, 150, 150, 150, 150, 150,
            150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
            150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
            150, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,
            151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,
            151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,
            151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,
            151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151,
            152, 152, 152, 152, 152, 152, 152, 152, 153, 153, 153, 153, 153,
            153, 153, 153, 153, 153, 154, 154, 154, 154, 154, 154, 154, 154,
            154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
            154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
            154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
            154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
            154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
            154, 154, 154, 154, 154, 154, 154, 155, 155, 155, 155, 155, 155,
            155, 155, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
            156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
            157, 157, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
            158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
            158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
            158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
            158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
            158, 159, 159, 160, 160, 161, 161, 162, 162, 163, 163, 164, 164,
            165, 165, 166, 166, 167, 167, 167, 167, 167, 167, 167, 167, 168,
            168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168,
            168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 169, 169, 169,
            169, 169, 169, 169, 169, 170, 171, 172, 172, 172, 172, 172, 172,
            172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 173, 174, 174,
            174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174,
            174, 175, 175, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176,
            176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176,
            176, 176, 176, 176, 176, 176, 176, 176, 176, 177, 178, 179, 180,
            181, 181, 182, 182, 182, 182, 183, 183, 183, 183, 184, 184, 184,
            184, 184, 184, 184, 185, 185, 185, 185, 185, 185, 185, 185, 185,
            185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185,
            185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185,
            185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185,
            185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185,
            185, 185, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
            196, 197, 198, 199, 200, 201, 202, 202, 202, 202, 202, 202, 202,
            202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202,
            202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202,
            202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202,
            202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202,
            202, 202, 202, 202, 202, 203, 203, 203, 203, 203, 203, 203, 203,
            203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203,
            203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203,
            203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203,
            203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203,
            203, 203, 203, 203, 204, 204, 204, 204, 204, 204, 204, 204, 204,
            204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
            204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
            204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
            204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
            204, 204, 204, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
            205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
            205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
            205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
            205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
            205, 205, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206,
            206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206,
            206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206,
            206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206,
            206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206,
            206, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
            207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
            207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
            207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
            207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
            208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 218, 218,
            218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218,
            218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218,
            218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218,
            218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218,
            218, 218, 218, 218, 218, 218, 218, 218, 218, 219, 219, 219, 219,
            219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
            219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
            219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
            219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
            219, 219, 219, 219, 219, 219, 219, 219, 220, 220, 220, 220, 220,
            220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220,
            220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220,
            220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220,
            220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220,
            220, 220, 220, 220, 220, 220, 220, 221, 221, 221, 221, 221, 221,
            221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221,
            221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221,
            222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222,
            222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222,
            222, 222, 222, 222, 222, 222, 223, 224, 224, 224, 224, 225, 225,
            225, 225, 226, 226, 226, 226, 226, 226, 226, 226, 227, 227, 227,
            227, 227, 227, 227, 227, 228, 228, 228, 228, 228, 228, 228, 228,
            228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
            228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 229, 229,
            229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
            229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
            229, 229, 229, 229, 230, 230, 230, 230, 230, 230, 230, 230, 230,
            230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
            230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
            230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
            230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
            230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
            230, 230, 230, 230, 230, 230, 231, 231, 231, 231, 231, 231, 231,
            231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231,
            231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231,
            231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231,
            231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231,
            231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231,
            231, 231, 231, 231, 231, 231, 231, 231, 232, 232, 233, 234, 235,
            235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235,
            235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235,
            235, 235, 235, 235, 235, 236, 236, 236, 236, 236, 236, 236, 236,
            236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
            236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
            236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
            236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
            236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
            236, 236, 236, 236, 236, 236, 236, 237, 237, 237, 237, 238, 239,
            239, 239, 239, 239, 239, 239, 239, 240, 240, 240, 240, 241, 242,
            242, 242, 242, 242, 242, 242, 242, 243, 244, 245, 245, 246, 246,
            246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 247, 247, 247,
            247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247,
            247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247,
            247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247,
            247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247,
            247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247,
            247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 248,
            249, 249, 249, 249, 249, 249, 249, 249, 250, 250, 250, 250, 250,
            250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
            251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 253, 253, 253,
            253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
            253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
            253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
            253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
            253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
            253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254,
            254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
            254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
            254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
            254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
            254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
            254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
            254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
            256, 256, 257, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
            258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
            258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
            258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
            258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
            258, 258, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259,
            259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259,
            259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259,
            259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259,
            259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259, 259,
            259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271,
            272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 282, 282,
            282, 282, 282, 282, 282, 283, 283, 283, 283, 283, 283, 283, 283,
            284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284,
            284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 285, 285,
            285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285,
            285, 285, 285, 285, 285, 285, 285, 285, 285, 286, 286, 286, 286,
            286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286,
            286, 286, 286, 286, 286, 286, 286, 287, 287, 287, 287, 287, 287,
            287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287,
            287, 287, 287, 287, 287, 288, 288, 288, 288, 288, 288, 288, 288,
            288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288,
            288, 288, 288, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289,
            289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289,
            289, 290, 290, 290, 290, 291, 291, 291, 291, 291, 291, 291, 291,
            291, 291, 291, 291, 291, 291, 291, 291, 292, 292, 292, 292, 293,
            293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293,
            293, 293, 294, 294, 294, 294, 295, 295, 295, 295, 295, 295, 295,
            295, 295, 295, 295, 295, 295, 295, 295, 295, 296, 296, 296, 296,
            297, 297, 297, 297, 297, 297, 297, 297, 297, 297, 297, 297, 297,
            297, 297, 297, 298, 298, 298, 298, 299, 299, 299, 299, 299, 299,
            299, 299, 299, 299, 299, 299, 299, 299, 299, 299, 300, 300, 300,
            300, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301,
            301, 301, 301, 301, 302, 302, 302, 302, 303, 303, 303, 303, 304,
            304, 304, 304, 305, 305, 305, 305, 305, 305, 305, 305, 305, 305,
            305, 305, 305, 305, 305, 305, 305, 305, 305, 305, 305, 305, 305,
            305, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306,
            306, 306, 306, 306, 307, 307, 307, 307, 307, 307, 307, 307, 307,
            307, 307, 307, 307, 307, 307, 307, 308, 308, 308, 308, 308, 308,
            308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 309, 309, 309,
            309, 309, 309, 309, 309, 309, 309, 309, 309, 309, 309, 309, 309,
            309, 309, 309, 309, 309, 309, 309, 309, 309, 309, 309, 309, 309,
            309, 309, 309, 310, 311, 312, 312, 312, 312, 312, 312, 312, 312,
            312, 312, 313, 314, 315, 315, 315, 316, 316, 316, 316, 316, 316,
            316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316,
            316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316,
            316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316,
            316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316,
            316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316, 316,
            316, 316, 316, 316, 316, 316, 316, 316, 316, 317, 317, 317, 317,
            317, 317, 317, 317, 317, 317, 317, 317, 318, 318, 318, 318, 318,
            318, 318, 318, 318, 318, 318, 318, 319, 320, 321, 322, 323, 324,
            325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 335, 335,
            335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335,
            335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335,
            335, 335, 335, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
            336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
            336, 336, 336, 336, 336, 336, 336, 336, 336, 337, 337, 337, 337,
            337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337,
            337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337,
            337, 337, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
            338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
            338, 338, 338, 338, 338, 338, 338, 338, 339, 339, 339, 339, 339,
            339, 339, 339, 339, 339, 339, 339, 339, 339, 339, 339, 339, 339,
            339, 339, 339, 339, 339, 339, 339, 339, 339, 339, 339, 339, 339,
            339, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340,
            340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340,
            340, 340, 340, 340, 340, 340, 340, 341, 341, 341, 341, 341, 341,
            341, 341, 341, 341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
            341, 341, 341, 341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
            342, 342, 342, 342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
            342, 342, 342, 342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
            342, 342, 342, 342, 342, 342, 343, 343, 343, 343, 343, 343, 343,
            343, 343, 343, 343, 343, 343, 343, 343, 343, 343, 343, 343, 343,
            343, 343, 343, 343, 343, 343, 343, 343, 343, 343, 343, 343, 344,
            344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344,
            344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344, 344,
            344, 344, 344, 344, 344, 345, 345, 345, 345, 345, 345, 345, 345,
            345, 345, 345, 345, 345, 345, 345, 345, 345, 345, 345, 345, 345,
            345, 345, 345, 345, 345, 345, 345, 345, 345, 345, 345, 346, 346,
            346, 346, 346, 346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
            346, 346, 346, 346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
            346, 346, 346, 346, 347, 347, 347, 347, 347, 347, 347, 347, 347,
            347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347,
            347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 348, 348, 348,
            348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348,
            348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348,
            348, 348, 348, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349,
            349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349,
            349, 349, 349, 349, 349, 349, 349, 349, 349, 350, 350, 350, 350,
            350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350,
            350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350,
            350, 350, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351,
            351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351,
            351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351,
            351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351,
            351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351,
            351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351,
            351, 351, 351, 351, 352, 352, 352, 352, 352, 352, 352, 352, 352,
            352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
            352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
            352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
            352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
            352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352,
            352, 352, 352, 352, 352, 352, 353, 354, 355, 355, 355, 355, 355,
            355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355,
            355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355,
            355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355,
            355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355,
            355, 355, 355, 355, 355, 355, 355, 356, 357, 358, 359, 359, 359,
            359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359,
            359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359,
            359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359,
            359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359,
            359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359,
            359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 360,
            360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360,
            360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360,
            360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360,
            360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360,
            360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360,
            360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360,
            360, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361,
            361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361,
            361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361,
            361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361,
            361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361,
            361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361,
            361, 361, 361, 362, 362, 362, 362, 362, 362, 363, 363, 363, 363,
            363, 363, 364, 364, 364, 364, 364, 364, 365, 366, 366, 366, 367,
            368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380,
            381, 382, 383, 384, 385, 386, 386, 387, 387, 388, 388, 389, 389,
            390, 390, 391, 391, 392, 392, 392, 392, 392, 392, 392, 392, 392,
            392, 392, 392, 392, 392, 392, 392, 393, 393, 393, 393, 393, 393,
            393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 394, 394, 394,
            394, 394, 394, 394, 394, 394, 394, 394, 394, 394, 394, 394, 394,
            395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395,
            395, 395, 395, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396,
            396, 396, 396, 396, 396, 396, 397, 397, 397, 397, 397, 397, 397,
            397, 397, 397, 397, 397, 397, 397, 397, 397, 398, 398, 398, 398,
            398, 398, 398, 398, 398, 398, 398, 398, 398, 398, 398, 398, 399,
            399, 399, 399, 399, 399, 399, 399, 399, 399, 399, 399, 399, 399,
            399, 399, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
            400, 400, 400, 400, 400, 401, 401, 401, 401, 401, 401, 401, 401,
            401, 401, 401, 401, 401, 401, 401, 401, 402, 402, 402, 402, 402,
            402, 402, 402, 402, 402, 402, 402, 402, 402, 402, 402, 403, 403,
            403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403,
            403, 404, 404, 404, 404, 404, 404, 404, 404, 404, 404, 404, 404,
            404, 404, 404, 404, 405, 405, 405, 405, 405, 405, 405, 405, 405,
            405, 405, 405, 405, 405, 405, 405, 406, 406, 406, 406, 406, 406,
            406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 407, 407, 407,
            407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407,
            408, 408, 408, 408, 409, 409, 409, 409, 410, 410, 410, 410, 411,
            411, 411, 411, 412, 412, 412, 412, 413, 413, 413, 413, 414, 414,
            414, 414, 415, 415, 415, 415, 416, 416, 416, 416, 417, 417, 417,
            417, 418, 418, 418, 418, 419, 419, 419, 419, 420, 420, 420, 420,
            421, 421, 421, 421, 422, 422, 422, 422, 423, 423, 423, 423, 424,
            425, 426, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427,
            427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427,
            427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427,
            427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427,
            427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427,
            427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427, 427,
            427, 427, 427, 427, 428, 428, 428, 428, 428, 428, 429, 429, 429,
            429, 429, 429, 430, 430, 430, 430, 430, 430, 431, 431, 431, 431,
            431, 431, 432, 432, 432, 432, 432, 432, 433, 433, 433, 433, 433,
            433, 434, 434, 434, 434, 434, 434, 435, 435, 435, 435, 435, 435,
            436, 436, 436, 436, 436, 436, 437, 437, 437, 437, 437, 437, 438,
            438, 438, 438, 438, 438, 439, 439, 439, 439, 439, 439, 440, 440,
            440, 440, 440, 440, 441, 441, 441, 441, 441, 441, 442, 442, 442,
            442, 442, 442, 443, 443, 443, 443, 443, 443, 444, 445, 446, 447,
            448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448,
            448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448,
            448, 448, 448, 448, 448, 448, 449, 449, 449, 449, 449, 449, 449,
            449, 449, 449, 449, 449, 449, 449, 449, 449, 449, 449, 449, 449,
            449, 449, 449, 449, 449, 449, 449, 449, 449, 449, 449, 449, 450,
            450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450,
            450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450,
            450, 450, 450, 450, 450, 451, 451, 451, 451, 451, 451, 451, 451,
            451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451,
            451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 452, 452,
            452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452,
            452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452,
            452, 452, 452, 452, 453, 453, 453, 453, 453, 453, 453, 453, 453,
            453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453,
            453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 454, 454, 454,
            454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454,
            454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454,
            454, 454, 454, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455,
            455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455,
            455, 455, 455, 455, 455, 455, 455, 455, 455, 456, 456, 456, 456,
            456, 456, 456, 456, 456, 456, 456, 456, 456, 456, 456, 456, 456,
            456, 456, 456, 456, 456, 456, 456, 456, 456, 456, 456, 456, 456,
            456, 456, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457,
            457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457,
            457, 457, 457, 457, 457, 457, 457, 457, 458, 458, 458, 458, 458,
            458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458,
            458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458,
            458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458,
            458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458, 458,
            458, 458, 458, 458, 458, 458, 458, 459, 459, 459, 459, 459, 459,
            459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459,
            459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459,
            459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459,
            459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459, 459,
            459, 459, 459, 459, 459, 459, 460, 460, 460, 460, 460, 460, 460,
            460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460,
            460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460,
            460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460,
            460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460,
            460, 460, 460, 460, 460, 461, 461, 461, 461, 461, 461, 461, 461,
            461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
            461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
            461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
            461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
            461, 461, 461, 461, 462, 462, 462, 462, 462, 462, 462, 462, 462,
            462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462,
            462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462,
            462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462,
            462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462,
            462, 462, 462, 463, 463, 463, 463, 463, 463, 464, 464, 464, 464,
            464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464,
            464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464,
            464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464,
            464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464,
            464, 464, 464, 464, 464, 464, 464, 464, 465, 465, 465, 465, 465,
            465, 466, 467, 467, 467, 468, 468, 468, 468, 468, 468, 468, 468,
            468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
            468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
            468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
            468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
            468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
            468, 468, 468, 468, 468, 468, 468, 469, 470, 470, 470, 470, 471,
            472, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473,
            474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 475,
            475, 475, 475, 476, 477, 478, 479, 479, 479, 480, 481, 481, 481,
            481, 481, 481, 481, 481, 481, 481, 481, 481, 482, 482, 482, 482,
            482, 482, 483, 483, 483, 483, 483, 483, 484, 484, 484, 484, 484,
            484, 485, 485, 485, 485, 485, 485, 486, 486, 486, 486, 486, 486,
            487, 487, 487, 487, 487, 487, 488, 488, 488, 488, 488, 488, 489,
            489, 489, 489, 489, 489, 490, 490, 490, 490, 490, 490, 491, 491,
            491, 491, 491, 491, 492, 492, 492, 492, 492, 492, 493, 493, 493,
            493, 493, 493, 494, 494, 494, 494, 494, 494, 495, 495, 495, 495,
            495, 495, 496, 496, 496, 496, 496, 496, 497, 497, 497, 497, 497,
            497, 498, 498, 498, 498, 498, 498, 499, 499, 499, 499, 500, 500,
            500, 500, 501, 501, 501, 501, 502, 502, 502, 502, 503, 503, 503,
            503, 504, 504, 504, 504, 505, 505, 505, 505, 506, 506, 506, 506,
            507, 507, 507, 507, 508, 508, 508, 508, 509, 509, 509, 509, 510,
            510, 510, 510, 511, 511, 511, 511, 512, 512, 512, 512, 513, 513,
            513, 513, 514, 514, 514, 514, 515, 516, 517, 518, 519, 520, 521,
            522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534,
            535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547,
            547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547,
            547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 548,
            549, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550,
            551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 561, 562,
            562, 563, 563, 564, 564, 565, 565, 566, 566, 567, 567, 568, 568,
            569, 569, 570, 570, 571, 571, 571, 571, 571, 571, 571, 571, 572,
            572, 572, 572, 572, 572, 572, 572, 573, 573, 573, 573, 573, 573,
            573, 573, 574, 574, 574, 574, 574, 574, 574, 574, 575, 575, 575,
            575, 575, 575, 575, 575, 576, 576, 576, 576, 576, 576, 576, 576,
            577, 577, 577, 577, 577, 577, 577, 577, 578, 578, 578, 578, 578,
            578, 578, 578, 579, 579, 579, 579, 579, 579, 579, 579, 580, 580,
            580, 580, 580, 580, 580, 580, 581, 581, 582, 582, 583, 583, 584,
            584, 585, 585, 586, 586, 587, 587, 588, 588, 589, 589, 590, 590,
            591, 591, 591, 591, 591, 591, 591, 591, 592, 593, 593, 594, 595,
            596, 596, 597, 597, 597, 597,
        };

        unsigned int transparent_ids[] = {
            0,    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
            31,   32,   34,   35,   36,   37,   38,   39,   40,   41,   42,
            43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
            54,   55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
            65,   1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013,
            1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024,
            1025, 1026, 1027, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1111,
            1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119, 1120, 1121, 1122,
            1131, 1132, 1133, 1134, 1135, 1136, 1137, 1138, 1139, 1140, 1141,
            1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1150, 1151, 1152,
            1153, 1154, 1155, 1156, 1157, 1158, 1159, 1160, 1161, 1162, 1163,
            1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174,
            1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185,
            1186, 1187, 1188, 1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196,
            1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207,
            1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218,
            1219, 1220, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229,
            1230, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240,
            1241, 1242, 1243, 1244, 1245, 1246, 1247, 1248, 1249, 1250, 1251,
            1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260, 1261, 1262,
            1263, 1264, 1265, 1266, 1267, 1268, 1269, 1270, 1271, 1272, 1273,
            1274, 1275, 1276, 1277, 1278, 1279, 1280, 1281, 1282, 1283, 1284,
            1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295,
            1296, 1297, 1298, 1299, 1300, 1301, 1302, 1303, 1304, 1305, 1306,
            1307, 1308, 1309, 1310, 1311, 1312, 1313, 1314, 1315, 1316, 1317,
            1318, 1319, 1320, 1321, 1322, 1323, 1324, 1325, 1326, 1327, 1328,
            1329, 1330, 1331, 1332, 1333, 1334, 1335, 1336, 1337, 1338, 1339,
            1340, 1341, 1342, 1343, 1344, 1345, 1346, 1347, 1348, 1349, 1350,
            1351, 1352, 1353, 1354, 1355, 1356, 1357, 1358, 1359, 1360, 1361,
            1362, 1363, 1364, 1365, 1366, 1367, 1368, 1369, 1370, 1371, 1372,
            1373, 1374, 1375, 1376, 1377, 1378, 1379, 1380, 1381, 1382, 1383,
            1384, 1385, 1386, 1387, 1388, 1389, 1390, 1391, 1392, 1393, 1394,
            1395, 1396, 1397, 1398, 1399, 1400, 1401, 1402, 1403, 1404, 1405,
            1406, 1407, 1408, 1409, 1410, 1411, 1412, 1413, 1414, 1415, 1416,
            1417, 1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427,
            1428, 1429, 1430, 1431, 1432, 1433, 1434, 1435, 1436, 1437, 1438,
            1439, 1440, 1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449,
            1450, 1451, 1452, 1453, 1454, 1455, 1456, 1457, 1458, 1459, 1460,
            1461, 1462, 1463, 1464, 1465, 1466, 1467, 1468, 1469, 1470, 1471,
            1472, 1473, 1474, 1475, 1476, 1477, 1478, 1479, 1480, 1481, 1482,
            1483, 1484, 1485, 1486, 1487, 1488, 1489, 1490, 1491, 1492, 1493,
            1494, 1495, 1496, 1497, 1498, 1499, 1500, 1501, 1502, 1503, 1504,
            1505, 1506, 1507, 1508, 1509, 1510, 1511, 1512, 1513, 1514, 1515,
            1516, 1517, 1518, 1519, 1520, 1521, 1522, 1523, 1524, 1525, 1526,
            1527, 1528, 1529, 1530, 1531, 1532, 1533, 1534, 1535, 1536, 1537,
            1538, 1539, 1540, 1541, 1542, 1543, 1544, 1545, 1546, 1547, 1548,
            1549, 1550, 1551, 1552, 1553, 1554, 1555, 1556, 1557, 1558, 1559,
            1560, 1561, 1562, 1563, 1564, 1565, 1566, 1567, 1568, 1569, 1570,
            1571, 1572, 1573, 1574, 1575, 1576, 1577, 1578, 1579, 1580, 1581,
            1582, 1583, 1584, 1585, 1586, 1587, 1588, 1589, 1590, 1591, 1592,
            1593, 1594, 1595, 1596, 1597, 1598, 1599, 1600, 1601, 1602, 1603,
            1604, 1605, 1606, 1607, 1608, 1609, 1610, 1611, 1612, 1613, 1614,
            1615, 1616, 1617, 1618, 1619, 1620, 1621, 1622, 1623, 1624, 1625,
            1626, 1627, 1628, 1629, 1630, 1631, 1632, 1633, 1634, 1635, 1636,
            1637, 1638, 1639, 1640, 1641, 1642, 1643, 1644, 1645, 1646, 1647,
            1753, 1754, 1755, 1756, 1757, 1758, 1759, 1760, 1761, 1762, 1763,
            1764, 1765, 1766, 1767, 1768, 1769, 1770, 1771, 1772, 1773, 1774,
            1775, 1776, 1777, 1778, 1779, 1780, 1781, 1782, 1783, 1784, 1785,
            1786, 1787, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796,
            1797, 1798, 1799, 1800, 1801, 1802, 1803, 1804, 1805, 1806, 1807,
            1808, 1809, 1810, 1811, 1812, 1813, 1814, 1815, 1816, 1817, 1818,
            1819, 1820, 1821, 1822, 1823, 1824, 1825, 1826, 1827, 1828, 1829,
            1830, 1831, 1832, 1833, 1834, 1835, 1836, 1837, 1838, 1839, 1840,
            1841, 1842, 1843, 1844, 1845, 1846, 1847, 1848, 1849, 1850, 1851,
            1852, 1853, 1854, 1855, 1856, 1857, 1858, 1859, 1860, 1861, 1862,
            1863, 1864, 1865, 1866, 1867, 1868, 1869, 1870, 1871, 1872, 1873,
            1874, 1875, 1876, 1877, 1878, 1879, 1880, 1881, 1882, 1883, 1884,
            1885, 1886, 1887, 1888, 1889, 1890, 1891, 1892, 1893, 1894, 1895,
            1896, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1904, 1905, 1906,
            1907, 1908, 1909, 1910, 1911, 1912, 1913, 1914, 1915, 1916, 1917,
            1918, 1919, 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927, 1928,
            1929, 1930, 1931, 1932, 1933, 1934, 1935, 1936, 1937, 1938, 1939,
            1940, 1941, 1942, 1943, 1944, 1945, 1946, 1947, 1948, 1949, 1950,
            1951, 1952, 1953, 1954, 1955, 1956, 1957, 1958, 1959, 1960, 1961,
            1962, 1963, 1964, 1965, 1966, 1967, 1968, 1969, 1970, 1971, 1972,
            1973, 1974, 1975, 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983,
            1984, 1985, 1986, 1987, 1988, 1989, 1990, 1991, 1992, 1993, 1994,
            1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005,
            2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
            2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026, 2027,
            2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037, 2038,
            2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047, 2048, 2049,
            2050, 2051, 2052, 2053, 2054, 2055, 2056, 2057, 2058, 2059, 2060,
            2061, 2062, 2063, 2064, 2065, 2066, 2067, 2068, 2069, 2070, 2071,
            2072, 2073, 2074, 2075, 2076, 2077, 2078, 2079, 2080, 2081, 2082,
            2083, 2084, 2085, 2086, 2087, 2088, 2089, 2090, 2091, 2092, 2093,
            2094, 2095, 2096, 2097, 2098, 2099, 2100, 2101, 2102, 2103, 2104,
            2105, 2106, 2107, 2108, 2109, 2110, 2111, 2112, 2113, 2114, 2115,
            2116, 2117, 2118, 2119, 2120, 2121, 2122, 2123, 2124, 2125, 2126,
            2127, 2128, 2129, 2130, 2131, 2132, 2133, 2134, 2135, 2136, 2137,
            2138, 2139, 2140, 2141, 2142, 2143, 2144, 2145, 2146, 2147, 2148,
            2149, 2150, 2151, 2152, 2153, 2154, 2155, 2156, 2157, 2158, 2159,
            2160, 2161, 2162, 2163, 2164, 2165, 2166, 2167, 2168, 2169, 2170,
            2171, 2172, 2173, 2174, 2175, 2176, 2177, 2178, 2179, 2180, 2181,
            2182, 2183, 2184, 2185, 2186, 2187, 2188, 2189, 2190, 2191, 2192,
            2193, 2194, 2195, 2196, 2197, 2198, 2199, 2200, 2201, 2202, 2203,
            2204, 2205, 2206, 2207, 2208, 2209, 2210, 2211, 2212, 2213, 2214,
            2215, 2216, 2217, 2218, 2219, 2220, 2221, 2222, 2223, 2224, 2225,
            2226, 2227, 2228, 2229, 2230, 2231, 2232, 2233, 2234, 2235, 2236,
            2237, 2238, 2239, 2240, 2241, 2242, 2243, 2244, 2245, 2246, 2247,
            2248, 2249, 2250, 2251, 2252, 2253, 2254, 2255, 2256, 2257, 2258,
            2259, 2260, 2261, 2262, 2263, 2264, 2265, 2266, 2267, 2268, 2269,
            2270, 2271, 2272, 2273, 2274, 2275, 2276, 2277, 2278, 2279, 2280,
            2281, 2282, 2283, 2284, 2285, 2286, 2287, 2288, 2289, 2290, 2291,
            2292, 2293, 2294, 2295, 2296, 2297, 2298, 2299, 2300, 2301, 2302,
            2303, 2304, 2305, 2306, 2307, 2308, 2309, 2310, 2311, 2312, 2313,
            2314, 2315, 2316, 2317, 2318, 2319, 2320, 2321, 2322, 2323, 2324,
            2325, 2326, 2327, 2328, 2329, 2330, 2331, 2332, 2333, 2334, 2335,
            2336, 2337, 2338, 2339, 2340, 2341, 2342, 2343, 2344, 2345, 2346,
            2347, 2348, 2349, 2350, 2351, 2352, 2353, 2354, 2355, 2356, 2357,
            2358, 2359, 2360, 2361, 2362, 2363, 2364, 2365, 2366, 2367, 2368,
            2369, 2370, 2371, 2372, 2373, 2374, 2375, 2376, 2377, 2378, 2379,
            2380, 2381, 2382, 2383, 2384, 2385, 2386, 2387, 2388, 2389, 2390,
            2391, 2392, 2393, 2394, 2395, 2396, 2397, 2398, 2399, 2400, 2401,
            2402, 2403, 2404, 2405, 2406, 2407, 2408, 2409, 2410, 2411, 2412,
            2413, 2414, 2415, 2416, 2417, 2418, 2419, 2420, 2421, 2422, 2423,
            2424, 2425, 2426, 2427, 2428, 2429, 2430, 2431, 2432, 2433, 2434,
            2435, 2436, 2437, 2438, 2439, 2440, 2441, 2442, 2443, 2444, 2445,
            2446, 2447, 2448, 2449, 2450, 2451, 2452, 2453, 2454, 2455, 2456,
            2457, 2458, 2459, 2460, 2461, 2462, 2463, 2464, 2465, 2466, 2467,
            2468, 2469, 2470, 2471, 2472, 2473, 2474, 2475, 2476, 2477, 2478,
            2479, 2480, 2481, 2482, 2483, 2484, 2485, 2486, 2487, 2488, 2489,
            2490, 2491, 2492, 2493, 2494, 2495, 2496, 2497, 2498, 2499, 2500,
            2501, 2502, 2503, 2504, 2505, 2506, 2507, 2508, 2509, 2510, 2511,
            2512, 2513, 2514, 2515, 2516, 2517, 2518, 2519, 2520, 2521, 2522,
            2523, 2524, 2525, 2526, 2527, 2528, 2529, 2530, 2531, 2532, 2533,
            2534, 2535, 2536, 2537, 2538, 2539, 2540, 2541, 2542, 2543, 2544,
            2545, 2546, 2547, 2548, 2549, 2550, 2551, 2552, 2553, 2554, 2555,
            2556, 2557, 2558, 2559, 2560, 2561, 2562, 2563, 2564, 2565, 2566,
            2567, 2568, 2569, 2570, 2571, 2572, 2573, 2574, 2575, 2576, 2577,
            2578, 2579, 2580, 2581, 2582, 2583, 2584, 2585, 2586, 2587, 2588,
            2589, 2590, 2591, 2592, 2593, 2594, 2595, 2596, 2597, 2598, 2599,
            2600, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609, 2610,
            2611, 2612, 2613, 2614, 2615, 2616, 2617, 2618, 2619, 2620, 2621,
            2622, 2623, 2624, 2625, 2626, 2627, 2628, 2629, 2630, 2631, 2632,
            2633, 2634, 2635, 2636, 2637, 2638, 2639, 2640, 2641, 2642, 2643,
            2644, 2645, 2646, 2647, 2648, 2649, 2650, 2651, 2652, 2653, 2654,
            2655, 2656, 2657, 2658, 2659, 2660, 2661, 2662, 2663, 2664, 2665,
            2666, 2667, 2668, 2669, 2670, 2671, 2672, 2673, 2674, 2675, 2676,
            2677, 2678, 2679, 2680, 2681, 2682, 2683, 2684, 2685, 2686, 2687,
            2688, 2689, 2690, 2691, 2692, 2693, 2694, 2695, 2696, 2697, 2698,
            2699, 2700, 2701, 2702, 2703, 2704, 2705, 2706, 2707, 2708, 2709,
            2710, 2711, 2712, 2713, 2714, 2715, 2716, 2717, 2718, 2719, 2720,
            2721, 2722, 2723, 2724, 2725, 2726, 2727, 2728, 2729, 2730, 2731,
            2732, 2733, 2734, 2735, 2736, 2737, 2738, 2739, 2740, 2741, 2742,
            2743, 2744, 2745, 2746, 2747, 2748, 2749, 2750, 2751, 2752, 2753,
            2754, 2755, 2756, 2757, 2758, 2759, 2760, 2761, 2762, 2763, 2764,
            2765, 2766, 2767, 2768, 2769, 2770, 2771, 2772, 2773, 2774, 2775,
            2776, 2777, 2778, 2779, 2780, 2781, 2782, 2783, 2784, 2785, 2786,
            2787, 2788, 2789, 2790, 2791, 2792, 2793, 2794, 2795, 2796, 2797,
            2798, 2799, 2800, 2801, 2802, 2803, 2804, 2805, 2806, 2807, 2808,
            2809, 2810, 2811, 2812, 2813, 2814, 2815, 2816, 2817, 2818, 2819,
            2820, 2821, 2822, 2823, 2824, 2825, 2826, 2827, 2828, 2829, 2830,
            2831, 2832, 2833, 2834, 2835, 2836, 2837, 2838, 2839, 2840, 2841,
            2842, 2843, 2844, 2845, 2846, 2847, 2848, 2849, 2850, 2851, 2852,
            2853, 2854, 2855, 2856, 2857, 2858, 2859, 2860, 2861, 2862, 2863,
            2864, 2865, 2866, 2867, 2868, 2869, 2870, 2871, 2872, 2873, 2874,
            2875, 2876, 2877, 2878, 2879, 2880, 2881, 2882, 2883, 2884, 2885,
            2886, 2887, 2888, 2889, 2890, 2891, 2892, 2893, 2894, 2895, 2896,
            2897, 2898, 2899, 2900, 2901, 2902, 2903, 2904, 2905, 2906, 2907,
            2908, 2909, 2910, 2911, 2912, 2913, 2914, 2915, 2916, 2917, 2918,
            2919, 2920, 2921, 2922, 2923, 2924, 2925, 2926, 2927, 2928, 2929,
            2930, 2931, 2932, 2933, 2934, 2935, 2936, 2937, 2938, 2939, 2940,
            2941, 2942, 2943, 2944, 2945, 2946, 2947, 2948, 2949, 2950, 2951,
            2952, 2953, 2954, 2955, 2956, 2957, 2958, 2959, 2960, 2961, 2962,
            2963, 2964, 2965, 2966, 2967, 2968, 2969, 2970, 2971, 2972, 2973,
            2974, 2975, 2976, 2977, 2978, 2979, 2980, 2981, 2982, 2983, 2984,
            2985, 2986, 2987, 2988, 2989, 2990, 2991, 2992, 2993, 2994, 2995,
            2996, 2997, 2998, 2999, 3000, 3001, 3002, 3003, 3004, 3005, 3006,
            3007, 3008, 3009, 3010, 3011, 3012, 3013, 3014, 3015, 3016, 3017,
            3018, 3019, 3020, 3021, 3022, 3023, 3024, 3025, 3026, 3027, 3028,
            3029, 3030, 3031, 3032, 3033, 3034, 3035, 3036, 3037, 3038, 3039,
            3040, 3041, 3042, 3043, 3044, 3045, 3046, 3047, 3048, 3052, 3053,
            3054, 3055, 3056, 3057, 3058, 3059, 3076, 3077, 3078, 3079, 3080,
            3081, 3082, 3083, 3084, 3085, 3086, 3087, 3088, 3089, 3090, 3091,
            3092, 3093, 3094, 3095, 3096, 3097, 3098, 3099, 3100, 3101, 3102,
            3103, 3104, 3105, 3106, 3107, 3180, 3181, 3182, 3183, 3184, 3185,
            3186, 3187, 3188, 3189, 3270, 3271, 3272, 3273, 3274, 3275, 3276,
            3277, 3278, 3279, 3280, 3281, 3282, 3283, 3284, 3285, 3286, 3287,
            3288, 3289, 3290, 3291, 3292, 3293, 3294, 3295, 3296, 3297, 3298,
            3299, 3300, 3301, 3382, 3383, 3384, 3385, 3386, 3387, 3388, 3389,
            3390, 3391, 3392, 3393, 3394, 3395, 3396, 3397, 3398, 3399, 3400,
            3401, 3402, 3403, 3404, 3405, 3406, 3407, 3408, 3409, 3410, 3411,
            3412, 3413, 3414, 3415, 3416, 3417, 3418, 3419, 3420, 3421, 3422,
            3423, 3443, 3444, 3445, 3446, 3447, 3448, 3449, 3450, 3451, 3452,
            3453, 3454, 3455, 3456, 3457, 3458, 3497, 3498, 3514, 3515, 3516,
            3517, 3518, 3519, 3520, 3521, 3522, 3523, 3524, 3525, 3526, 3527,
            3528, 3529, 3530, 3531, 3532, 3533, 3534, 3535, 3536, 3537, 3538,
            3539, 3540, 3541, 3542, 3543, 3544, 3545, 3546, 3547, 3548, 3549,
            3550, 3551, 3552, 3553, 3554, 3555, 3556, 3557, 3558, 3559, 3560,
            3561, 3562, 3563, 3564, 3565, 3566, 3567, 3568, 3569, 3570, 3571,
            3572, 3573, 3574, 3575, 3576, 3577, 4116, 4117, 4118, 4119, 4120,
            4121, 4122, 4123, 4124, 4125, 4126, 4127, 4128, 4129, 4130, 4131,
            4132, 4133, 4134, 4135, 4136, 4137, 4138, 4139, 4140, 4141, 4142,
            4143, 4144, 4145, 4146, 4147, 4148, 4149, 4150, 4151, 4152, 4153,
            4154, 4155, 4156, 4157, 4158, 4159, 4160, 4161, 4162, 4163, 4164,
            4165, 4166, 4167, 4168, 4169, 4170, 4171, 4172, 4173, 4174, 4175,
            4176, 4177, 4178, 4179, 4245, 4246, 4247, 4248, 4249, 4250, 4251,
            4252, 4253, 4254, 4255, 4256, 4257, 4258, 4259, 4260, 4261, 4262,
            4263, 4264, 4265, 4266, 4267, 4268, 4269, 4270, 4271, 4272, 4273,
            4274, 4275, 4276, 4277, 4278, 4279, 4280, 4281, 4282, 4283, 4284,
            4285, 4286, 4287, 4288, 4289, 4290, 4291, 4292, 4293, 4294, 4295,
            4296, 4297, 4298, 4299, 4300, 4609, 4610, 4611, 4612, 4626, 4756,
            4757, 4758, 4759, 4760, 4761, 4762, 4763, 4764, 4765, 4766, 4767,
            4768, 4769, 4770, 4771, 4772, 4773, 4774, 4775, 4776, 4777, 4778,
            4779, 4780, 4781, 4782, 4783, 4784, 4785, 4786, 4787, 4788, 4789,
            4790, 4791, 4792, 4793, 4794, 4795, 4796, 4797, 4798, 4799, 4800,
            4801, 4802, 4803, 4804, 4805, 4806, 4807, 4808, 4809, 4810, 4811,
            4812, 4813, 4814, 4815, 4816, 4817, 4818, 4819, 4820, 4821, 4822,
            4823, 4824, 4825, 4826, 4827, 4828, 4829, 4830, 4831, 4832, 4833,
            4834, 4835, 4836, 4837, 4838, 4839, 4840, 4841, 4842, 4843, 4844,
            4845, 4846, 4847, 4848, 4849, 4850, 4851, 4852, 4853, 4854, 4855,
            4856, 4857, 4858, 4859, 4860, 4861, 4862, 4863, 4864, 4865, 4866,
            4867, 4868, 4869, 4870, 4871, 4872, 4873, 4874, 4875, 4876, 4877,
            4878, 4879, 4880, 4881, 4882, 4883, 5267, 5268, 5269, 5270, 5271,
            5272, 5273, 5274, 5275, 5276, 5277, 5278, 5279, 5280, 5281, 5282,
            5283, 5284, 5285, 5286, 5288, 5289, 5290, 5291, 5292, 5293, 5294,
            5295, 5296, 5297, 5298, 5299, 5300, 5301, 5302, 5303, 5304, 5305,
            5306, 5307, 5308, 5309, 5310, 5311, 5312, 5313, 5314, 5315, 5316,
            5317, 5318, 5319, 5320, 5321, 5322, 5323, 5324, 5325, 5326, 5327,
            5328, 5329, 5330, 5331, 5332, 5333, 5334, 5335, 5336, 5337, 5338,
            5339, 5340, 5341, 5342, 5343, 5344, 5345, 5346, 5347, 5348, 5349,
            5350, 5351, 5352, 5353, 5354, 5355, 5356, 5357, 5358, 5359, 5360,
            5361, 5362, 5363, 5364, 5365, 5366, 5367, 5368, 5369, 5370, 5371,
            5372, 5373, 5374, 5375, 5376, 5377, 5378, 5379, 5380, 5381, 5382,
            5383, 5384, 5385, 5386, 5387, 5388, 5389, 5390, 5391, 5392, 5393,
            5394, 5395, 5396, 5397, 5398, 5399, 5400, 5401, 5402, 5403, 5404,
            5405, 5406, 5407, 5408, 5409, 5410, 5411, 5412, 5413, 5414, 5415,
            5416, 5417, 5418, 5419, 5420, 5421, 5422, 5423, 5424, 5425, 5426,
            5427, 5428, 5429, 5430, 5431, 5432, 5433, 5434, 5435, 5436, 5437,
            5438, 5439, 5440, 5441, 5442, 5443, 5444, 5445, 5446, 5447, 5781,
            5782, 5783, 5784, 5785, 5786, 5787, 5788, 5789, 5790, 5791, 5792,
            6843, 6844, 6845, 6846, 6847, 6848, 6849, 6850, 6851, 6852, 6853,
            6854, 8591, 8592,
        };

        for (int i = 0; i < sizeof(map) / sizeof(*map); ++i) {
            RegisterBlock(new Block(blockNames[map[i]], i, true));
        }

        for (int i = 0; i < sizeof(transparent_ids) / sizeof(*transparent_ids);
             ++i) {
            GetBlock(transparent_ids[i])->m_Solid = false;
        }
    }

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

BlockPtr BlockRegistry::GetBlock(const std::string& name) const {
    auto iter = m_BlockNames.find(name);
    if (iter == m_BlockNames.end()) return nullptr;
    return iter->second;
}

}  // namespace block
}  // namespace mc
