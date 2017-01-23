#include "Metadata.h"


namespace Minecraft {


DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ByteType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::VarIntType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::StringType& value) {
    MCString str(value.value);

    return out << str;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::SlotType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::BooleanType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::RotationType& value) {
    return out << value.value.x << value.value.y << value.value.z;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::PositionType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::UUIDType& value) {
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

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::SlotType& value) {
    return in >> value.value;
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


DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& md) {
    for (std::size_t i = 0; i < md.MetadataCount; ++i) {
        EntityMetadata::Types type = md.m_Metadata[i].second;
        EntityMetadata::Type* value = md.m_Metadata[i].first;

        if (!value) continue;

        u8 item = ((type << 5) | (i & 0x1F)) & 0xFF;

        out << item;

        switch (type) {
        case EntityMetadata::Types::Byte:
            out << *((EntityMetadata::ByteType*)value);
            break;
        case EntityMetadata::Types::VarInt:
        case EntityMetadata::Types::Direction:
        case EntityMetadata::Types::OptBlockID:
            out << *((EntityMetadata::VarIntType*)value);
            break;
        case EntityMetadata::Types::Float:
            out << *((EntityMetadata::FloatType*)value);
            break;
        case EntityMetadata::Types::String:
        case EntityMetadata::Types::Chat:
            out << *((EntityMetadata::StringType*)value);
            break;
        case EntityMetadata::Types::Slot:
            out << *((EntityMetadata::SlotType*)value);
            break;
        case EntityMetadata::Types::Boolean:
            out << *((EntityMetadata::BooleanType*)value);
            break;
        case EntityMetadata::Types::Rotation:
            out << *((EntityMetadata::RotationType*)value);
            break;
        case EntityMetadata::Types::Position:
            out << *((EntityMetadata::PositionType*)value);
            break;
        case EntityMetadata::Types::OptPosition:
            out << ((EntityMetadata::PositionType*)value)->exists;
            out << *((EntityMetadata::PositionType*)value);
            break;
        case EntityMetadata::Types::OptUUID:
            out << ((EntityMetadata::UUIDType*)value)->exists;
            out << *((EntityMetadata::UUIDType*)value);
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

        EntityMetadata::Types type = (EntityMetadata::Types)(typeVal);
        md.m_Metadata[index].second = type;

        switch (type) {
        case EntityMetadata::Types::Byte:
        {
            EntityMetadata::ByteType* value = new EntityMetadata::ByteType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::VarInt:
        case EntityMetadata::Types::Direction:
        case EntityMetadata::Types::OptBlockID:
        {
            EntityMetadata::VarIntType* value = new EntityMetadata::VarIntType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::Float:
        {
            EntityMetadata::FloatType* value = new EntityMetadata::FloatType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::Chat:
        case EntityMetadata::Types::String:
        {
            EntityMetadata::StringType* value = new EntityMetadata::StringType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::Slot:
        {
            EntityMetadata::SlotType* value = new EntityMetadata::SlotType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::Boolean:
        {
            EntityMetadata::BooleanType* value = new EntityMetadata::BooleanType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::Rotation:
        {
            EntityMetadata::RotationType* value = new EntityMetadata::RotationType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::Position:
        {
            EntityMetadata::PositionType* value = new EntityMetadata::PositionType;
            in >> *value;
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::OptPosition:
        {
            EntityMetadata::PositionType* value = new EntityMetadata::PositionType;
            in >> value->exists;
            if (value->exists) {
                in >> *value;
            }
            md.m_Metadata[index].first = value;
        }
        break;
        case EntityMetadata::Types::OptUUID:
        {
            EntityMetadata::UUIDType* value = new EntityMetadata::UUIDType;
            in >> value->exists;
            if (value->exists) {
                in >> *value;
            }
            md.m_Metadata[index].first = value;
        }
        break;
        default:
        break;
        }
    }
    return in;
}

EntityMetadata::EntityMetadata() {
    for (std::size_t i = 0; i < MetadataCount; ++i) {
        m_Metadata[i].first = nullptr;
        m_Metadata[i].second = Types::None;
    }
}

EntityMetadata::~EntityMetadata() {
    for (std::size_t i = 0; i < MetadataCount; ++i) {
        if (m_Metadata[i].first)
            delete m_Metadata[i].first;
    }
}

} // ns Minecraft
