#include <byte_vector.hpp>
#include <hash.hpp>

#include <gtest/gtest.h>

#include <iterator>

using namespace keepasspp;

const auto SHA256_SIZE = Hash::getAlgorithmLength(HashAlgorithm::sha256);

TEST(TestHash, GetAlgorithmLength) {
    auto sha256Algorithm = HashAlgorithm::sha256;
    EXPECT_EQ(Hash::getAlgorithmLength(sha256Algorithm), 32u);
}

TEST(TestHash, SumFromString) {
    std::string input = "Hello, world!";
    ByteVector expected = {
        0x31, 0x5f, 0x5b, 0xdb, 0x76, 0xd0, 0x78, 0xc4, 0x3b, 0x8a, 0xc0, 0x06,
        0x4e, 0x4a, 0x01, 0x64, 0x61, 0x2b, 0x1f, 0xce, 0x77, 0xc8, 0x69, 0x34,
        0x5b, 0xfc, 0x94, 0xc7, 0x58, 0x94, 0xed, 0xd3
    };
    EXPECT_EQ(std::size(expected), SHA256_SIZE);

    auto result = Hash(HashAlgorithm::sha256).sum(input);
    EXPECT_EQ(result, expected);
}

TEST(TestHash, SumWithWrite) {
    std::string input = "Hello, world!";
    ByteVector expected = {
        0x31, 0x5f, 0x5b, 0xdb, 0x76, 0xd0, 0x78, 0xc4, 0x3b, 0x8a, 0xc0, 0x06,
        0x4e, 0x4a, 0x01, 0x64, 0x61, 0x2b, 0x1f, 0xce, 0x77, 0xc8, 0x69, 0x34,
        0x5b, 0xfc, 0x94, 0xc7, 0x58, 0x94, 0xed, 0xd3
    };
    EXPECT_EQ(std::size(expected), SHA256_SIZE);

    auto sha256 = Hash(HashAlgorithm::sha256);
    auto result = sha256.write(input).sum();
    EXPECT_EQ(result, expected);
}

TEST(TestHash, HexSumFromString) {
    std::string input = "Hello, world!";
    std::string expected = "315f5bdb76d078c43b8ac0064e4a0164"
                           "612b1fce77c869345bfc94c75894edd3";

    auto sha256 = Hash(HashAlgorithm::sha256);
    auto result = sha256.hexSum(input);
    EXPECT_EQ(result, expected);
}

TEST(TestHash, HexSumWithWrite) {
    std::string input = "Hello, world!";
    std::string expected = "315f5bdb76d078c43b8ac0064e4a0164"
                           "612b1fce77c869345bfc94c75894edd3";

    auto sha256 = Hash(HashAlgorithm::sha256);
    auto result = sha256.write(input).hexSum();
    EXPECT_EQ(result, expected);
}

TEST(TestHash, HexSum256) {
    std::string input = "Hello, world!";
    std::string expected = "315f5bdb76d078c43b8ac0064e4a0164"
                           "612b1fce77c869345bfc94c75894edd3";

    auto sha256 = Hash(HashAlgorithm::sha256);
    auto output = sha256.hexSum(input);
    EXPECT_EQ(output, expected);
}
