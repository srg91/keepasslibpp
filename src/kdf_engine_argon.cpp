#include "byte_vector.hpp"
#include "crypto_util.hpp"
#include "kdf_engine_argon.hpp"
#include "kdf_parameters.hpp"

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

void Argon2Kdf::Randomize(KdfParameters& kp) const {
    // TODO: Sha256DigestLength?
    kp[ParamSalt] = CryptoUtil::getRandomBytes(CryptoUtil::Sha256DigestLength);
}

ByteVector Argon2Kdf::Transform(ByteVector msg, const KdfParameters& kp) const {
    std::uint32_t version;
    if (!kp.get<std::uint32_t>(ParamVersion, version))
        throw exception::ArgumentNullException("version");
    if ((version < minVersion) || (version > maxVersion))
        throw exception::ArgumentOutOfRangeException("version");

    ByteVector salt;
    if (!kp.get<ByteVector>(ParamSalt, salt))
        throw exception::ArgumentNullException("salt");
    // TODO: ???
    if ((salt.size() < minSalt) || (salt.size() > maxSalt))
        throw exception::ArgumentOutOfRangeException("salt");

    // TODO: Can we simplify this?
    std::uint32_t parallelism;
    if (!kp.get<std::uint32_t>(ParamParallelism, parallelism))
        throw exception::ArgumentNullException("parallelism");
    if ((parallelism < minParallelism) || (parallelism > maxParallelism))
        throw exception::ArgumentOutOfRangeException("parallelism");

    std::uint64_t memory;
    if (!kp.get<std::uint64_t>(ParamMemory, memory))
        throw exception::ArgumentNullException("memory");
    if ((memory < minMemory) || (memory > maxMemory))
        throw exception::ArgumentOutOfRangeException("memory");

    std::uint64_t iterations;
    if (!kp.get<std::uint64_t>(ParamIterations, iterations))
        throw exception::ArgumentNullException("iterations");
    if ((iterations < minIterations) || (iterations > maxIterations))
        throw exception::ArgumentOutOfRangeException("iterations");

    // TODO: Add checks?
    ByteVector secret_key;
    kp.get<ByteVector>(ParamSecretKey, secret_key);
    ByteVector assoc_data;
    kp.get<ByteVector>(ParamAssocData, assoc_data);

    // TODO: What about memory? Shod we clear smthg
    // TODO: change 32 to const?
    return transformKey(
        msg, salt, parallelism, memory, iterations, 32,
        version, secret_key, assoc_data
    );
}

ByteVector Argon2Kdf::transformKey(ByteVector msg, ByteVector salt,
                                   std::uint32_t parallelism, std::uint64_t memory, std::uint64_t iterations,
                                   std::size_t result_size, std::uint32_t version,
                                   ByteVector secret_key, ByteVector assoc_data) const {
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
