#include "crypto_util.hpp"
#include "exception.hpp"
#include "kdf_engine_aes.hpp"
#include "kdf_parameters.hpp"
#include "typedefs.hpp"

// TODO: don't forget to move
//#include <openssl/evp.h>
#include <gcrypt.h>

#include <cstdint>

using namespace keepasslibpp;

// uint64
const std::string AesKdf::ParamRounds = "R";
// char[32]
const std::string AesKdf::ParamSeed = "S";

KdfParameters AesKdf::GetDefaultParameters() const  {
    KdfParameters kp = KdfEngine::GetDefaultParameters();
    kp.set(ParamRounds, DEFAULT_KEY_ENCRYPTION_ROUNDS);
    return kp;
}

void AesKdf::Randomize(KdfParameters& kp) const {
    // TODO: Sha256DigestLength?
    kp[ParamSeed] = CryptoUtil::GetRandomBytes(CryptoUtil::Sha256DigestLength);
}

type::ByteVector AesKdf::Transform(type::ByteVector msg, const KdfParameters& kp) const {
    std::uint64_t rounds;
    if (!kp.get<std::uint64_t>(ParamRounds, rounds))
        throw exception::ArgumentNullException("rounds");

    type::ByteVector seed;
    if (!kp.get<type::ByteVector>(ParamSeed, seed))
        throw exception::ArgumentNullException("seed");

    // TODO: something without copy??
    if (msg.size() != CryptoUtil::Sha256DigestLength) {
        msg = CryptoUtil::HashSha256(msg);
    }

    if (seed.size() != CryptoUtil::Sha256DigestLength) {
        seed = CryptoUtil::HashSha256(seed);
    }

    // TODO: do not copy msg?
    return transformKey(msg, seed, rounds);
}

//type::ByteVector AesKdf::transformKey(const type::ByteVector& data, const type::ByteVector& seed,
//                                  std::uint64_t rounds) const {
//    type::ByteVector result_data = data;
//    // TODO: Add many many checks
//    // TODO: change 16 to some constant?
//    // TODO: change it to char* ?
//    type::ByteVector iv(16, 0);
//    // TODO: handle errors
//    auto ctx = EVP_CIPHER_CTX_new();
//    // TODO: handle errors
//    EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), nullptr, &seed[0], &iv[0]);
//
//    int len = 16;
//    for (std::uint64_t i = 0; i < rounds; i++) {
//        // TODO: handle errors
//        EVP_EncryptUpdate(ctx, &result_data[0], &len, &result_data[0], len);
//        EVP_EncryptUpdate(ctx, &result_data[16], &len, &result_data[16], len);
//    }
//
//    EVP_CIPHER_CTX_free(ctx);
//    return CryptoUtil::HashSha256(result_data);
//}

type::ByteVector AesKdf::transformKey(const type::ByteVector& data, const type::ByteVector& seed,
                                       std::uint64_t rounds) const {
    // TODO: do not copy?
    type::ByteVector result_data = data;
    // TODO: Add many many checks
    // TODO: change to constant by gcry_cipher_get_algo_keylen or smtelse
    // TODO: change it to char* ?
    type::ByteVector iv(16, 0);
    // TODO: handle errors
    // TODO: handle errors
    gcry_cipher_hd_t handle;

    // TODO: add gcry_control?
    // TODO: handle errors
    gcry_cipher_open(&handle, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    gcry_cipher_setiv(handle, &iv[0], iv.size());
    gcry_cipher_setkey(handle, &seed[0], seed.size());

    // TODO: make constant by gcry_cipher_get_algo_keylen or smtelse
    size_t len = 16;
    for (std::uint64_t i = 0; i < rounds; i++) {
        // TODO: handle errors
        gcry_cipher_encrypt(handle, &result_data[0], len, &result_data[0], len);
        gcry_cipher_encrypt(handle, &result_data[16], len, &result_data[16], len);
    }

    return CryptoUtil::HashSha256(result_data);
}