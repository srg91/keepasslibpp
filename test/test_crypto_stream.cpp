#include "byte_vector.hpp"
#include "cipher_adapter.hpp"
#include "crypto_stream.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace keepasspp;

TEST(TestCryptoStream, EncryptAes256) {
    std::string input = "CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk";
    std::string key = "jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp";
    ByteVector iv(16);

    std::ostringstream os;
    CryptoStream cs(os, CipherAlgorithm::aes256, CipherMode::ecb, key, iv);
    cs.write(std::data(input), std::size(input));

    ByteVector expected = {
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x37, 0x54, 0x47, 0x34, 0xaa, 0x61, 0x32, 0xa2,
        0x1d, 0x42, 0xac, 0x3c, 0x56, 0xfb, 0x70, 0xe1
    };
    EXPECT_EQ(to_byte_vector(os.str()), expected);
}

TEST(TestCryptoStream, DecryptAes256) {
    std::string key = "jJ4AOXiyVa1pTMkdMXLOwxloIFtygxkp";
    ByteVector iv(16);

    auto input = to_string({
        0x2d, 0xbe, 0x5c, 0xe0, 0x62, 0x32, 0x5c, 0xab,
        0xc9, 0xd8, 0xe1, 0xa6, 0xed, 0xe1, 0x27, 0x08,
        0x37, 0x54, 0x47, 0x34, 0xaa, 0x61, 0x32, 0xa2,
        0x1d, 0x42, 0xac, 0x3c, 0x56, 0xfb, 0x70, 0xe1
    });
    std::istringstream is(input);
    std::string expected = "CLptVAZ5iIpYRWEhJyZ4ublZQcHj91mk";

    CryptoStream cs(is, CipherAlgorithm::aes256, CipherMode::ecb, key, iv);

    std::string output(std::size(expected), 0);
    cs.read(std::data(output), std::size(output));

    EXPECT_EQ(output, expected);
}
