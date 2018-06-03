#pragma once

#include "pw_defs.hpp"
#include "kdf_engine.hpp"
#include "typedefs.hpp"

#include <cstdint>
#include <string>

namespace keepasslib {
    class AesKdf : KdfEngine {
    public:
        static const std::string ParamRounds;
        static const std::string ParamSeed;

        // TODO: static?
        const PwUuid& GetUuid() const override { return uuid; }
        KdfParameters GetDefaultParameters() const override;

        void Randomize(KdfParameters& kp) const override;
        types::bytes Transform(types::bytes msg, const KdfParameters& kp) const override;
    private:
        // TODO: static?
        PwUuid uuid = types::bytes({0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
                                    0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea});

        types::bytes transformKey(const types::bytes& data, const types::bytes& seed,
                                  std::uint64_t rounds) const;
    };
}
