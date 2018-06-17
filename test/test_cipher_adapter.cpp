#include "byte_vector.hpp"
#include "cipher_adapter.hpp"
#include "exception.hpp"

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

TEST(TestCipherAdapter, SetKey) {
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");
    auto input = to_byte_vector("CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk");

    ByteVector expected = {
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x37, 0x54, 0x47, 0x34, 0xaa, 0x61, 0x32, 0xa2,
        0x1d, 0x42, 0xac, 0x3c, 0x56, 0xfb, 0x70, 0xe1
    };

    CipherAdapter aes(CipherAlgorithm::aes256, CipherMode::ecb);

    ByteVector output(32);
    // TODO: shows unexpected error in stderr
    EXPECT_THROW(
        aes.encrypt(std::begin(input), std::begin(output), std::size(input)),
        exception::CipherInternalError
    );

    aes.setKey(key);
    EXPECT_NO_THROW(
        aes.encrypt(std::begin(input), std::begin(output), std::size(input))
    );
    EXPECT_TRUE(output == expected);
}

TEST(TestCipherAdapter, SetIv) {
    auto iv = to_byte_vector(std::string(16, 0x10));
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");
    auto input = to_byte_vector("CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk");

    CipherAdapter aes(CipherAlgorithm::aes256, CipherMode::cbc);
    aes.setKey(key);

    ByteVector output(32);
    ByteVector expected = {
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x35, 0x30, 0xcf, 0xde, 0x50, 0x06, 0xef, 0x26,
        0x1c, 0xc6, 0x74, 0x99, 0xf3, 0xc0, 0x40, 0x61
    };

    aes.encrypt(std::begin(input), std::begin(output), std::size(input));
    EXPECT_TRUE(output == expected);

    expected = {
        0xe8, 0x03, 0x78, 0xdf, 0x76, 0xa2, 0x13, 0xd3,
        0xc9, 0x54, 0xd3, 0x2d, 0x2a, 0xb0, 0xa5, 0x7f,
        0xd2, 0x79, 0xc7, 0x03, 0x46, 0x5c, 0x92, 0xea,
        0xe1, 0x14, 0x78, 0x7e, 0xb0, 0xe9, 0x25, 0x45
    };

    aes.setIv(iv);
    aes.encrypt(std::begin(input), std::begin(output), std::size(input));
    EXPECT_TRUE(output == expected);
}

// TODO: add tests for getMapped?
// TODO: add tests for handle errors
