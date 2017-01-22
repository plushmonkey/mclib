#ifndef METADATA_H_
#define METADATA_H_

#include "Types.h"
#include "DataBuffer.h"
#include "Slot.h"
#include "Position.h"

#include <array>

namespace Minecraft {

class EntityMetadata {
public:
    class Type {
    public:
        virtual ~Type() { }
    };

    struct ByteType : public Type {
        u8 value;
    };

    struct VarIntType : public Type {
        VarInt value;
    };

    struct FloatType : public Type {
        float value;
    };

    struct StringType : public Type {
        std::wstring value;
    };

    struct SlotType : public Type {
        Slot value;
    };

    struct BooleanType : public Type {
        bool value;
    };

    struct RotationType : public Type {
        Vector3f value;
    };

    struct PositionType : public Type {
        bool exists;
        Position value;
    };

    struct UUIDType : public Type {
        bool exists;
        UUID value;
    };

private:
    enum Types { Byte, VarInt, Float, String, Chat, Slot, Boolean, Rotation, Position, OptPosition, Direction, OptUUID, OptBlockID, None };

    enum { MetadataCount = 36 };
    std::array<std::pair<Type*, Types>, MetadataCount> m_Metadata;

public:
    MCLIB_API EntityMetadata();
    MCLIB_API ~EntityMetadata();

    template <typename T>
    T* GetIndex(std::size_t index) {
        return dynamic_cast<T*>(m_Metadata[index]);
    }

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& metadata);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& metadata);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ByteType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::VarIntType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::SlotType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::BooleanType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::RotationType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::PositionType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::UUIDType& value);

MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ByteType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::VarIntType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::SlotType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::BooleanType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::RotationType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::PositionType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::UUIDType& value);

} // ns Minecraft

#endif
