#include "byte_vector.hpp"
#include "rand.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <string>

using namespace keepasslibpp;

TEST(TestRand, FillRandom) {
    const std::size_t COUNT = 32;

    ByteVector bytes(COUNT);

    Rand::fillRandom(bytes.data(), COUNT);
    EXPECT_NE(bytes, ByteVector(COUNT));

    std::string str(COUNT, 0);
    Rand::fillRandom(str.data(), COUNT);
    EXPECT_NE(str, std::string(COUNT, 0));

    std::int64_t value = 0;
    Rand::fillRandom(reinterpret_cast<void *>(&value), 8);
    EXPECT_NE(value, 0);
}

TEST(TestRand, FillStrongRandom) {
    const std::size_t COUNT = 32;

    ByteVector bytes(COUNT);

    Rand::fillStrongRandom(bytes.data(), COUNT);
    EXPECT_NE(bytes, ByteVector(COUNT));

    std::string str(COUNT, 0);
    Rand::fillStrongRandom(str.data(), COUNT);
    EXPECT_NE(str, std::string(COUNT, 0));

    std::int64_t value = 0;
    Rand::fillStrongRandom(reinterpret_cast<void *>(&value), 8);
    EXPECT_NE(value, 0);
}

TEST(TestRand, GetRandomBytes) {
    ByteVector result = Rand::getRandomBytes(64);

    EXPECT_EQ(result.size(), 64);
    EXPECT_NE(result, ByteVector(64));
}
