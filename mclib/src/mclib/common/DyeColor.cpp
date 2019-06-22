#include <mclib/common/DyeColor.h>

#include <unordered_map>

namespace std {

template <>
struct hash<mc::DyeColor> {
    std::size_t operator()(const mc::DyeColor& color) const {
        return std::hash<int>()(static_cast<int>(color));
    }
};

}  // namespace std

namespace mc {

constexpr Vector3d normRGB(int x, int y, int z) {
    return Vector3d(x / 255.0, y / 255.0, z / 255.0);
}

const std::unordered_map<DyeColor, Vector3d> colorMap = {
    {DyeColor::White, normRGB(255, 255, 255)},
    {DyeColor::Orange, normRGB(216, 127, 51)},
    {DyeColor::Magenta, normRGB(178, 76, 216)},
    {DyeColor::LightBlue, normRGB(102, 153, 216)},
    {DyeColor::Yellow, normRGB(229, 229, 51)},
    {DyeColor::Lime, normRGB(127, 204, 25)},
    {DyeColor::Pink, normRGB(242, 127, 165)},
    {DyeColor::Gray, normRGB(76, 76, 76)},
    {DyeColor::LightGray, normRGB(153, 153, 153)},
    {DyeColor::Cyan, normRGB(76, 127, 153)},
    {DyeColor::Purple, normRGB(127, 63, 178)},
    {DyeColor::Blue, normRGB(51, 76, 178)},
    {DyeColor::Brown, normRGB(102, 76, 51)},
    {DyeColor::Green, normRGB(102, 127, 51)},
    {DyeColor::Red, normRGB(153, 51, 51)},
    {DyeColor::Black, normRGB(25, 25, 25)},
};

const std::unordered_map<DyeColor, std::wstring> stringMap = {
    {DyeColor::White, L"White"},
    {DyeColor::Orange, L"Orange"},
    {DyeColor::Magenta, L"Magenta"},
    {DyeColor::LightBlue, L"Light Blue"},
    {DyeColor::Yellow, L"Yellow"},
    {DyeColor::Lime, L"Lime"},
    {DyeColor::Pink, L"Pink"},
    {DyeColor::Gray, L"Gray"},
    {DyeColor::LightGray, L"LightGray"},
    {DyeColor::Cyan, L"Cyan"},
    {DyeColor::Purple, L"Purple"},
    {DyeColor::Blue, L"Blue"},
    {DyeColor::Brown, L"Brown"},
    {DyeColor::Green, L"Green"},
    {DyeColor::Red, L"Red"},
    {DyeColor::Black, L"Black"},
};

Vector3d GetColor(DyeColor dyeColor) { return colorMap.at(dyeColor); }

std::string to_string(DyeColor dyeColor) {
    std::wstring result = stringMap.at(dyeColor);

    return std::string(result.begin(), result.end());
}

std::wstring to_wstring(DyeColor dyeColor) { return stringMap.at(dyeColor); }

}  // namespace mc
