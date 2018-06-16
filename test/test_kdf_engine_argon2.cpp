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

    auto salt = params.get<std::uint32_t>(KdfEngineArgon2::paramSalt);
}
