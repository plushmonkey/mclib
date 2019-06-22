#include <mclib/entity/Metadata.h>

#include <mclib/common/DataBuffer.h>

namespace mc {
namespace entity {

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ByteType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out,
                       const EntityMetadata::VarIntType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out,
                       const EntityMetadata::FloatType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out,
                       const EntityMetadata::StringType& value) {
    MCString str(value.value);

    return out << str;
}

DataBuffer EntityMetadata::SlotType::Serialize(
    mc::protocol::Version protocolVersion) {
    return value.Serialize(protocolVersion);
}

DataBuffer& operator<<(DataBuffer& out,
                       const EntityMetadata::BooleanType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out,
                       const EntityMetadata::RotationType& value) {
    return out << value.value.x << value.value.y << value.value.z;
}

DataBuffer& operator<<(DataBuffer& out,
                       const EntityMetadata::PositionType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::UUIDType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::NBTType& value) {
    return out << value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ByteType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::VarIntType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::StringType& value) {
    MCString str;
    in >> str;

    value.value = str.GetUTF16();

    return in;
}

void EntityMetadata::SlotType::Deserialize(
    DataBuffer& in, mc::protocol::Version protocolVersion) {
    value.Deserialize(in, protocolVersion);
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::BooleanType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::RotationType& value) {
    return in >> value.value.x >> value.value.y >> value.value.z;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::PositionType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::UUIDType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::NBTType& value) {
    return in >> value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& md) {
    for (std::size_t i = 0; i < md.MetadataCount; ++i) {
        EntityMetadata::DataType type = md.m_Metadata[i].second;
        EntityMetadata::Type* value = md.m_Metadata[i].first.get();

        if (!value) continue;

        u8 item = ((type << 5) | (i & 0x1F)) & 0xFF;

        out << item;

        if (md.m_ProtocolVersion <= protocol::Version::Minecraft_1_12_2 &&
            type >= 5) {
            type = static_cast<EntityMetadata::DataType>(
                static_cast<int>(type) + 1);
        }

        switch (type) {
            case EntityMetadata::DataType::Byte:
                out << *((EntityMetadata::ByteType*)value);
                break;
            case EntityMetadata::DataType::VarInt:
            case EntityMetadata::DataType::Direction:
            case EntityMetadata::DataType::OptBlockID:
                out << *((EntityMetadata::VarIntType*)value);
                break;
            case EntityMetadata::DataType::Float:
                out << *((EntityMetadata::FloatType*)value);
                break;
            case EntityMetadata::DataType::String:
            case EntityMetadata::DataType::Chat:
                out << *((EntityMetadata::StringType*)value);
                break;
            case EntityMetadata::DataType::OptChat:
                out << ((EntityMetadata::StringType*)value)->exists;
                if (((EntityMetadata::StringType*)value)->exists) {
                    out << *((EntityMetadata::StringType*)value);
                }
                break;
            case EntityMetadata::DataType::Slot: {
                DataBuffer serializedSlot =
                    ((EntityMetadata::SlotType*)value)
                        ->Serialize(md.m_ProtocolVersion);
                out << serializedSlot;
            } break;
            case EntityMetadata::DataType::Boolean:
                out << *((EntityMetadata::BooleanType*)value);
                break;
            case EntityMetadata::DataType::Rotation:
                out << *((EntityMetadata::RotationType*)value);
                break;
            case EntityMetadata::DataType::Position:
                out << *((EntityMetadata::PositionType*)value);
                break;
            case EntityMetadata::DataType::OptPosition:
                out << ((EntityMetadata::PositionType*)value)->exists;
                out << *((EntityMetadata::PositionType*)value);
                break;
            case EntityMetadata::DataType::OptUUID:
                out << ((EntityMetadata::UUIDType*)value)->exists;
                out << *((EntityMetadata::UUIDType*)value);
                break;
            case EntityMetadata::DataType::NBT:
                out << *((EntityMetadata::NBTType*)value);
                break;
            default:
                break;
        }
    }

    // End byte
    out << (u8)0x7F;
    return out;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata& md) {
    while (true) {
        u8 index;

        in >> index;

        if (index == 0xFF) return in;

        u8 typeVal;

        in >> typeVal;

        EntityMetadata::DataType type = (EntityMetadata::DataType)(typeVal);

        if (md.m_ProtocolVersion <= protocol::Version::Minecraft_1_12_2 &&
            type >= 5) {
            type = static_cast<EntityMetadata::DataType>(
                static_cast<int>(type) + 1);
        }

        md.m_Metadata[index].second = type;

        switch (type) {
            case EntityMetadata::DataType::Byte: {
                std::unique_ptr<EntityMetadata::ByteType> value =
                    std::make_unique<EntityMetadata::ByteType>();
                in >> *value;
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::VarInt:
            case EntityMetadata::DataType::Direction:
            case EntityMetadata::DataType::OptBlockID: {
                std::unique_ptr<EntityMetadata::VarIntType> value =
                    std::make_unique<EntityMetadata::VarIntType>();
                in >> *value;
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::Float: {
                std::unique_ptr<EntityMetadata::FloatType> value =
                    std::make_unique<EntityMetadata::FloatType>();
                in >> *value;
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::Chat:
            case EntityMetadata::DataType::String: {
                std::unique_ptr<EntityMetadata::StringType> value =
                    std::make_unique<EntityMetadata::StringType>();
                in >> *value;
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::OptChat: {
                std::unique_ptr<EntityMetadata::StringType> value =
                    std::make_unique<EntityMetadata::StringType>();
                in >> value->exists;
                if (value->exists) {
                    in >> *value;
                }
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::Slot: {
                std::unique_ptr<EntityMetadata::SlotType> value =
                    std::make_unique<EntityMetadata::SlotType>();
                value->Deserialize(in, md.m_ProtocolVersion);
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::Boolean: {
                std::unique_ptr<EntityMetadata::BooleanType> value =
                    std::make_unique<EntityMetadata::BooleanType>();
                in >> *value;
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::Rotation: {
                std::unique_ptr<EntityMetadata::RotationType> value =
                    std::make_unique<EntityMetadata::RotationType>();
                in >> *value;
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::Position: {
                std::unique_ptr<EntityMetadata::PositionType> value =
                    std::make_unique<EntityMetadata::PositionType>();
                in >> *value;
                value->exists = true;
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::OptPosition: {
                std::unique_ptr<EntityMetadata::PositionType> value =
                    std::make_unique<EntityMetadata::PositionType>();
                in >> value->exists;
                if (value->exists) {
                    in >> *value;
                }
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::OptUUID: {
                std::unique_ptr<EntityMetadata::UUIDType> value =
                    std::make_unique<EntityMetadata::UUIDType>();
                in >> value->exists;
                if (value->exists) {
                    in >> *value;
                }
                md.m_Metadata[index].first = std::move(value);
            } break;
            case EntityMetadata::DataType::NBT: {
                std::unique_ptr<EntityMetadata::NBTType> value =
                    std::make_unique<EntityMetadata::NBTType>();
                in >> *value;
                md.m_Metadata[index].first = std::move(value);
            } break;
            default:
                break;
        }
    }

    return in;
}

void EntityMetadata::CopyOther(const EntityMetadata& other) {
    m_ProtocolVersion = other.m_ProtocolVersion;

    for (std::size_t i = 0; i < MetadataCount; ++i) {
        auto type = m_Metadata[i].second = other.m_Metadata[i].second;

        switch (type) {
            case DataType::Byte:
                m_Metadata[i].first = std::make_unique<ByteType>(
                    dynamic_cast<ByteType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::Direction:
            case DataType::OptBlockID:
            case DataType::VarInt:
                m_Metadata[i].first = std::make_unique<VarIntType>(
                    dynamic_cast<VarIntType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::Float:
                m_Metadata[i].first = std::make_unique<FloatType>(
                    dynamic_cast<FloatType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::String:
            case DataType::Chat:
            case DataType::OptChat:
                m_Metadata[i].first = std::make_unique<StringType>(
                    dynamic_cast<StringType*>(other.m_Metadata[i].first.get())
                        ->exists,
                    dynamic_cast<StringType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::Slot:
                m_Metadata[i].first = std::make_unique<SlotType>(
                    dynamic_cast<SlotType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::Boolean:
                m_Metadata[i].first = std::make_unique<BooleanType>(
                    dynamic_cast<BooleanType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::Rotation:
                m_Metadata[i].first = std::make_unique<RotationType>(
                    dynamic_cast<RotationType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::OptPosition:
            case DataType::Position:
                m_Metadata[i].first = std::make_unique<PositionType>(
                    dynamic_cast<PositionType*>(other.m_Metadata[i].first.get())
                        ->exists,
                    dynamic_cast<PositionType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::OptUUID:
                m_Metadata[i].first = std::make_unique<UUIDType>(
                    dynamic_cast<UUIDType*>(other.m_Metadata[i].first.get())
                        ->exists,
                    dynamic_cast<UUIDType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            case DataType::NBT:
                m_Metadata[i].first = std::make_unique<NBTType>(
                    dynamic_cast<NBTType*>(other.m_Metadata[i].first.get())
                        ->value);
                break;
            default:
                break;
        }
    }
}

EntityMetadata::EntityMetadata(protocol::Version protocolVersion)
    : m_ProtocolVersion(protocolVersion) {
    for (std::size_t i = 0; i < MetadataCount; ++i) {
        m_Metadata[i].first = nullptr;
        m_Metadata[i].second = DataType::None;
    }
}

EntityMetadata::EntityMetadata(const EntityMetadata& rhs) { CopyOther(rhs); }

EntityMetadata& EntityMetadata::operator=(const EntityMetadata& rhs) {
    CopyOther(rhs);
    return *this;
}

}  // namespace entity
}  // namespace mc
