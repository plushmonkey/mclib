#ifndef ENTITY_H_
#define ENTITY_H_

#include "Types.h"
#include "DataBuffer.h"
#include "Slot.h"

#include <array>
#include <string>

namespace Minecraft {


typedef s32 EntityId;

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
    EntityMetadata();
    ~EntityMetadata();

    template <typename T>
    T* GetIndex(std::size_t index) {
        return dynamic_cast<T*>(m_Metadata[index]);
    }

    friend DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& metadata);
    friend DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
};

DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& metadata);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ByteType& value);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ShortType& value);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::IntType& value);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatType& value);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::StringType& value);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::SlotType& value);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::IntVectorType& value);
DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatVectorType& value);

DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ByteType& value);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ShortType& value);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::IntType& value);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatType& value);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::StringType& value);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::SlotType& value);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::IntVectorType& value);
DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatVectorType& value);

} // ns Minecraft

#endif
