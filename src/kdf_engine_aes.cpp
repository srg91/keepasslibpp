#include "crypto_util.hpp"
#include "exception.hpp"
#include "kdf_engine_aes.hpp"
#include "kdf_parameters.hpp"
#include "typedefs.hpp"

// TODO: don't forget to move
#include <openssl/evp.h>

#include <cstdint>

using namespace keepasslib;

// uint64
const std::string AesKdf::ParamRounds = "R";
// char[32]
const std::string AesKdf::ParamSeed = "S";

KdfParameters AesKdf::GetDefaultParameters() const  {
    KdfParameters kp = KdfEngine::GetDefaultParameters();
    kp.Set(ParamRounds, DefaultKeyEncryptionRounds);
    return kp;
}

void AesKdf::Randomize(KdfParameters& kp) const {
    // TODO: Sha256DigestLength?
    kp[ParamSeed] = CryptoUtil::GetRandomBytes(CryptoUtil::Sha256DigestLength);
}

types::bytes AesKdf::Transform(types::bytes msg, const KdfParameters& kp) const {
    std::uint64_t rounds;
    if (!kp.Get<std::uint64_t>(ParamRounds, rounds))
        throw exception::ArgumentNullException("rounds");

    types::bytes seed;
    if (!kp.Get<types::bytes>(ParamSeed, seed))
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

types::bytes AesKdf::transformKey(const types::bytes& data, const types::bytes& seed,
                                  std::uint64_t rounds) const {
    types::bytes result_data = data;
    // TODO: Add many many checks
    // TODO: change 16 to some constant?
    // TODO: change it to char* ?
    types::bytes iv(16, 0);
    // TODO: handle errors
    auto ctx = EVP_CIPHER_CTX_new();
    // TODO: handle errors
    EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), nullptr, &seed[0], &iv[0]);

    int len = 16;
    for (std::uint64_t i = 0; i < rounds; i++) {
        // TODO: handle errors
        EVP_EncryptUpdate(ctx, &result_data[0], &len, &result_data[0], len);
        EVP_EncryptUpdate(ctx, &result_data[16], &len, &result_data[16], len);
    }

    EVP_CIPHER_CTX_free(ctx);
    return CryptoUtil::HashSha256(result_data);
}
