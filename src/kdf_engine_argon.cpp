#include "crypto_util.hpp"
#include "kdf_engine_argon.hpp"
#include "kdf_parameters.hpp"
#include "typedefs.hpp"

#include <argon2.h>

#include <cstdint>
#include <numeric>

using namespace keepasslibpp;

const std::string Argon2Kdf::ParamSalt = "S";
const std::string Argon2Kdf::ParamParallelism = "P";
const std::string Argon2Kdf::ParamMemory = "M";
const std::string Argon2Kdf::ParamIterations = "I";
const std::string Argon2Kdf::ParamVersion = "V";
const std::string Argon2Kdf::ParamSecretKey = "K";
const std::string Argon2Kdf::ParamAssocData = "A";

const std::uint32_t Argon2Kdf::minVersion = 0x10;
const std::uint32_t Argon2Kdf::maxVersion = 0x13;

const std::uint32_t Argon2Kdf::minSalt = 8;
const std::uint32_t Argon2Kdf::maxSalt = static_cast<std::uint32_t>(-1);

const std::uint64_t Argon2Kdf::minIterations = 1;
const std::uint64_t Argon2Kdf::maxIterations = static_cast<std::uint32_t>(-1);

const std::uint64_t Argon2Kdf::minMemory = 1024 * 8;
const std::uint64_t Argon2Kdf::maxMemory = 1024u * static_cast<std::uint32_t>(-1);

const std::uint32_t Argon2Kdf::minParallelism = 1;
const std::uint32_t Argon2Kdf::maxParallelism = (1u << 24u) - 1;

const std::uint64_t Argon2Kdf::defaultIterations = 2;
const std::uint64_t Argon2Kdf::defaultMemory = 1024 * 1024;
const std::uint32_t Argon2Kdf::defaultParallelism = 2;

KdfParameters Argon2Kdf::GetDefaultParameters() const {
    KdfParameters kp = KdfEngine::GetDefaultParameters();

    kp[ParamVersion] = Argon2Kdf::maxVersion;
    kp[ParamIterations] = Argon2Kdf::defaultIterations;
    kp[ParamMemory] = Argon2Kdf::defaultMemory;
    kp[ParamParallelism] = Argon2Kdf::defaultParallelism;

    return kp;
}

void Argon2Kdf::Randomize(keepasslibpp::KdfParameters& kp) const {
    // TODO: Sha256DigestLength?
    kp[ParamSalt] = CryptoUtil::GetRandomBytes(CryptoUtil::Sha256DigestLength);
}

types::bytes Argon2Kdf::Transform(keepasslibpp::types::bytes msg, const keepasslibpp::KdfParameters& kp) const {
    std::uint32_t version;
    if (!kp.Get<std::uint32_t>(ParamVersion, version))
        throw exception::ArgumentNullException("version");
    if ((version < minVersion) || (version > maxVersion))
        throw exception::ArgumentOutOfRangeException("version");

    types::bytes salt;
    if (!kp.Get<types::bytes>(ParamSalt, salt))
        throw exception::ArgumentNullException("salt");
    // TODO: ???
    if ((salt.size() < minSalt) || (salt.size() > maxSalt))
        throw exception::ArgumentOutOfRangeException("salt");

    // TODO: Can we simplify this?
    std::uint32_t parallelism;
    if (!kp.Get<std::uint32_t>(ParamParallelism, parallelism))
        throw exception::ArgumentNullException("parallelism");
    if ((parallelism < minParallelism) || (parallelism > maxParallelism))
        throw exception::ArgumentOutOfRangeException("parallelism");

    std::uint64_t memory;
    if (!kp.Get<std::uint64_t>(ParamMemory, memory))
        throw exception::ArgumentNullException("memory");
    if ((memory < minMemory) || (memory > maxMemory))
        throw exception::ArgumentOutOfRangeException("memory");

    std::uint64_t iterations;
    if (!kp.Get<std::uint64_t>(ParamIterations, iterations))
        throw exception::ArgumentNullException("iterations");
    if ((iterations < minIterations) || (iterations > maxIterations))
        throw exception::ArgumentOutOfRangeException("iterations");

    // TODO: Add checks?
    types::bytes secret_key;
    kp.Get<types::bytes>(ParamSecretKey, secret_key);
    types::bytes assoc_data;
    kp.Get<types::bytes>(ParamAssocData, assoc_data);

    // TODO: What about memory? Shod we clear smthg
    // TODO: change 32 to const?
    return transformKey(
        msg, salt, parallelism, memory, iterations, 32,
        version, secret_key, assoc_data
    );
}

types::bytes Argon2Kdf::transformKey(keepasslibpp::types::bytes msg, keepasslibpp::types::bytes salt,
                                     std::uint32_t parallelism, std::uint64_t memory, std::uint64_t iterations,
                                     std::size_t result_size, std::uint32_t version,
                                     keepasslibpp::types::bytes secret_key, keepasslibpp::types::bytes assoc_data) const {
    // TODO: use secret key?
    // TODO: use assoc_data?
    // TODO: check version?
    types::bytes result(result_size);
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
