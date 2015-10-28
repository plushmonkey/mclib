#ifndef NBT_H_
#define NBT_H_

#include "Types.h"
#include <string>
#include <vector>
#include <memory>

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
    Tag(std::wstring name);
    std::wstring GetName() const;
    void SetName(const std::wstring& name) { m_Name = name; }
    virtual TagType GetType() const = 0;
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
    friend DataBuffer& operator>>(DataBuffer& in, Tag& tag);
    friend class TagList;
    friend class TagCompound;
};

typedef std::shared_ptr<Tag> TagPtr;

class TagString : public Tag {
private:
    std::wstring m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagString() : Tag(L"") { }
    TagString(std::wstring name, std::wstring val) : Tag(name), m_Value(val) { }

    TagType GetType() const;

    std::wstring GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagByteArray : public Tag {
private:
    std::string m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagByteArray() : Tag(L"") { }
    TagByteArray(std::wstring name, std::string val) : Tag(name), m_Value(val) { }

    TagType GetType() const;

    std::string GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagIntArray: public Tag {
private:
    std::vector<s32> m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagIntArray() : Tag(L"") { }
    TagIntArray(std::wstring name, std::vector<s32> val) : Tag(name), m_Value(val) { }

    TagType GetType() const;

    std::vector<s32> GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagList : public Tag {
private:
    std::vector<TagPtr> m_Tags;
    TagType m_ListType;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagList() : Tag(L""), m_ListType(TagType::End) { }
    TagList(std::wstring name, TagType listType);
    ~TagList();

    TagType GetType() const;
    TagType GetListType() const { return m_ListType; }
    std::vector<TagPtr> GetList() const { return m_Tags; }

    void AddItem(TagPtr item);
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagCompound : public Tag {
private:
    std::vector<TagPtr> m_Tags;
    TagType m_ListType;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);
public:
    TagCompound() : Tag(L""), m_ListType(TagType::End) { }
    ~TagCompound();

    TagType GetType() const;
    TagType GetListType() const { return m_ListType; }
    std::vector<TagPtr> GetList() const { return m_Tags; }

    void AddItem(TagPtr item);
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagByte : public Tag {
private:
    u8 m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagByte() : Tag(L""), m_Value(0) { }
    TagByte(std::wstring name, u8 value) : Tag(name), m_Value(value) { }
    TagType GetType() const;

    u8 GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagShort : public Tag {
private:
    s16 m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagShort() : Tag(L""), m_Value(0) { }
    TagShort(std::wstring name, s16 value) : Tag(name), m_Value(value) { }
    TagType GetType() const;

    s16 GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagInt : public Tag {
private:
    s32 m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagInt() : Tag(L""), m_Value(0) { }
    TagInt(std::wstring name, s32 value) : Tag(name), m_Value(value) { }
    TagType GetType() const;

    s32 GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagLong : public Tag {
private:
    s64 m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagLong() : Tag(L""), m_Value(0) { }
    TagLong(std::wstring name, s64 value) : Tag(name), m_Value(value) { }
    TagType GetType() const;

    s64 GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagFloat : public Tag {
private:
    float m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagFloat() : Tag(L""), m_Value(0.0f) { }
    TagFloat(std::wstring name, float value) : Tag(name), m_Value(value) { }
    TagType GetType() const;

    float GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};

class TagDouble : public Tag {
private:
    double m_Value;

    void Write(DataBuffer& buffer) const;
    void Read(DataBuffer& buffer);

public:
    TagDouble() : Tag(L""), m_Value(0.0) { }
    TagDouble(std::wstring name, double value) : Tag(name), m_Value(value) { }
    TagType GetType() const;

    double GetValue() const { return m_Value; }
    friend DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
};


class NBT {
private:
    TagCompound m_Root;

public:
    NBT() { }

    TagCompound& GetRoot() { return m_Root; }
    const TagCompound& GetRoot() const { return m_Root; }
    bool HasData() const { return m_Root.GetList().size() != 0; }

    friend DataBuffer& operator>>(DataBuffer& out, NBT& nbt);
};

DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
DataBuffer& operator<<(DataBuffer& out, const TagString& tag);
DataBuffer& operator<<(DataBuffer& out, const TagByteArray& tag);
DataBuffer& operator<<(DataBuffer& out, const TagList& tag);
DataBuffer& operator<<(DataBuffer& out, const TagCompound& tag);
DataBuffer& operator<<(DataBuffer& out, const TagIntArray& tag);
DataBuffer& operator<<(DataBuffer& out, const TagByte& tag);
DataBuffer& operator<<(DataBuffer& out, const TagShort& tag);
DataBuffer& operator<<(DataBuffer& out, const TagInt& tag);
DataBuffer& operator<<(DataBuffer& out, const TagLong& tag);
DataBuffer& operator<<(DataBuffer& out, const TagFloat& tag);
DataBuffer& operator<<(DataBuffer& out, const TagDouble& tag);

DataBuffer& operator>>(DataBuffer& in, NBT& nbt);
DataBuffer& operator>>(DataBuffer& in, Tag& tag);
DataBuffer& operator>>(DataBuffer& in, TagString& tag);
DataBuffer& operator>>(DataBuffer& in, TagByteArray& tag);
DataBuffer& operator>>(DataBuffer& in, TagList& tag);
DataBuffer& operator>>(DataBuffer& in, TagCompound& tag);
DataBuffer& operator>>(DataBuffer& in, TagIntArray& tag);
DataBuffer& operator>>(DataBuffer& in, TagByte& tag);
DataBuffer& operator>>(DataBuffer& in, TagShort& tag);
DataBuffer& operator>>(DataBuffer& in, TagInt& tag);
DataBuffer& operator>>(DataBuffer& in, TagLong& tag);
DataBuffer& operator>>(DataBuffer& in, TagFloat& tag);
DataBuffer& operator>>(DataBuffer& in, TagDouble& tag);

} // ns NBT

} // ns Minecraft

#endif
