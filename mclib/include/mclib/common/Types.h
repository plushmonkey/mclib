#ifndef MCLIB_COMMON_TYPES_H_
#define MCLIB_COMMON_TYPES_H_

#include <cstddef>
#include <cstdint>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#ifdef _MSC_VER
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

namespace mc {

typedef s32 EntityId;

enum class Hand { Main, Off };
enum class MainHand { Left, Right };

enum class ChatMode { Enabled, Commands, Hidden };

enum class SkinPart {
    Cape = 0x1,
    Jacket = 0x2,
    LeftSleeve = 0x4,
    RightSleeve = 0x8,
    LeftPants = 0x10,
    RightPants = 0x20,
    Hat = 0x40
};

enum class Face { Bottom = 0, Top, North, South, West, East };

}  // namespace mc

#include <mclib/common/Vector.h>
#include <mclib/mclib.h>

#endif
