#include "byte_vector.hpp"
#include "cipher_adapter.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <iterator>

using namespace keepasspp;

TEST(TestCipherAdapter, GetBlockLength) {
    auto result = CipherAdapter::getBlockLength(CipherAlgorithm::aes256);
    EXPECT_EQ(result, 16);
}

TEST(TestCipherAdapter, GetKeyLength) {
    auto result = CipherAdapter::getKeyLength(CipherAlgorithm::aes256);
    EXPECT_EQ(result, 32);
}

TEST(TestCipherAdapter, Encrypt) {
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");
    auto input = to_byte_vector("CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk");
    ByteVector expected = {
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x37, 0x54, 0x47, 0x34, 0xaa, 0x61, 0x32, 0xa2,
        0x1d, 0x42, 0xac, 0x3c, 0x56, 0xfb, 0x70, 0xe1
    };

    CipherAdapter aes(CipherAlgorithm::aes256, CipherMode::ecb);
    aes.setKey(key);

    ByteVector output(32);
    aes.encrypt(std::begin(input), std::begin(output), std::size(input));

    EXPECT_TRUE(output == expected);
}

TEST(TestCipherAdapter, Decrypt) {
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");
    ByteVector input = {
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x37, 0x54, 0x47, 0x34, 0xaa, 0x61, 0x32, 0xa2,
        0x1d, 0x42, 0xac, 0x3c, 0x56, 0xfb, 0x70, 0xe1
    };
    auto expected = to_byte_vector("CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk");

    CipherAdapter aes(CipherAlgorithm::aes256, CipherMode::ecb);
    aes.setKey(key);

    ByteVector output(32);
    aes.decrypt(std::begin(input), std::begin(output), std::size(input));

    EXPECT_TRUE(output == expected);
}

// TODO: add tests for setIv, setKey
// TODO: add tests for getMapped?
// TODO: add tests for handle errors
