#include "byte_vector.hpp"
#include "cipher_adapter.hpp"
#include "exception.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <iterator>
#include <sstream>

using namespace keepasspp;

TEST(TestCipherAdapter, GetBlockLength) {
    auto result = CipherAdapter::getBlockLength(CipherAlgorithm::aes256);
    EXPECT_EQ(result, 16);
}

TEST(TestCipherAdapter, GetKeyLength) {
    auto result = CipherAdapter::getKeyLength(CipherAlgorithm::aes256);
    EXPECT_EQ(result, 32);
}

TEST(TestCipherAdapter, EncryptAes256) {
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");
    std::string input = "CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk";
    ByteVector expected = {
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x37, 0x54, 0x47, 0x34, 0xaa, 0x61, 0x32, 0xa2,
        0x1d, 0x42, 0xac, 0x3c, 0x56, 0xfb, 0x70, 0xe1
    };

    CipherAdapter aes(CipherAlgorithm::aes256, CipherMode::ecb);
    aes.setKey(key);

    ByteVector output(std::size(input));
    aes.encrypt(input, output);
    EXPECT_EQ(output, expected);

    output = to_byte_vector(input);
    EXPECT_NE(output, expected);
    aes.encrypt(output);
    EXPECT_EQ(output, expected);
}


TEST(TestCipherAdapter, DecryptAes256) {
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");
    ByteVector input = {
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x37, 0x54, 0x47, 0x34, 0xaa, 0x61, 0x32, 0xa2,
        0x1d, 0x42, 0xac, 0x3c, 0x56, 0xfb, 0x70, 0xe1
    };
    auto expected_string = "CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk";
    auto expected = to_byte_vector(expected_string);

    CipherAdapter aes(CipherAlgorithm::aes256, CipherMode::ecb);
    aes.setKey(key);

    ByteVector output(std::size(input));
    aes.decrypt(input, output);
    EXPECT_EQ(output, expected);

    output = input;
    EXPECT_NE(output, expected);
    aes.decrypt(output);
    EXPECT_EQ(output, expected);

    auto string_output = std::string(std::size(expected), 0);
    aes.decrypt(input, string_output);
    EXPECT_EQ(string_output, expected_string);
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
        aes.encrypt(input, output),
        exception::CipherInternalError
    );

    aes.setKey(key);
    EXPECT_NO_THROW(aes.encrypt(input, output));
    EXPECT_EQ(output, expected);
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

    aes.encrypt(input, output);
    EXPECT_EQ(output, expected);

    expected = {
        0xe8, 0x03, 0x78, 0xdf, 0x76, 0xa2, 0x13, 0xd3,
        0xc9, 0x54, 0xd3, 0x2d, 0x2a, 0xb0, 0xa5, 0x7f,
        0xd2, 0x79, 0xc7, 0x03, 0x46, 0x5c, 0x92, 0xea,
        0xe1, 0x14, 0x78, 0x7e, 0xb0, 0xe9, 0x25, 0x45
    };

    aes.setIv(iv);
    aes.encrypt(input, output);
    EXPECT_EQ(output, expected);
}

TEST(TestCipherAdapter, InvalidInput) {
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");
    auto input = to_byte_vector("Hello, world!");

    CipherAdapter aes(CipherAlgorithm::aes256, CipherMode::ecb);
    aes.setKey(key);

    EXPECT_THROW(aes.encrypt(input), exception::CipherInternalError);
}

TEST(TestCipherAdapter, UnknownAlgorithmOrMode) {
    EXPECT_THROW(
        CipherAdapter aes(static_cast<CipherAlgorithm>(-1), CipherMode::ecb),
        exception::CipherInternalError
    );

    EXPECT_THROW(
        CipherAdapter aes(CipherAlgorithm::aes256, static_cast<CipherMode>(-1)),
        exception::CipherInternalError
    );
}

TEST(TestCipherAdapter, EncryptChaCha20) {
    auto iv = to_byte_vector(std::string(12, 0x11));
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");

    auto input = to_byte_vector("Hello, world");
    ByteVector expected = {
        0xb1, 0x2a, 0xcf, 0x3e, 0xeb, 0xf5,
        0xfd, 0x97, 0xf7, 0xe5, 0xa7, 0x7f
    };

    CipherAdapter chacha20(CipherAlgorithm::chacha20, CipherMode::stream);
    chacha20.setKey(key);
    chacha20.setIv(iv);

    ByteVector output(std::size(input));
    chacha20.encrypt(input, output);
    EXPECT_EQ(output, expected);
}

TEST(TestCipherAdapter, DecryptChaCha20) {
    auto iv = to_byte_vector(std::string(12, 0x11));
    auto key = to_byte_vector("jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp");

    ByteVector input = {
        0xb1, 0x2a, 0xcf, 0x3e, 0xeb, 0xf5,
        0xfd, 0x97, 0xf7, 0xe5, 0xa7, 0x7f
    };
    std::string expected = "Hello, world";

    CipherAdapter chacha20(CipherAlgorithm::chacha20, CipherMode::stream);
    chacha20.setKey(key);
    chacha20.setIv(iv);

    std::string output(std::size(expected), 0);
    chacha20.decrypt(input, output);
    EXPECT_EQ(output, expected);
}
