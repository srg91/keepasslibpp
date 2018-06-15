#include "byte_vector.hpp"
#include "kdf_engine.hpp"
#include "kdf_engine_aes.hpp"
#include "uuid.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace keepasspp;

TEST(TestKdfEngineAes, GetUuid) {
    Uuid expected = Uuid::fromByteVector({
        0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
        0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea
    });

    std::unique_ptr<KdfEngine> aes = std::make_unique<KdfEngineAes>();
    auto result = aes->getUuid();

    EXPECT_EQ(result, expected);
}

TEST(TestKdfEngineAes, GetDefaultParameters) {

    KdfEngineAes aes;
    auto params = aes.getDefaultParameters();
    EXPECT_EQ(std::size(params), 2);

    std::uint64_t rounds = 0;
    EXPECT_TRUE(params.get(KdfEngineAes::paramRounds, rounds));
    std::uint64_t expected_rounds = 6000;
    EXPECT_EQ(rounds, expected_rounds);

    ByteVector uuid;
    EXPECT_TRUE(params.get("$UUID", uuid));
    auto expected_uuid = aes.getUuid().byteVector();
    EXPECT_TRUE(uuid == expected_uuid);
}

TEST(TestKdfEngineAes, Randomize) {
    KdfEngineAes aes;

    auto params = aes.getDefaultParameters();
    aes.randomize(params);
    EXPECT_EQ(std::size(params), 3);

    ByteVector seed;
    EXPECT_TRUE(params.get(KdfEngineAes::paramSeed, seed));
    ByteVector not_expected(32);
    EXPECT_EQ(std::size(seed), std::size(not_expected));
    EXPECT_TRUE(seed != not_expected);
}

TEST(TestKdfEngineAes, Transform) {
    auto message = to_byte_vector("message");

    KdfEngineAes aes;
    auto params = aes.getDefaultParameters();
    params[KdfEngineAes::paramSeed] = to_byte_vector("some seed");

    params[KdfEngineAes::paramRounds] = std::uint64_t(1);
    auto result = aes.transform(message, params);
    ByteVector expected_one_round = {
        0x33, 0x87, 0x94, 0xfb, 0x98, 0xa1, 0x4d, 0xbe, 0xa0, 0x4d,
        0xad, 0x14, 0xf3, 0xab, 0x03, 0xe5, 0x65, 0x4e, 0x86, 0xb5,
        0xb5, 0x2d, 0xe3, 0x5d, 0x88, 0x6c, 0x2a, 0xca, 0xd2, 0x3e,
        0xa1, 0x9a
    };
    EXPECT_TRUE(result == expected_one_round);

    params[KdfEngineAes::paramRounds] = std::uint64_t(6000);
    result = aes.transform(message, params);
    ByteVector expected_several_rounds = {
        0xe3, 0xa8, 0x90, 0x77, 0x8b, 0xbc, 0xb1, 0x13, 0x0b, 0x93,
        0xb2, 0xf3, 0xbe, 0x9f, 0xcd, 0x2a, 0x9a, 0x56, 0xb6, 0x6a,
        0xaf, 0x22, 0x14, 0x56, 0xa6, 0xd2, 0xcb, 0x55, 0x44, 0x88,
        0xca, 0x99
    };
    EXPECT_TRUE(result == expected_several_rounds);
}
