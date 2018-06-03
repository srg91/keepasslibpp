#pragma once

#include "pw_defs.hpp"
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
        const PwUuid& GetUuid() const override { return uuid; }
        KdfParameters GetDefaultParameters() const override;

        void Randomize(KdfParameters& kp) const override;
        type::byte_vector Transform(type::byte_vector msg, const KdfParameters& kp) const override;
    private:
        // TODO: static?
        PwUuid uuid = type::byte_vector({0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
                                    0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea});

        type::byte_vector transformKey(const type::byte_vector& data, const type::byte_vector& seed,
                                  std::uint64_t rounds) const;
    };
}
