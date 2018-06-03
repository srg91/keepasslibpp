#include "crypto_util.hpp"

#include <openssl/rand.h>
#include <openssl/sha.h>

#include <cstdint>


using namespace keepasslibpp;

const std::size_t CryptoUtil::Sha256DigestLength = SHA256_DIGEST_LENGTH;

    // TODO: Add exceptions
types::bytes CryptoUtil::HashSha256(const types::bytes& data) {
    types::bytes result_data(Sha256DigestLength);

    // TODO: handle errors
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, &data[0], data.size());
    SHA256_Final(&result_data[0], &ctx);

    return result_data;
}

// TODO: Add exceptions
// TODO: Rename?
types::bytes CryptoUtil::GetRandomBytes(std::size_t count) {
    types::bytes result_data(count);
    // TODO: handle errors
    RAND_bytes(&result_data[0], static_cast<int>(count));
    return result_data;
}
