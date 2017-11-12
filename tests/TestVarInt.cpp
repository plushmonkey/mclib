#include "catch.hpp"

#include <mclib/common/VarInt.h>
#include <mclib/common/DataBuffer.h>

#include <limits>

TEST_CASE("VarInt stores and returns integers", "[VarInt]") {
    const auto PositiveValue = 34;
    const auto NegativeValue = -42;

    mc::VarInt positive = PositiveValue;
    mc::VarInt negative = NegativeValue;
    mc::VarInt zero = 0;

    REQUIRE(positive.GetInt() == PositiveValue);
    REQUIRE(negative.GetInt() == NegativeValue);
    REQUIRE(zero.GetInt() == 0);
}

TEST_CASE("VarLong stores and returns longs", "[VarInt]") {
    mc::VarLong positive = std::numeric_limits<s64>::max();
    mc::VarLong negative = std::numeric_limits<s64>::min();

    REQUIRE(positive.GetLong() == std::numeric_limits<s64>::max());
    REQUIRE(negative.GetLong() == std::numeric_limits<s64>::min());
}

TEST_CASE("VarInt serializes and deserializes correctly", "[VarInt]") {
    const auto PositiveValue = 34;
    const auto NegativeValue = -42;

    mc::VarInt positive = PositiveValue;
    mc::VarInt negative = NegativeValue;
    mc::VarInt zero = 0;
    mc::DataBuffer buffer;

    SECTION("positive integers serialize and deserialize") {
        buffer << positive;

        mc::VarInt result;
        buffer >> result;

        REQUIRE(result.GetInt() == PositiveValue);
    }

    SECTION("negative integers serialize and deserialize") {
        buffer << negative;

        mc::VarInt result;
        buffer >> result;

        REQUIRE(result.GetInt() == NegativeValue);
    }

    SECTION("zero serializes and deserializes") {
        buffer << zero;

        mc::VarInt result;
        buffer >> result;

        REQUIRE(result.GetInt() == 0);
    }
}
