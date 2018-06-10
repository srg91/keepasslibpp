#include "crypto_util.hpp"

//#include <openssl/rand.h>
//#include <openssl/sha.h>
#include <cstring>
#include <algorithm>

#include <gcrypt.h>

#include <cstdint>


using namespace keepasslibpp;

const std::size_t CryptoUtil::Sha256DigestLength = gcry_md_get_algo_dlen(GCRY_MD_SHA256);

    // TODO: Add exceptions
type::ByteVector CryptoUtil::HashSha256(const type::ByteVector& data) {
    type::ByteVector result_data(Sha256DigestLength);

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
type::ByteVector CryptoUtil::GetRandomBytes(std::size_t count) {
    type::ByteVector result_data(count);
    // TODO: handle errors
//    RAND_bytes(&result_data[0], static_cast<int>(count));
    gcry_create_nonce(&result_data[0], count);
    return result_data;
}
