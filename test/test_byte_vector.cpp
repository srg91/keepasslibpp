#include "byte_vector.hpp"

#include <gtest/gtest.h>

#include <string>

using namespace keepasslibpp;

TEST(TestByteVector, ToByteVector) {
    std::string s = "Hello, world!";
    ByteVector expected = {'H', 'e', 'l', 'l', 'o', ',', ' ',
                           'w', 'o', 'r', 'l', 'd', '!'};

    auto result = to_byte_vector(s);
    EXPECT_TRUE(result == expected);
}

TEST(TestByteVector, ToString) {
    ByteVector bv = {'H', 'e', 'l', 'l', 'o', ',', ' ',
                     'w', 'o', 'r', 'l', 'd', '!'};
    std::string expected = "Hello, world!";

    auto result = to_string(bv);
    EXPECT_EQ(result, expected);
}

TEST(TestByteVector, ToHexString) {
    ByteVector bv = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    std::string expected = "0123456789abcdef";

    auto result = to_hex_string(bv);
    EXPECT_EQ(result, expected);
}
