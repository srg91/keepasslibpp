#include "byte_vector.hpp"
#include "kdf_engine_argon.hpp"
#include "kdf_parameters.hpp"
#include "rand.hpp"

#include <argon2.h>

#include <cstdint>
#include <iterator>
#include <numeric>

using namespace keepasspp;

KdfParameters KdfEngineArgon2::getDefaultParameters() const {
    KdfParameters kp = KdfEngine::getDefaultParameters();

    kp[paramVersion] = KdfEngineArgon2::maxVersion;
    kp[paramIterations] = KdfEngineArgon2::defaultIterations;
    kp[paramMemory] = KdfEngineArgon2::defaultMemory;
    kp[paramParallelism] = KdfEngineArgon2::defaultParallelism;

    return kp;
}

void KdfEngineArgon2::randomize(KdfParameters& kp) const {
    // TODO: const?
    kp[paramSalt] = Rand(RandomStrength::strong).get(32);
}

ByteVector KdfEngineArgon2::transform(const ByteVector& msg,
                                const KdfParameters& kp) const {
    std::uint32_t version;
    if (!kp.get<std::uint32_t>(paramVersion, version))
        throw exception::ArgumentIsNullError("version");
    if ((version < minVersion) || (version > maxVersion))
        throw exception::ArgumentIsOutOfRangeError("version");

    ByteVector salt;
    if (!kp.get<ByteVector>(paramSalt, salt))
        throw exception::ArgumentIsNullError("salt");
    // TODO: ???
    if ((std::size(salt) < minSalt) || (std::size(salt) > maxSalt))
        throw exception::ArgumentIsOutOfRangeError("salt");

    // TODO: Can we simplify this?
    std::uint32_t parallelism;
    if (!kp.get<std::uint32_t>(paramParallelism, parallelism))
        throw exception::ArgumentIsNullError("parallelism");
    if ((parallelism < minParallelism) || (parallelism > maxParallelism))
        throw exception::ArgumentIsOutOfRangeError("parallelism");

    std::uint64_t memory;
    if (!kp.get<std::uint64_t>(paramMemory, memory))
        throw exception::ArgumentIsNullError("memory");
    if ((memory < minMemory) || (memory > maxMemory))
        throw exception::ArgumentIsOutOfRangeError("memory");

    std::uint64_t iterations;
    if (!kp.get<std::uint64_t>(paramIterations, iterations))
        throw exception::ArgumentIsNullError("iterations");
    if ((iterations < minIterations) || (iterations > maxIterations))
        throw exception::ArgumentIsOutOfRangeError("iterations");

    // TODO: Add checks?
    ByteVector secret_key;
    kp.get<ByteVector>(paramSecretKey, secret_key);
    ByteVector assoc_data;
    kp.get<ByteVector>(paramAssocData, assoc_data);

    // TODO: What about memory? Shod we clear smthg
    // TODO: change 32 to const?
    return transformKey(
        msg, salt, parallelism, memory, iterations, 32,
        version, secret_key, assoc_data
    );
}

ByteVector KdfEngineArgon2::transformKey(
        const keepasspp::ByteVector& msg,
        const keepasspp::ByteVector& salt,
        std::uint32_t parallelism,
        std::uint64_t memory,
        std::uint64_t iterations,
        std::size_t result_size,
        std::uint32_t version,
        const keepasspp::ByteVector& secret_key,
        const keepasspp::ByteVector& assoc_data) const {
    // TODO: use secret key?
    // TODO: use assoc_data?
    // TODO: check version?
    ByteVector result(result_size);
    argon2d_hash_raw(
        static_cast<std::uint32_t>(iterations),
        static_cast<std::uint32_t>(memory),
        parallelism,
        std::data(msg),
        std::size(msg),
        std::data(salt),
        std::size(salt),
        std::data(result),
        result_size
    );
    return result;
}
