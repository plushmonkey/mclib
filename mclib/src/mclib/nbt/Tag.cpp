#include <mclib/nbt/Tag.h>

#include <mclib/common/DataBuffer.h>
#include <mclib/common/MCString.h>
#include <array>

namespace mc {
namespace nbt {

std::string to_string(mc::nbt::TagType type) {
    static const std::array<std::string, 12> typeStr = {
        "End",    "Byte",      "Short",  "Int",  "Long",     "Float",
        "Double", "ByteArray", "String", "List", "Compound", "IntArray"};

    return typeStr[static_cast<int>(type)];
}

std::ostream& operator<<(std::ostream& os, TagType type) {
    os << to_string(type);
    return os;
}

std::wstring Tag::GetName() const noexcept { return m_Name; }

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

DataBuffer& operator<<(DataBuffer& out, const TagByteArray& tag) {
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
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagCompound& tag) {
    out << (Tag&)tag;
    return out;
}

DataBuffer& operator<<(DataBuffer& out, const TagIntArray& tag) {
    out << (Tag&)tag;
    return out;
}

TagType TagString::GetType() const noexcept { return TagType::String; }

void TagString::Write(DataBuffer& buffer) const {
    std::string utf8 = utf16to8(m_Value);

    u16 length = (u16)utf8.length();
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

        m_Value = utf8to16(utf8);
    }
}

TagType TagByteArray::GetType() const noexcept { return TagType::ByteArray; }

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

TagType TagIntArray::GetType() const noexcept { return TagType::IntArray; }

void TagIntArray::Write(DataBuffer& buffer) const {
    s32 length = m_Value.size();

    buffer << length;
    for (s32 val : m_Value) buffer << val;
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
    s32 size = (m_ListType != TagType::End) ? m_Tags.size() : 0;

    buffer << type;
    buffer << size;

    if (m_ListType == TagType::End) return;

    for (TagPtr tag : m_Tags) tag->Write(buffer);
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

TagType TagList::GetType() const noexcept { return TagType::List; }

void TagList::AddItem(TagPtr item) {
    if (item->GetType() != m_ListType) {
        std::string message = "Tried to add " + to_string(item->GetType()) +
                              " to list containing " + to_string(m_ListType) +
                              ".";
        throw std::invalid_argument(message.c_str());
    }
    m_Tags.push_back(item);
}

TagList::~TagList() {}

void TagList::CopyOther(const TagList& rhs) {
    m_Tags = std::vector<TagPtr>(rhs.m_Tags.size());

    for (std::size_t i = 0; i < m_Tags.size(); ++i) {
        auto tag = rhs.m_Tags[i];

        if (m_ListType == TagType::Byte) {
            m_Tags[i] = std::make_shared<TagByte>(*(TagByte*)tag.get());
        } else if (m_ListType == TagType::Short) {
            m_Tags[i] = std::make_shared<TagShort>(*(TagShort*)tag.get());
        } else if (m_ListType == TagType::Int) {
            m_Tags[i] = std::make_shared<TagInt>(*(TagInt*)tag.get());
        } else if (m_ListType == TagType::Long) {
            m_Tags[i] = std::make_shared<TagLong>(*(TagLong*)tag.get());
        } else if (m_ListType == TagType::Float) {
            m_Tags[i] = std::make_shared<TagFloat>(*(TagFloat*)tag.get());
        } else if (m_ListType == TagType::Double) {
            m_Tags[i] = std::make_shared<TagDouble>(*(TagDouble*)tag.get());
        } else if (m_ListType == TagType::ByteArray) {
            m_Tags[i] =
                std::make_shared<TagByteArray>(*(TagByteArray*)tag.get());
        } else if (m_ListType == TagType::String) {
            m_Tags[i] = std::make_shared<TagString>(*(TagString*)tag.get());
        } else if (m_ListType == TagType::List) {
            m_Tags[i] = std::make_shared<TagList>(*(TagList*)tag.get());
        } else if (m_ListType == TagType::Compound) {
            m_Tags[i] = std::make_shared<TagCompound>(*(TagCompound*)tag.get());
        } else if (m_ListType == TagType::IntArray) {
            m_Tags[i] = std::make_shared<TagIntArray>(*(TagIntArray*)tag.get());
        }
    }
}

TagList::TagList(const TagList& rhs)
    : Tag(rhs.GetName()), m_ListType(rhs.GetListType()) {
    CopyOther(rhs);
}

TagList& TagList::operator=(const TagList& rhs) {
    SetName(rhs.GetName());
    m_ListType = rhs.GetListType();
    CopyOther(rhs);
    return *this;
}

void TagCompound::Write(DataBuffer& buffer) const {
    for (auto pair : m_Tags) {
        buffer << *pair.second;
    }

    buffer << (u8)0;
}

void TagCompound::Read(DataBuffer& buffer) {
    while (true) {
        u8 typeValue;

        buffer >> typeValue;

        TagType type = (TagType)typeValue;
        if (type == TagType::End) break;

        TagString name;

        ((Tag&)name).Read(buffer);

        TagPtr tag;

        if (type == TagType::Byte)
            tag = std::make_shared<TagByte>();
        else if (type == TagType::Short)
            tag = std::make_shared<TagShort>();
        else if (type == TagType::Int)
            tag = std::make_shared<TagInt>();
        else if (type == TagType::Long)
            tag = std::make_shared<TagLong>();
        else if (type == TagType::Float)
            tag = std::make_shared<TagFloat>();
        else if (type == TagType::Double)
            tag = std::make_shared<TagDouble>();
        else if (type == TagType::ByteArray)
            tag = std::make_shared<TagByteArray>();
        else if (type == TagType::String)
            tag = std::make_shared<TagString>();
        else if (type == TagType::List)
            tag = std::make_shared<TagList>();
        else if (type == TagType::Compound)
            tag = std::make_shared<TagCompound>();
        else if (type == TagType::IntArray)
            tag = std::make_shared<TagIntArray>();

        if (tag) {
            m_Tags.push_back(std::make_pair(type, tag));
            tag->Read(buffer);
            tag->SetName(name.GetValue());
        } else {
            throw std::runtime_error("Error with TagCompound::Read");
        }
    }
}

TagType TagCompound::GetType() const noexcept { return TagType::Compound; }

void TagCompound::AddItem(TagType type, TagPtr item) {
    m_Tags.push_back(std::make_pair(type, item));
}

TagCompound::~TagCompound() {}

void TagCompound::CopyOther(const TagCompound& rhs) {
    m_Tags = std::vector<DataType>(rhs.m_Tags.size());

    for (std::size_t i = 0; i < m_Tags.size(); ++i) {
        auto pair = rhs.m_Tags[i];
        auto tag = pair.second;
        auto type = pair.first;

        std::shared_ptr<Tag> newTag;

        if (type == TagType::Byte) {
            newTag = std::make_shared<TagByte>(*(TagByte*)tag.get());
        } else if (type == TagType::Short) {
            newTag = std::make_shared<TagShort>(*(TagShort*)tag.get());
        } else if (type == TagType::Int) {
            newTag = std::make_shared<TagInt>(*(TagInt*)tag.get());
        } else if (type == TagType::Long) {
            newTag = std::make_shared<TagLong>(*(TagLong*)tag.get());
        } else if (type == TagType::Float) {
            newTag = std::make_shared<TagFloat>(*(TagFloat*)tag.get());
        } else if (type == TagType::Double) {
            newTag = std::make_shared<TagDouble>(*(TagDouble*)tag.get());
        } else if (type == TagType::ByteArray) {
            newTag = std::make_shared<TagByteArray>(*(TagByteArray*)tag.get());
        } else if (type == TagType::String) {
            newTag = std::make_shared<TagString>(*(TagString*)tag.get());
        } else if (type == TagType::List) {
            newTag = std::make_shared<TagList>(*(TagList*)tag.get());
        } else if (type == TagType::Compound) {
            newTag = std::make_shared<TagCompound>(*(TagCompound*)tag.get());
        } else if (type == TagType::IntArray) {
            newTag = std::make_shared<TagIntArray>(*(TagIntArray*)tag.get());
        }

        m_Tags[i] = std::make_pair(type, newTag);
    }
}

TagCompound::TagCompound(const TagCompound& rhs) : Tag(rhs.GetName()) {
    CopyOther(rhs);
}

TagCompound& TagCompound::operator=(const TagCompound& rhs) {
    SetName(rhs.GetName());
    CopyOther(rhs);
    return *this;
}

TagType TagByte::GetType() const noexcept { return TagType::Byte; }

void TagByte::Write(DataBuffer& buffer) const { buffer << m_Value; }

void TagByte::Read(DataBuffer& buffer) { buffer >> m_Value; }

TagType TagShort::GetType() const noexcept { return TagType::Short; }

void TagShort::Write(DataBuffer& buffer) const { buffer << m_Value; }

void TagShort::Read(DataBuffer& buffer) { buffer >> m_Value; }

TagType TagInt::GetType() const noexcept { return TagType::Int; }

void TagInt::Write(DataBuffer& buffer) const { buffer << m_Value; }

void TagInt::Read(DataBuffer& buffer) { buffer >> m_Value; }

TagType TagLong::GetType() const noexcept { return TagType::Long; }

void TagLong::Write(DataBuffer& buffer) const { buffer << m_Value; }

void TagLong::Read(DataBuffer& buffer) { buffer >> m_Value; }

TagType TagFloat::GetType() const noexcept { return TagType::Float; }

void TagFloat::Write(DataBuffer& buffer) const { buffer << m_Value; }

void TagFloat::Read(DataBuffer& buffer) { buffer >> m_Value; }

TagType TagDouble::GetType() const noexcept { return TagType::Double; }

void TagDouble::Write(DataBuffer& buffer) const { buffer << m_Value; }

void TagDouble::Read(DataBuffer& buffer) { buffer >> m_Value; }

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

}  // namespace nbt
}  // namespace mc
