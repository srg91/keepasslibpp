#pragma once

#include "byte_vector.hpp"
#include "cipher_adapter.hpp"
#include "kdf_engine.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace keepasspp {

class KdfEngineAes : public KdfEngine {
public:
    // std::uint64_t
    inline static const std::string paramRounds = "R";
    // ByteVector
    inline static const std::string paramSeed = "S";

    const Uuid& getUuid() const noexcept override { return uuid; }
    KdfParameters getDefaultParameters() const override;

    void randomize(KdfParameters& kp) const override;
    ByteVector transform(const ByteVector& msg,
                         const KdfParameters& kp) const override;
private:
    inline static const auto uuid = Uuid::fromByteVector({
        0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
        0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea
    });

    inline static const std::size_t defaultRounds = 6000;

    inline static const std::size_t keySize =
        CipherAdapter::getKeyLength(CipherAlgorithm::aes256);

    ByteVector transformKey(const ByteVector& data, const ByteVector& seed,
                            std::uint64_t rounds) const;
};

}
