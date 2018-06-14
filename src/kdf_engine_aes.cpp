#include "byte_vector.hpp"
#include "exception.hpp"
#include "hash.hpp"
#include "kdf_engine_aes.hpp"
#include "kdf_parameters.hpp"
#include "rand.hpp"

// TODO: don't forget to move
//#include <openssl/evp.h>
#include <gcrypt.h>

#include <cstdint>

using namespace keepasslibpp;

// uint64
const std::string AesKdf::paramRounds = "R";
// char[32]
const std::string AesKdf::paramSeed = "S";

KdfParameters AesKdf::getDefaultParameters() const  {
    KdfParameters kp = KdfEngine::getDefaultParameters();
    kp.set(paramRounds, DEFAULT_KEY_ENCRYPTION_ROUNDS);
    return kp;
}

void AesKdf::randomize(KdfParameters& kp) const {
    kp[paramSeed] = Rand(RandomStrength::strong).get(AesKdf::defaultSize);
}

ByteVector AesKdf::transform(ByteVector msg, const KdfParameters& kp) const {
    std::uint64_t rounds;
    if (!kp.get<std::uint64_t>(paramRounds, rounds))
        throw exception::ArgumentNullException("rounds");

    ByteVector seed;
    if (!kp.get<ByteVector>(paramSeed, seed))
        throw exception::ArgumentNullException("seed");

    // TODO: something without copy??
    if (msg.size() != AesKdf::defaultSize) {
        msg = Hash(HashAlgorithm::sha256).sum(msg);
    }

    if (seed.size() != AesKdf::defaultSize) {
        seed = Hash(HashAlgorithm::sha256).sum(seed);
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
//    return CryptoUtil::hashSha256(result_data);
//}

ByteVector AesKdf::transformKey(const ByteVector& data, const ByteVector& seed,
                                       std::uint64_t rounds) const {
    // TODO: do not copy?
    ByteVector result_data = data;
    // TODO: Add many many checks
    // TODO: change to constant by gcry_cipher_get_algo_keylen or smtelse
    // TODO: change it to char* ?
    ByteVector iv(16, 0);
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

    return Hash(HashAlgorithm::sha256).sum(result_data);
}
