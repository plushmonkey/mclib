#include <mclib/nbt/Tag.h>

#include <mclib/common/DataBuffer.h>

#include <utf8.h>

namespace mc {
namespace nbt {

std::wstring Tag::GetName() const {
    return m_Name;
}

DataBuffer& operator<<(DataBuffer& out, const Tag& tag) {
    u8 type = (u8)tag.GetType();
    out << type;

    std::wstring name = tag.GetName();
    TagString stringTag(L"", name);
    stringTag.Write(out);
    tag.Write(out);
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagString& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagByte& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagShort& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagInt& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagLong& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagFloat& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagDouble& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagList& tag) {
    tag.Write(out);
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagCompound& tag) {
    tag.Write(out);
    return out;
}


TagType TagString::GetType() const {
    return TagType::String;
}

void TagString::Write(DataBuffer& buffer) const {
    std::string utf8;
    utf8::utf16to8(m_Value.begin(), m_Value.end(), std::back_inserter(utf8));

    u16 length = utf8.length();
    buffer << length;
    buffer << utf8;
}

void TagString::Read(DataBuffer& buffer) {
    u16 length;

    buffer >> length;

    m_Value.clear();

    if (length > 0) {
        std::string utf8;
        buffer.ReadSome(utf8, length);

        m_Value.clear();
        utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(m_Value));
    }
}


TagType TagByteArray::GetType() const {
    return TagType::ByteArray;
}

void TagByteArray::Write(DataBuffer& buffer) const {
    s32 length = m_Value.length();

    buffer << length;
    buffer << m_Value;
}

void TagByteArray::Read(DataBuffer& buffer) {
    s32 length;

    buffer >> length;

    m_Value.clear();
    buffer.ReadSome(m_Value, length);
}

TagType TagIntArray::GetType() const {
    return TagType::IntArray;
}

void TagIntArray::Write(DataBuffer& buffer) const {
    s32 length = m_Value.size();

    buffer << length;
    for (s32 val : m_Value)
        buffer << val;
}

void TagIntArray::Read(DataBuffer& buffer) {
    s32 length;

    buffer >> length;

    m_Value.clear();

    for (s32 i = 0; i < length; ++i) {
        s32 val;
        buffer >> val;
        m_Value.push_back(val);
    }
}

void TagList::Write(DataBuffer& buffer) const {
    u8 type = (u8)m_ListType;
    s32 size = m_Tags.size();

    buffer << type;
    buffer << size;

    for (TagPtr tag : m_Tags)
        tag->Write(buffer);
}

void TagList::Read(DataBuffer& buffer) {
    u8 type;
    s32 size;

    buffer >> type;
    buffer >> size;

    for (s32 i = 0; i < size; ++i) {
        TagPtr tag;

        if (type == 1)
            tag = std::make_shared<TagByte>();
        else if (type == 2)
            tag = std::make_shared<TagShort>();
        else if (type == 3)
            tag = std::make_shared<TagInt>();
        else if (type == 4)
            tag = std::make_shared<TagLong>();
        else if (type == 5)
            tag = std::make_shared<TagFloat>();
        else if (type == 6)
            tag = std::make_shared<TagDouble>();
        else if (type == 7)
            tag = std::make_shared<TagByteArray>();
        else if (type == 8)
            tag = std::make_shared<TagString>();
        else if (type == 9)
            tag = std::make_shared<TagList>();
        else if (type == 10)
            tag = std::make_shared<TagCompound>();
        else if (type == 11)
            tag = std::make_shared<TagIntArray>();

        if (tag) {
            tag->Read(buffer);
            m_Tags.push_back(tag);
        } else {
            throw std::runtime_error("Error with TagList::Read");
        }
    }
}

TagType TagList::GetType() const {
    return TagType::List;
}

void TagList::AddItem(TagPtr item) {
    m_Tags.push_back(item);
}

TagList::~TagList() {

}

void TagCompound::Write(DataBuffer& buffer) const {
    for (TagPtr tag : m_Tags)
        buffer << *tag;

    buffer << (u8)0;
}

void TagCompound::Read(DataBuffer& buffer) {
    while (true) {
        u8 type;

        buffer >> type;

        if (type == 0) break;

        TagString name;

        ((Tag&)name).Read(buffer);

        TagPtr tag;

        if (type == 1)
            tag = std::make_shared<TagByte>();
        else if (type == 2)
            tag = std::make_shared<TagShort>();
        else if (type == 3)
            tag = std::make_shared<TagInt>();
        else if (type == 4)
            tag = std::make_shared<TagLong>();
        else if (type == 5)
            tag = std::make_shared<TagFloat>();
        else if (type == 6)
            tag = std::make_shared<TagDouble>();
        else if (type == 7)
            tag = std::make_shared<TagByteArray>();
        else if (type == 8)
            tag = std::make_shared<TagString>();
        else if (type == 9)
            tag = std::make_shared<TagList>();
        else if (type == 10)
            tag = std::make_shared<TagCompound>();
        else if (type == 11)
            tag = std::make_shared<TagIntArray>();

        if (tag) {
            m_Tags.push_back(tag);
            tag->Read(buffer);
            tag->SetName(name.GetValue());
        } else {
            throw std::runtime_error("Error with TagCompound::Read");
        }
    }
}

TagType TagCompound::GetType() const {
    return TagType::Compound;
}

void TagCompound::AddItem(TagPtr item) {
    m_Tags.push_back(item);
}

TagCompound::~TagCompound() {

}

TagType TagByte::GetType() const {
    return TagType::Byte;
}

void TagByte::Write(DataBuffer& buffer) const {
    buffer << m_Value;
}

void TagByte::Read(DataBuffer& buffer) {
    buffer >> m_Value;
}

TagType TagShort::GetType() const {
    return TagType::Short;
}

void TagShort::Write(DataBuffer& buffer) const {
    buffer << m_Value;
}

void TagShort::Read(DataBuffer& buffer) {
    buffer >> m_Value;
}

TagType TagInt::GetType() const {
    return TagType::Int;
}

void TagInt::Write(DataBuffer& buffer) const {
    buffer << m_Value;
}

void TagInt::Read(DataBuffer& buffer) {
    buffer >> m_Value;
}

TagType TagLong::GetType() const {
    return TagType::Long;
}

void TagLong::Write(DataBuffer& buffer) const {
    buffer << m_Value;
}

void TagLong::Read(DataBuffer& buffer) {
    buffer >> m_Value;
}

TagType TagFloat::GetType() const {
    return TagType::Float;
}

void TagFloat::Write(DataBuffer& buffer) const {
    buffer << m_Value;
}

void TagFloat::Read(DataBuffer& buffer) {
    buffer >> m_Value;
}

TagType TagDouble::GetType() const {
    return TagType::Double;
}

void TagDouble::Write(DataBuffer& buffer) const {
    buffer << m_Value;
}

void TagDouble::Read(DataBuffer& buffer) {
    buffer >> m_Value;
}


DataBuffer& operator>>(DataBuffer& in, Tag& tag) {
    u8 type;
    in >> type;

    TagString name;

    ((Tag&)name).Read(in);

    tag.SetName(name.GetValue());
    tag.Read(in);
    return in;
}

DataBuffer& operator>>(DataBuffer& in, TagString& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagByteArray& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagList& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagCompound& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagIntArray& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagByte& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagShort& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagInt& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagLong& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagFloat& tag) {
    in >> (Tag&)tag;
    return in;
}
DataBuffer& operator>>(DataBuffer& in, TagDouble& tag) {
    in >> (Tag&)tag;
    return in;
}

} // ns nbt
} // ns mc
