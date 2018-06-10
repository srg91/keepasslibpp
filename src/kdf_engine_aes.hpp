#pragma once

#include "defs.hpp"
#include "kdf_engine.hpp"
#include "typedefs.hpp"

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
    type::ByteVector Transform(type::ByteVector msg, const KdfParameters& kp) const override;
private:
    // TODO: static?
    Uuid uuid = type::ByteVector({0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
                                0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea});

    type::ByteVector transformKey(const type::ByteVector& data, const type::ByteVector& seed,
                              std::uint64_t rounds) const;
};

}
