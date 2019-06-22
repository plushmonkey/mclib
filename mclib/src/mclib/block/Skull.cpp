#include <mclib/block/Skull.h>

#include <unordered_map>

namespace std {

template <>
struct hash<mc::block::SkullType> {
    std::size_t operator()(const mc::block::SkullType& type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std

namespace mc {
namespace block {

bool Skull::ImportNBT(nbt::NBT* nbt) {
    auto typeTag = nbt->GetTag<nbt::TagByte>(L"SkullType");
    auto rotTag = nbt->GetTag<nbt::TagByte>(L"Rot");
    auto ownerTag = nbt->GetTag<nbt::TagCompound>(L"Owner");

    if (typeTag) m_Type = static_cast<SkullType>(typeTag->GetValue());

    if (rotTag) m_Rotation = rotTag->GetValue();

    if (ownerTag) {
        auto uuidTag = ownerTag->GetTag<nbt::TagString>(L"Id");
        auto nameTag = ownerTag->GetTag<nbt::TagString>(L"Name");
        auto propertiesTag = ownerTag->GetTag<nbt::TagCompound>(L"Properties");

        if (uuidTag) m_OwnerUUID = UUID::FromString(uuidTag->GetValue());

        if (nameTag) m_OwnerName = nameTag->GetValue();

        if (propertiesTag) {
            auto texturesTag = propertiesTag->GetTag<nbt::TagList>(L"textures");

            if (texturesTag) {
                for (auto iter = texturesTag->begin();
                     iter != texturesTag->end(); ++iter) {
                    nbt::TagCompound* textureCompound =
                        dynamic_cast<nbt::TagCompound*>(iter->get());

                    if (!textureCompound) continue;

                    auto signatureTag =
                        textureCompound->GetTag<nbt::TagString>(L"Signature");
                    auto valueTag =
                        textureCompound->GetTag<nbt::TagString>(L"Value");

                    if (!signatureTag || !valueTag) continue;

                    Texture texture;

                    if (signatureTag)
                        texture.signature = signatureTag->GetValue();

                    if (valueTag) texture.value = valueTag->GetValue();

                    m_Textures.push_back(texture);
                }
            }
        }
    }

    return true;
}

const std::unordered_map<SkullType, std::wstring> stringMap = {
    {SkullType::Skeleton, L"Skeleton"},
    {SkullType::WitherSkeleton, L"Wither Skeleton"},
    {SkullType::Zombie, L"Zombie"},
    {SkullType::Head, L"Head"},
    {SkullType::Creeper, L"Creeper"},
    {SkullType::Dragon, L"Dragon"},
};

std::string to_string(SkullType type) {
    std::wstring result = stringMap.at(type);

    return std::string(result.begin(), result.end());
}

std::wstring to_wstring(SkullType type) { return stringMap.at(type); }

}  // namespace block
}  // namespace mc
