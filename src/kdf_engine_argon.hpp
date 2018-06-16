#pragma once

#include "byte_vector.hpp"
#include "kdf_engine.hpp"

#include <cstdint>
#include <limits>
#include <string>

namespace keepasspp {

class KdfEngineArgon2 : public KdfEngine {
public:
    inline static const std::string paramSalt = "S";
    inline static const std::string paramParallelism = "P";
    inline static const std::string paramMemory = "M";
    inline static const std::string paramIterations = "I";
    inline static const std::string paramVersion = "V";
    inline static const std::string paramSecretKey = "K";
    inline static const std::string paramAssocData = "A";

    const Uuid& getUuid() const override { return uuid; };
    KdfParameters getDefaultParameters() const override;

    void randomize(KdfParameters& kp) const override;
    ByteVector transform(const ByteVector& msg,
                         const KdfParameters& kp) const override;
private:
    Uuid uuid = Uuid::fromByteVector({
        0xef, 0x63, 0x6d, 0xdf, 0x8c, 0x29, 0x44, 0x4b,
        0x91, 0xf7, 0xa9, 0xa4, 0x03, 0xe3, 0x0a, 0x0c
    });

    // TODO: fix Memory, Iterations and etc to match with libargon2 api
    // TODO: use big amounts but check ranges

    // TODO: Change this consts to DEFINES from argon2.h
    // TODO: Is libargon can work with these versions?
    inline static const std::uint32_t minVersion = 0x10;
    inline static const std::uint32_t maxVersion = 0x13;

    inline static const std::uint32_t minSalt = 8;
    inline static const std::uint32_t maxSalt =
        std::numeric_limits<std::uint32_t>::max();

    inline static const std::uint64_t minIterations = 1;
    inline static const std::uint64_t maxIterations =
        std::numeric_limits<std::uint64_t>::max();

    inline static const std::uint64_t minMemory = 8 * 1024;
    // TODO: IS IT REAL??? 4TB???????????
    inline static const std::uint64_t maxMemory =
        1024 * std::numeric_limits<std::uint32_t>::max();

    inline static const std::uint32_t minParallelism = 1;
    inline static const std::uint32_t maxParallelism = (1u << 24u) - 1;

    inline static const std::uint64_t defaultIterations = 2;
    inline static const std::uint64_t defaultMemory = 1 * 1024 * 1024;
    inline static const std::uint32_t defaultParallelism = 2;

    ByteVector transformKey(
        const ByteVector& msg, const ByteVector& salt,
        std::uint32_t parallelism, std::uint64_t memory,
        std::uint64_t iterations, std::size_t result_size,
        std::uint32_t version, const ByteVector& secret_key,
        const ByteVector& assoc_data) const;
};

}
