#include "byte_vector.hpp"
#include "cipher_adapter.hpp"
#include "exception.hpp"
#include "hash.hpp"
#include "kdf_engine_aes.hpp"
#include "kdf_parameters.hpp"
#include "rand.hpp"

#include <gcrypt.h>

#include <cstdint>
#include <iterator>

namespace keepasspp {

KdfParameters KdfEngineAes::getDefaultParameters() const  {
    KdfParameters kp = KdfEngine::getDefaultParameters();
    kp.set(KdfEngineAes::paramRounds, KdfEngineAes::defaultRounds);
    return kp;
}

void KdfEngineAes::randomize(KdfParameters& kp) const {
    kp[KdfEngineAes::paramSeed] =
        Rand(RandomStrength::strong).get(KdfEngineAes::keySize);
}

ByteVector KdfEngineAes::transform(const ByteVector& msg,
                                   const KdfParameters& kp) const {
    auto rounds = kp.get<std::uint64_t>(KdfEngineAes::paramRounds);
    auto seed = kp.get<ByteVector>(KdfEngineAes::paramSeed);

    // TODO: something without copy?
    auto data = msg;
    if (std::size(data) != KdfEngineAes::keySize) {
        data = Hash(HashAlgorithm::sha256).sum(data);
    }

    if (std::size(seed) != KdfEngineAes::keySize) {
        seed = Hash(HashAlgorithm::sha256).sum(seed);
    }

    return transformKey(data, seed, rounds);
}

ByteVector KdfEngineAes::transformKey(const ByteVector& data,
                                      const ByteVector& seed,
                                      std::uint64_t rounds) const {
    // TODO: do not copy?
    ByteVector result_data = data;

    CipherAdapter cipher(CipherAlgorithm::aes256, CipherMode::ecb);

    ByteVector iv(KdfEngineAes::blockSize, 0);
    cipher.setIv(iv);
    cipher.setKey(seed);

    size_t len = KdfEngineAes::blockSize;
    for (std::uint64_t i = 0; i < rounds; i++) {
        cipher.encrypt(
            std::begin(result_data),
            std::begin(result_data),
            std::size(result_data)
        );
    }

    return Hash(HashAlgorithm::sha256).sum(result_data);
}

std::size_t KdfEngineAes::getKeySize() noexcept {
    return gcry_cipher_get_algo_keylen(GCRY_CIPHER_AES256);
}

std::size_t KdfEngineAes::getBlockSize() noexcept {
    return gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);
}

}
