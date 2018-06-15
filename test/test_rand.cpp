#include "byte_vector.hpp"
#include "rand.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <string>

using namespace keepasspp;

void TestFillRandom(RandomStrength strength, std::size_t count) {
    Rand rand(strength);

    ByteVector bytes(count);
    rand.fill(bytes);
    EXPECT_TRUE(bytes != ByteVector(count));

    std::string str(count, 0);
    rand.fill(str);
    EXPECT_NE(str, std::string(count, 0));
}

void TestGetRandom(RandomStrength strength, std::size_t count) {
    auto result = Rand(strength).get(count);
    EXPECT_EQ(std::size(result), count);
    EXPECT_TRUE(result != ByteVector(count));
}

TEST(TestRand, FillWeakRandom) {
    for (std::size_t i = 1; i <= 32; i++)
        TestFillRandom(RandomStrength::weak, i);
}

TEST(TestRand, FillStrongRandom) {
    for (std::size_t i = 1; i <= 32; i++)
        TestFillRandom(RandomStrength::strong, i);
}

TEST(TestRand, GetWeakRandom) {
    for (std::size_t i = 1; i < 32; i++)
        TestGetRandom(RandomStrength::weak, 64);
}

TEST(TestRand, GetStrongRandom) {
    for (std::size_t i = 1; i < 32; i++)
        TestGetRandom(RandomStrength::strong, 64);
}
