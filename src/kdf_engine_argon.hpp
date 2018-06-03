#pragma once

#include "kdf_engine.hpp"
#include "typedefs.hpp"

#include <cstdint>
#include <string>

namespace keepasslibpp {
    class Argon2Kdf : KdfEngine {
    public:
        static const std::string ParamSalt;
        static const std::string ParamParallelism;
        static const std::string ParamMemory;
        static const std::string ParamIterations;
        static const std::string ParamVersion;
        static const std::string ParamSecretKey;
        static const std::string ParamAssocData;

        const PwUuid& GetUuid() const override { return uuid; };
        KdfParameters GetDefaultParameters() const override;

        void Randomize(KdfParameters& kp) const override;
        type::byte_vector Transform(type::byte_vector msg, const KdfParameters& kp) const override;
    private:
        PwUuid uuid = type::byte_vector({0xef, 0x63, 0x6d, 0xdf, 0x8c, 0x29, 0x44, 0x4b,
                                    0x91, 0xf7, 0xa9, 0xa4, 0x03, 0xe3, 0x0a, 0x0c});

        // TODO: fix Memory, Iterations and etc to match with libargon2 api
        // TODO: use big amounts but check ranges

        // TODO: Change this consts to DEFINES from argon2.h
        // TODO: Is libargon can work with these versions?
        static const std::uint32_t minVersion;
        static const std::uint32_t maxVersion;

        static const std::uint32_t minSalt;
        // TODO: std::numeric_limits<std::uint32_t>::max() ?
        static const std::uint32_t maxSalt;

        static const std::uint64_t minIterations;
        // TODO: std::numeric_limits<std::uint32_t>::max() ?
        static const std::uint64_t maxIterations;

        static const std::uint64_t minMemory;
        // TODO: 1024 * std::numeric_limits<std::uint32_t>::max() ?
        // TODO: IS IT REAL??? 4TB???????????
        static const std::uint64_t maxMemory;

        static const std::uint32_t minParallelism;
        static const std::uint32_t maxParallelism;

        static const std::uint64_t defaultIterations;
        static const std::uint64_t defaultMemory;
        static const std::uint32_t defaultParallelism;

        // TODO: fix const &
        type::byte_vector transformKey(
            type::byte_vector msg, type::byte_vector salt, std::uint32_t parallelism,
            std::uint64_t memory, std::uint64_t iterations, std::size_t result_size,
            std::uint32_t version, type::byte_vector secret_key, type::byte_vector assoc_data
        ) const;
    };
}
