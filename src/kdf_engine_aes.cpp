#include "byte_vector.hpp"
#include "exception.hpp"
#include "hash.hpp"
#include "kdf_engine_aes.hpp"
#include "kdf_parameters.hpp"
#include "rand.hpp"

#include <gcrypt.h>

#include <cstdint>
#include <iterator>

using namespace keepasspp;

KdfParameters KdfEngineAes::getDefaultParameters() const  {
    KdfParameters kp = KdfEngine::getDefaultParameters();
    kp.set(KdfEngineAes::paramRounds, KdfEngineAes::defaultRounds);
    return kp;
}

void KdfEngineAes::randomize(KdfParameters& kp) const {
    kp[KdfEngineAes::paramSeed] =
        Rand(RandomStrength::strong).get(KdfEngineAes::defaultSize);
}

ByteVector KdfEngineAes::transform(const ByteVector& msg,
                                   const KdfParameters& kp) const {
    std::uint64_t rounds;
    if (!kp.get<std::uint64_t>(KdfEngineAes::paramRounds, rounds))
        throw exception::ArgumentIsNullError("rounds");

    ByteVector seed;
    if (!kp.get<ByteVector>(KdfEngineAes::paramSeed, seed))
        throw exception::ArgumentIsNullError("seed");

    // TODO: something without copy?
    auto data = msg;
    if (std::size(data) != KdfEngineAes::defaultSize) {
        data = Hash(HashAlgorithm::sha256).sum(data);
    }

    if (std::size(seed) != KdfEngineAes::defaultSize) {
        seed = Hash(HashAlgorithm::sha256).sum(seed);
    }

    return transformKey(data, seed, rounds);
}

ByteVector KdfEngineAes::transformKey(const ByteVector& data,
                                      const ByteVector& seed,
                                      std::uint64_t rounds) const {
    // TODO: do not copy?
    ByteVector result_data = data;
    // TODO: Add many many checks
    // TODO: move this to constructor
    ByteVector iv(gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256), 0);
    // TODO: handle errors
    // TODO: handle errors
    gcry_cipher_hd_t handle;

    // TODO: add gcry_control?
    // TODO: handle errors
    gcry_cipher_open(&handle, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    gcry_cipher_setiv(handle, std::data(iv), std::size(iv));
    gcry_cipher_setkey(handle, std::data(seed), std::size(seed));

    // TODO: make constant by gcry_cipher_get_algo_keylen or smtelse
    size_t len = gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);
    for (std::uint64_t i = 0; i < rounds; i++) {
        // TODO: handle errors
        gcry_cipher_encrypt(
            handle,
            std::data(result_data), len,
            std::data(result_data), len
        );
        gcry_cipher_encrypt(
            handle,
            std::data(result_data) + len, len,
            std::data(result_data) + len, len
        );
    }

    return Hash(HashAlgorithm::sha256).sum(result_data);
}
