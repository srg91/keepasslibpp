#include "byte_vector.hpp"
#include "kdf_engine_argon.hpp"
#include "kdf_parameters.hpp"
#include "rand.hpp"

#include <argon2.h>

#include <cstdint>
#include <numeric>

using namespace keepasslibpp;

const std::string KdfEngineArgon2::paramSalt = "S";
const std::string KdfEngineArgon2::paramParallelism = "P";
const std::string KdfEngineArgon2::paramMemory = "M";
const std::string KdfEngineArgon2::paramIterations = "I";
const std::string KdfEngineArgon2::paramVersion = "V";
const std::string KdfEngineArgon2::paramSecretKey = "K";
const std::string KdfEngineArgon2::paramAssocData = "A";

const std::uint32_t KdfEngineArgon2::minVersion = 0x10;
const std::uint32_t KdfEngineArgon2::maxVersion = 0x13;

const std::uint32_t KdfEngineArgon2::minSalt = 8;
const std::uint32_t KdfEngineArgon2::maxSalt = static_cast<std::uint32_t>(-1);

const std::uint64_t KdfEngineArgon2::minIterations = 1;
const std::uint64_t KdfEngineArgon2::maxIterations = static_cast<std::uint32_t>(-1);

const std::uint64_t KdfEngineArgon2::minMemory = 1024 * 8;
const std::uint64_t KdfEngineArgon2::maxMemory = 1024u * static_cast<std::uint32_t>(-1);

const std::uint32_t KdfEngineArgon2::minParallelism = 1;
const std::uint32_t KdfEngineArgon2::maxParallelism = (1u << 24u) - 1;

const std::uint64_t KdfEngineArgon2::defaultIterations = 2;
const std::uint64_t KdfEngineArgon2::defaultMemory = 1024 * 1024;
const std::uint32_t KdfEngineArgon2::defaultParallelism = 2;

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
        throw exception::ArgumentNullException("version");
    if ((version < minVersion) || (version > maxVersion))
        throw exception::ArgumentOutOfRangeException("version");

    ByteVector salt;
    if (!kp.get<ByteVector>(paramSalt, salt))
        throw exception::ArgumentNullException("salt");
    // TODO: ???
    if ((salt.size() < minSalt) || (salt.size() > maxSalt))
        throw exception::ArgumentOutOfRangeException("salt");

    // TODO: Can we simplify this?
    std::uint32_t parallelism;
    if (!kp.get<std::uint32_t>(paramParallelism, parallelism))
        throw exception::ArgumentNullException("parallelism");
    if ((parallelism < minParallelism) || (parallelism > maxParallelism))
        throw exception::ArgumentOutOfRangeException("parallelism");

    std::uint64_t memory;
    if (!kp.get<std::uint64_t>(paramMemory, memory))
        throw exception::ArgumentNullException("memory");
    if ((memory < minMemory) || (memory > maxMemory))
        throw exception::ArgumentOutOfRangeException("memory");

    std::uint64_t iterations;
    if (!kp.get<std::uint64_t>(paramIterations, iterations))
        throw exception::ArgumentNullException("iterations");
    if ((iterations < minIterations) || (iterations > maxIterations))
        throw exception::ArgumentOutOfRangeException("iterations");

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
        const keepasslibpp::ByteVector& msg,
        const keepasslibpp::ByteVector& salt,
        std::uint32_t parallelism,
        std::uint64_t memory,
        std::uint64_t iterations,
        std::size_t result_size,
        std::uint32_t version,
        const keepasslibpp::ByteVector& secret_key,
        const keepasslibpp::ByteVector& assoc_data) const {
    // TODO: use secret key?
    // TODO: use assoc_data?
    // TODO: check version?
    ByteVector result(result_size);
    argon2d_hash_raw(
        static_cast<std::uint32_t>(iterations),
        static_cast<std::uint32_t>(memory),
        parallelism,
        &msg[0],
        msg.size(),
        &salt[0],
        salt.size(),
        &result[0],
        result_size
    );
    return result;
}
