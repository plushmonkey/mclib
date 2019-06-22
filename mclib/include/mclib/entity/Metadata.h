#ifndef MCLIB_ENTITY_METADATA_H_
#define MCLIB_ENTITY_METADATA_H_

#include <mclib/common/Position.h>
#include <mclib/common/Types.h>
#include <mclib/common/UUID.h>
#include <mclib/inventory/Slot.h>
#include <mclib/protocol/ProtocolState.h>

#include <array>
#include <memory>

namespace mc {

class DataBuffer;

namespace entity {

class EntityMetadata {
public:
    class Type {
    public:
        virtual ~Type() {}
    };

    struct ByteType : public Type {
        u8 value;

        ByteType() = default;
        ByteType(u8 value) : value(value) {}
    };

    struct VarIntType : public Type {
        VarInt value;

        VarIntType() = default;
        VarIntType(VarInt value) : value(value) {}
    };

    struct FloatType : public Type {
        float value;

        FloatType() = default;
        FloatType(float value) : value(value) {}
    };

    struct StringType : public Type {
        bool exists;
        std::wstring value;

        StringType() : exists(true) {}
        StringType(bool exists, const std::wstring& value)
            : exists(exists), value(value) {}
    };

    struct SlotType : public Type {
        inventory::Slot value;

        SlotType() = default;
        SlotType(const inventory::Slot& value) : value(value) {}

        DataBuffer Serialize(mc::protocol::Version protocolVersion);
        void Deserialize(DataBuffer& in, mc::protocol::Version protocolVersion);
    };

    struct BooleanType : public Type {
        bool value;

        BooleanType() = default;
        BooleanType(bool value) : value(value) {}
    };

    struct RotationType : public Type {
        Vector3f value;

        RotationType() = default;
        RotationType(Vector3f value) : value(value) {}
    };

    struct PositionType : public Type {
        bool exists;
        Position value;

        PositionType() = default;
        PositionType(bool exists, Position value)
            : exists(exists), value(value) {}
    };

    struct UUIDType : public Type {
        bool exists;
        UUID value;

        UUIDType() = default;
        UUIDType(bool exists, UUID value) : exists(exists), value(value) {}
    };

    struct NBTType : public Type {
        nbt::NBT value;

        NBTType() = default;
        NBTType(nbt::NBT value) : value(value) {}
    };

private:
    enum DataType {
        Byte,
        VarInt,
        Float,
        String,
        Chat,
        OptChat,
        Slot,
        Boolean,
        Rotation,
        Position,
        OptPosition,
        Direction,
        OptUUID,
        OptBlockID,
        NBT,
        Particle,
        None
    };

    enum { MetadataCount = 0xFE };
    std::array<std::pair<std::unique_ptr<Type>, DataType>, MetadataCount>
        m_Metadata;
    protocol::Version m_ProtocolVersion;

    void CopyOther(const EntityMetadata& other);

public:
    MCLIB_API EntityMetadata(protocol::Version protocolVersion);

    MCLIB_API EntityMetadata(const EntityMetadata& rhs);
    MCLIB_API EntityMetadata& operator=(const EntityMetadata& rhs);
    MCLIB_API EntityMetadata(EntityMetadata&& rhs) = default;
    MCLIB_API EntityMetadata& operator=(EntityMetadata&& rhs) = default;

    template <typename T>
    T* GetIndex(std::size_t index) const {
        return dynamic_cast<T*>(m_Metadata[index].first.get());
    }

    void MCLIB_API SetProtocolVersion(protocol::Version version) {
        m_ProtocolVersion = version;
    }

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                            const EntityMetadata& metadata);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                            EntityMetadata& metadata);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata& metadata);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::ByteType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::VarIntType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::BooleanType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::RotationType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::PositionType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::UUIDType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out,
                                 const EntityMetadata::NBTType& value);

MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::ByteType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::VarIntType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::BooleanType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::RotationType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::PositionType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::UUIDType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in,
                                 EntityMetadata::NBTType& value);

}  // namespace entity
}  // namespace mc

#endif
