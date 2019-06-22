#include <gtest/gtest.h>

#include <mclib/common/DataBuffer.h>
#include <mclib/common/VarInt.h>

#include <limits>

TEST(VarInt, VarIntStoresAndReturnsIntegers) {
    const auto PositiveValue = 34;
    const auto NegativeValue = -42;

    mc::VarInt positive = PositiveValue;
    mc::VarInt negative = NegativeValue;
    mc::VarInt zero = 0;

    EXPECT_EQ(positive.GetInt(), PositiveValue);
    EXPECT_EQ(negative.GetInt(), NegativeValue);
    EXPECT_EQ(zero.GetInt(), 0);
}

TEST(VarInt, VarLongStoresAndReturnsLongs) {
    mc::VarLong positive = std::numeric_limits<s64>::max();
    mc::VarLong negative = std::numeric_limits<s64>::min();

    EXPECT_EQ(positive.GetLong(), std::numeric_limits<s64>::max());
    EXPECT_EQ(negative.GetLong(), std::numeric_limits<s64>::min());
}

TEST(VarInt, VarIntSerializesAndDeserializesCorrectly) {
    const auto PositiveValue = 34;
    const auto NegativeValue = -42;

    mc::VarInt positive = PositiveValue;
    mc::VarInt negative = NegativeValue;
    mc::VarInt zero = 0;
    mc::DataBuffer buffer;

    {
        buffer << positive;

        mc::VarInt result;
        buffer >> result;

        EXPECT_EQ(result.GetInt(), PositiveValue)
            << "positive integers serialize and deserialize failed";
    }

    {
        buffer << negative;

        mc::VarInt result;
        buffer >> result;

        EXPECT_EQ(result.GetInt(), NegativeValue)
            << "negative integers serialize and deserialize failed";
    }

    {
        buffer << zero;

        mc::VarInt result;
        buffer >> result;

        EXPECT_EQ(result.GetInt(), 0)
            << "zero serializes and deserializes failed";
    }
}
