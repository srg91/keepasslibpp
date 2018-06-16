#include "kdf_engine_argon.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace keepasspp;

TEST(TestkdfEngineArgon2, GetUuid) {
    Uuid expected = Uuid::fromByteVector({
        0xef, 0x63, 0x6d, 0xdf, 0x8c, 0x29, 0x44, 0x4b,
        0x91, 0xf7, 0xa9, 0xa4, 0x03, 0xe3, 0x0a, 0x0c
    });

    std::unique_ptr<KdfEngine> aes = std::make_unique<KdfEngineArgon2>();
    auto result = aes->getUuid();

    EXPECT_EQ(result, expected);
}

TEST(TestkdfEngineArgon2, GetDefaultParameters) {
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
    EXPECT_EQ(memory, 1 * 1024 * 1024);

    auto parallelism =
        params.get<std::uint32_t>(KdfEngineArgon2::paramParallelism);
    EXPECT_EQ(parallelism, 2);
}

