#pragma once

#include "byte_vector.hpp"
#include "defs.hpp"
#include "kdf_engine.hpp"

#include <cstdint>
#include <string>

namespace keepasslibpp {

class AesKdf : KdfEngine {
public:
    static const std::string ParamRounds;
    static const std::string ParamSeed;

    // TODO: static?
    const Uuid& GetUuid() const override { return uuid; }
    KdfParameters GetDefaultParameters() const override;

    void Randomize(KdfParameters& kp) const override;
    ByteVector Transform(ByteVector msg, const KdfParameters& kp) const override;
private:
    // TODO: static?
    Uuid uuid = Uuid::fromByteVector({
        0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
        0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea
    });

    ByteVector transformKey(const ByteVector& data, const ByteVector& seed,
                            std::uint64_t rounds) const;
};

}
