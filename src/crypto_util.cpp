#include "byte_vector.hpp"
#include "crypto_util.hpp"

#include <gcrypt.h>

#include <algorithm>
#include <cstdint>
#include <cstring>

namespace keepasslibpp {

const std::size_t CryptoUtil::Sha256DigestLength = gcry_md_get_algo_dlen(
    GCRY_MD_SHA256);

// TODO: Add exceptions
ByteVector CryptoUtil::hashSha256(const ByteVector& data) {
    ByteVector result_data(Sha256DigestLength);

    // TODO: move to construct
    gcry_md_hd_t handle;
    gcry_md_open(&handle, GCRY_MD_SHA256, 0);

    gcry_md_write(handle, &data[0], data.size());

    std::copy_n(
        gcry_md_read(handle, GCRY_MD_SHA256), Sha256DigestLength,
        result_data.begin()
    );
//    std::memcpy(&result_data[0], gcry_md_read(handle, GCRY_MD_SHA256), Sha256DigestLength);

    // TODO: move to destruct
    gcry_md_close(handle);
    // TODO: add throw gcrypt error

    // TODO: handle errors
//    SHA256_CTX ctx;
//    SHA256_Init(&ctx);
//    SHA256_Update(&ctx, &data[0], data.size());
//    SHA256_Final(&result_data[0], &ctx);


    return result_data;
}

// TODO: Add exceptions
// TODO: Rename?
ByteVector CryptoUtil::getRandomBytes(std::size_t count) {
    ByteVector result_data(count);
    // TODO: handle errors
    gcry_create_nonce(&result_data[0], count);
    return result_data;
}

void CryptoUtil::fillRandomBytes(void* begin, std::size_t count) noexcept {
    // TODO: Is there any errors handle here?
    gcry_create_nonce(begin, count);
}

}
