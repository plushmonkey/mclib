#ifndef NBT_H_
#define NBT_H_

#include "Types.h"
#include "mclib.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace Minecraft {

class DataBuffer;

namespace NBT {
    
enum class TagType { End, Byte, Short, Int, Long, Float, Double, ByteArray, String, List, Compound, IntArray };

class TagList;
class TagCompound;

class Tag {
protected:
    std::wstring m_Name;
    virtual void Write(DataBuffer& buffer) const = 0;
    virtual void Read(DataBuffer& buffer) = 0;

public:
    MCLIB_API Tag(const std::string& name) : m_Name(name.begin(), name.end()) { }
    MCLIB_API Tag(const std::wstring& name) : m_Name(name) { }
    MCLIB_API std::wstring GetName() const;
    void MCLIB_API SetName(const std::wstring& name) { m_Name = name; }
    virtual TagType MCLIB_API GetType() const = 0;
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, Tag& tag);
    friend class TagList;
    friend class TagCompound;
};

typedef std::shared_ptr<Tag> TagPtr;

class TagString : public Tag {
private:
    std::wstring m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagString() : Tag(L"") { }
    MCLIB_API TagString(std::wstring name, std::wstring val) : Tag(name), m_Value(val) { }
    MCLIB_API TagString(std::string name, std::string val) : Tag(name), m_Value(val.begin(), val.end()) { }

    TagType MCLIB_API GetType() const;

    std::wstring MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagByteArray : public Tag {
private:
    std::string m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagByteArray() : Tag(L"") { }
    MCLIB_API TagByteArray(std::wstring name, std::string val) : Tag(name), m_Value(val) { }
    MCLIB_API TagByteArray(std::string name, std::string val) : Tag(name), m_Value(val) { }

    TagType MCLIB_API GetType() const;

    std::string MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagIntArray : public Tag {
private:
    std::vector<s32> m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagIntArray() : Tag(L"") { }
    MCLIB_API TagIntArray(std::wstring name, std::vector<s32> val) : Tag(name), m_Value(val) { }
    MCLIB_API TagIntArray(std::string name, std::vector<s32> val) : Tag(name), m_Value(val) { }

    TagType MCLIB_API GetType() const;

    std::vector<s32> MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagList : public Tag {
private:
    std::vector<TagPtr> m_Tags;
    TagType m_ListType;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagList() : Tag(L""), m_ListType(TagType::End) { }
    MCLIB_API TagList(std::wstring name, TagType listType) : Tag(name), m_ListType(listType) { }
    MCLIB_API TagList(std::string name, TagType listType) : Tag(name), m_ListType(listType) { }
    MCLIB_API ~TagList();

    TagType MCLIB_API GetType() const;
    TagType MCLIB_API GetListType() const { return m_ListType; }
    std::vector<TagPtr> MCLIB_API GetList() const { return m_Tags; }

    void MCLIB_API AddItem(TagPtr item);
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagList& tag);
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagCompound : public Tag {
private:
    std::vector<TagPtr> m_Tags;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagCompound() : Tag(L"") { }
    MCLIB_API TagCompound(const std::wstring& name) : Tag(name) { }
    MCLIB_API TagCompound(const std::string& name) : Tag(name) { }
    MCLIB_API ~TagCompound();

    TagType MCLIB_API GetType() const;

    std::vector<TagPtr>::iterator MCLIB_API begin() { return m_Tags.begin(); }
    std::vector<TagPtr>::iterator MCLIB_API end() { return m_Tags.end(); }
    std::vector<TagPtr>::const_iterator MCLIB_API begin() const { return m_Tags.begin(); }
    std::vector<TagPtr>::const_iterator MCLIB_API end() const { return m_Tags.end(); }

    void MCLIB_API AddItem(TagPtr item);
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagCompound& tag);
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagByte : public Tag {
private:
    u8 m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagByte() : Tag(L""), m_Value(0) { }
    MCLIB_API TagByte(std::wstring name, u8 value) : Tag(name), m_Value(value) { }
    MCLIB_API TagByte(std::string name, u8 value) : Tag(name), m_Value(value) { }
    TagType MCLIB_API GetType() const;

    u8 MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagShort : public Tag {
private:
    s16 m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagShort() : Tag(L""), m_Value(0) { }
    MCLIB_API TagShort(std::wstring name, s16 value) : Tag(name), m_Value(value) { }
    MCLIB_API TagShort(std::string name, s16 value) : Tag(name), m_Value(value) { }
    TagType MCLIB_API GetType() const;

    s16 MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagInt : public Tag {
private:
    s32 m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagInt() : Tag(L""), m_Value(0) { }
    MCLIB_API TagInt(std::wstring name, s32 value) : Tag(name), m_Value(value) { }
    MCLIB_API TagInt(std::string name, s32 value) : Tag(name), m_Value(value) { }
    TagType MCLIB_API GetType() const;

    s32 MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagLong : public Tag {
private:
    s64 m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagLong() : Tag(L""), m_Value(0) { }
    MCLIB_API TagLong(std::wstring name, s64 value) : Tag(name), m_Value(value) { }
    MCLIB_API TagLong(std::string name, s64 value) : Tag(name), m_Value(value) { }
    TagType MCLIB_API GetType() const;

    s64 MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagFloat : public Tag {
private:
    float m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagFloat() : Tag(L""), m_Value(0.0f) { }
    MCLIB_API TagFloat(std::wstring name, float value) : Tag(name), m_Value(value) { }
    MCLIB_API TagFloat(std::string name, float value) : Tag(name), m_Value(value) { }
    TagType MCLIB_API GetType() const;

    float MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagDouble : public Tag {
private:
    double m_Value;

    void MCLIB_API Write(DataBuffer& buffer) const;
    void MCLIB_API Read(DataBuffer& buffer);

public:
    MCLIB_API TagDouble() : Tag(L""), m_Value(0.0) { }
    MCLIB_API TagDouble(std::wstring name, double value) : Tag(name), m_Value(value) { }
    MCLIB_API TagDouble(std::string name, double value) : Tag(name), m_Value(value) { }
    TagType MCLIB_API GetType() const;

    double MCLIB_API GetValue() const { return m_Value; }
    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};


class NBT {
private:
    TagCompound m_Root;

public:
    MCLIB_API NBT() { }

    TagCompound& GetRoot() { return m_Root; }
    const TagCompound& GetRoot() const { return m_Root; }
    bool HasData() const { return m_Root.begin() != m_Root.end(); }

    friend MCLIB_API DataBuffer& operator>>(DataBuffer& out, NBT& nbt);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagString& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagByteArray& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagList& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagCompound& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagIntArray& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagByte& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagShort& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagInt& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagLong& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagFloat& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const TagDouble& tag);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const NBT& nbt);

MCLIB_API DataBuffer& operator>>(DataBuffer& in, NBT& nbt);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, Tag& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagString& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagByteArray& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagList& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagCompound& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagIntArray& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagByte& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagShort& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagInt& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagLong& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagFloat& tag);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, TagDouble& tag);

} // ns NBT

} // ns Minecraft

#endif
