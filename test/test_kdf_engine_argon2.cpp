#include "kdf_engine_argon.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace keepasspp;

TEST(TestKdfEngineArgon2, GetUuid) {
    Uuid expected = Uuid::fromByteVector({
        0xef, 0x63, 0x6d, 0xdf, 0x8c, 0x29, 0x44, 0x4b,
        0x91, 0xf7, 0xa9, 0xa4, 0x03, 0xe3, 0x0a, 0x0c
    });

    std::unique_ptr<KdfEngine> aes = std::make_unique<KdfEngineArgon2>();
    auto result = aes->getUuid();

    EXPECT_EQ(result, expected);
}

TEST(TestKdfEngineArgon2, GetDefaultParameters) {
    KdfEngineArgon2 argon;
    auto params = argon.getDefaultParameters();
    EXPECT_EQ(std::size(params), 5);

    auto version =
        params.get<std::uint32_t>(KdfEngineArgon2::paramVersion);
    EXPECT_EQ(version, 0x13);

    auto iterations =
        params.get<std::uint64_t>(KdfEngineArgon2::paramIterations);
    EXPECT_EQ(iterations, 2);

    auto memory =
        params.get<std::uint64_t>(KdfEngineArgon2::paramMemory);
    EXPECT_EQ(memory, 1 * 1024);

    auto parallelism =
        params.get<std::uint32_t>(KdfEngineArgon2::paramParallelism);
    EXPECT_EQ(parallelism, 2);
}

TEST(TestKdfEngineArgon2, Randomize) {
    KdfEngineArgon2 argon;

    auto params = argon.getDefaultParameters();
    argon.randomize(params);

    auto salt = params.get<ByteVector>(KdfEngineArgon2::paramSalt);
    ByteVector not_expected(32);
    EXPECT_EQ(std::size(salt), std::size(not_expected));
    EXPECT_NE(salt, not_expected);
}

TEST(TestKdfEngineArgon2, Transform) {
    auto message = to_byte_vector("message");

    KdfEngineArgon2 argon;
    auto params = argon.getDefaultParameters();
    params[KdfEngineArgon2::paramSalt] = to_byte_vector("some salt");

    ByteVector expected = {
        0x78, 0xbc, 0x19, 0x0c, 0x3d, 0x91, 0x07, 0xe8,
        0x98, 0x10, 0x30, 0x74, 0x5f, 0xf1, 0x72, 0xe9,
        0x89, 0xc6, 0x6b, 0x4a, 0x0a, 0xb4, 0xcc, 0xbd,
        0xe9, 0x08, 0x18, 0x9b, 0xa1, 0xa6, 0xc5, 0x15
    };
    auto result = argon.transform(message, params);
    EXPECT_EQ(result, expected);
}