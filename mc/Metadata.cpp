#include "Metadata.h"


namespace Minecraft {


DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ByteType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ShortType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::IntType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::SlotType& value) {
    return out << value.value;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::IntVectorType& value) {
    return out << value.value.x << value.value.y << value.value.z;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatVectorType& value) {
    return out << value.value.pitch << value.value.yaw << value.value.roll;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::StringType& value) {
    MCString str(value.value);

    return out << str;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ByteType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ShortType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::IntType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::SlotType& value) {
    return in >> value.value;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::IntVectorType& value) {
    return in >> value.value.x >> value.value.y >> value.value.z;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatVectorType& value) {
    return in >> value.value.pitch >> value.value.yaw >> value.value.roll;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata::StringType& value) {
    MCString str;
    in >> str;

    value.value = str.GetUTF16();

    return in;
}

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& md) {
    for (std::size_t i = 0; i < md.MetadataCount; ++i) {
        EntityMetadata::Types type = md.m_Metadata[i].second;
        EntityMetadata::Type* value = md.m_Metadata[i].first;

        if (!value || type == EntityMetadata::Types::None) continue;

        u8 item = ((type << 5) | (i & 0x1F)) & 0xFF;

        out << item;

        switch (type) {
        case EntityMetadata::Types::Byte:
            out << *((EntityMetadata::ByteType*)value);
        case EntityMetadata::Types::Short:
            out << *((EntityMetadata::ShortType*)value);
        case EntityMetadata::Types::Int:
            out << *((EntityMetadata::IntType*)value);
        case EntityMetadata::Types::Float:
            out << *((EntityMetadata::FloatType*)value);
        case EntityMetadata::Types::String:
            out << *((EntityMetadata::StringType*)value);
        case EntityMetadata::Types::Slot:
            out << *((EntityMetadata::SlotType*)value);
        case EntityMetadata::Types::IntVector:
            out << *((EntityMetadata::IntVectorType*)value);
        case EntityMetadata::Types::FloatVector:
            out << *((EntityMetadata::FloatVectorType*)value);
        }
    }

    // End byte
    out << (u8)0x7F;
    return out;
}

DataBuffer& operator>>(DataBuffer& in, EntityMetadata& md) {
    while (true) {
        u8 item;

        in >> item;

        if (item == 0x7F) return in;

        EntityMetadata::Types type = (EntityMetadata::Types)(item >> 5);
        u8 index = item & 0x1F;

        switch (type) {
        case EntityMetadata::Types::Byte:
        {
            EntityMetadata::ByteType* value = new EntityMetadata::ByteType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::Byte;
        }
        break;
        case EntityMetadata::Types::Short:
        {
            EntityMetadata::ShortType* value = new EntityMetadata::ShortType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::Short;
        }
        break;
        case EntityMetadata::Types::Int:
        {
            EntityMetadata::IntType* value = new EntityMetadata::IntType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::Int;
        }
        break;
        case EntityMetadata::Types::Float:
        {
            EntityMetadata::FloatType* value = new EntityMetadata::FloatType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::Float;
        }
        break;
        case EntityMetadata::Types::String:
        {
            EntityMetadata::StringType* value = new EntityMetadata::StringType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::String;
        }
        break;
        case EntityMetadata::Types::Slot:
        {
            EntityMetadata::SlotType* value = new EntityMetadata::SlotType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::Slot;
        }
        break;
        case EntityMetadata::Types::IntVector:
        {
            EntityMetadata::IntVectorType* value = new EntityMetadata::IntVectorType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::IntVector;
        }
        break;
        case EntityMetadata::Types::FloatVector:
        {
            EntityMetadata::FloatVectorType* value = new EntityMetadata::FloatVectorType;
            in >> *value;
            md.m_Metadata[index].first = value;
            md.m_Metadata[index].second = EntityMetadata::Types::FloatVector;
        }
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
