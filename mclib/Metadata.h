#ifndef METADATA_H_
#define METADATA_H_

#include "Types.h"
#include "DataBuffer.h"
#include "Slot.h"

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

    struct ShortType : public Type {
        s16 value;
    };

    struct IntType : public Type {
        s32 value;
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

    struct IntVectorType : public Type {
        struct {
            s32 x;
            s32 y;
            s32 z;
        } value;
    };

    struct FloatVectorType : public Type {
        struct {
            float pitch;
            float yaw;
            float roll;
        } value;
    };

private:
    enum Types { Byte, Short, Int, Float, String, Slot, IntVector, FloatVector, None };

    enum { MetadataCount = 25 };
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
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ShortType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::IntType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::SlotType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::IntVectorType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatVectorType& value);

MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ByteType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ShortType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::IntType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::SlotType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::IntVectorType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatVectorType& value);

} // ns Minecraft

#endif
