#include "common/common_pch.h"

#include "common/strings/parsing.h"

#include "gtest/gtest.h"

namespace {

TEST(StringsParsing, ParseDurationNumberWithUnitSecondUnitsIntegers) {
  int64_t value;

  EXPECT_TRUE(parse_duration_number_with_unit("12345ns", value));
  EXPECT_EQ(12345ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("12345us", value));
  EXPECT_EQ(12345000ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("12345ms", value));
  EXPECT_EQ(12345000000ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("12345s", value));
  EXPECT_EQ(12345000000000ll, value);
}

TEST(StringsParsing, ParseDurationNumberWithUnitSecondUnitsFloats) {
  int64_t value;

  EXPECT_TRUE(parse_duration_number_with_unit("12345.678ns", value));
  EXPECT_EQ(12345ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("12345.678us", value));
  EXPECT_EQ(12345678ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("12345.678ms", value));
  EXPECT_EQ(12345678000ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("12345.678s", value));
  EXPECT_EQ(12345678000000ll, value);
}

TEST(StringsParsing, ParseDurationNumberWithUnitSecondUnitsFractions) {
  int64_t value;

  EXPECT_TRUE(parse_duration_number_with_unit("2500/50ns", value));
  EXPECT_EQ(50ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("2500/50us", value));
  EXPECT_EQ(50000ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("2500/50ms", value));
  EXPECT_EQ(50000000ll, value);

  EXPECT_TRUE(parse_duration_number_with_unit("2500/50ns", value));
  EXPECT_EQ(50, value);
}

TEST(StringsParsing, ParseDurationNumberWithUnitFrameUnitsIntegers) {
  int64_t value;

  EXPECT_TRUE(parse_duration_number_with_unit("20fps", value));
  EXPECT_EQ(1000000000ll / 20, value);

  EXPECT_TRUE(parse_duration_number_with_unit("20p", value));
  EXPECT_EQ(1000000000ll / 20, value);

  EXPECT_TRUE(parse_duration_number_with_unit("20i", value));
  EXPECT_EQ(1000000000ll / 10, value);
}

TEST(StringsParsing, ParseDurationNumberWithUnitInvalid) {
  int64_t value;

  EXPECT_FALSE(parse_duration_number_with_unit("", value));
  EXPECT_FALSE(parse_duration_number_with_unit("20", value));
  EXPECT_FALSE(parse_duration_number_with_unit("fps", value));
  EXPECT_FALSE(parse_duration_number_with_unit("i", value));
  EXPECT_FALSE(parse_duration_number_with_unit("i20", value));
  EXPECT_FALSE(parse_duration_number_with_unit("20/s", value));
}

}
