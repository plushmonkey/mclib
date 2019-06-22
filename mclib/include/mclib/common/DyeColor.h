#ifndef MCLIB_COMMON_DYECOLOR_H_
#define MCLIB_COMMON_DYECOLOR_H_

#include <mclib/common/Vector.h>
#include <mclib/mclib.h>

namespace mc {

enum DyeColor {
    Black,
    Red,
    Green,
    Brown,
    Blue,
    Purple,
    Cyan,
    LightGray,
    Gray,
    Pink,
    Lime,
    Yellow,
    LightBlue,
    Magenta,
    Orange,
    White,
};

MCLIB_API Vector3d GetColor(DyeColor dyeColor);
MCLIB_API std::string to_string(DyeColor dyeColor);
MCLIB_API std::wstring to_wstring(DyeColor dyeColor);

}  // namespace mc

#endif
