#include "byte_vector.hpp"
#include "exception.hpp"
#include "kdf_engine_argon.hpp"
#include "kdf_parameters.hpp"
#include "rand.hpp"

#include <argon2.h>

#include <cstdint>
#include <iterator>
#include <numeric>

namespace keepasspp {

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
    // TODO: std::clamp? or smth else?
    auto version = kp.get<std::uint32_t>(paramVersion);
    if ((version < minVersion) || (version > maxVersion))
        throw exception::ArgumentIsOutOfRangeError("version");

    auto salt = kp.get<ByteVector>(paramSalt);
    if ((std::size(salt) < minSalt) || (std::size(salt) > maxSalt))
        throw exception::ArgumentIsOutOfRangeError("salt");

    auto parallelism = kp.get<std::uint32_t>(paramParallelism);
    if ((parallelism < minParallelism) || (parallelism > maxParallelism))
        throw exception::ArgumentIsOutOfRangeError("parallelism");

    auto memory = kp.get<std::uint64_t>(paramMemory);
    if ((memory < minMemory) || (memory > maxMemory))
        throw exception::ArgumentIsOutOfRangeError("memory");

    auto iterations = kp.get<std::uint64_t>(paramIterations);
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
        const ByteVector& msg,
        const ByteVector& salt,
        std::uint32_t parallelism,
        std::uint64_t memory,
        std::uint64_t iterations,
        std::size_t result_size,
        std::uint32_t version,
        const ByteVector& secret_key,
        const ByteVector& assoc_data) const {

    // TODO: Do not copy
//    auto msg_ = msg;
//    auto salt_ = salt;
//    auto secret_key_ = secret_key;
//    auto assoc_data_ = assoc_data;

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
//
//    // TODO: cast size
//    argon2_context context = {
//        std::data(result),
//        static_cast<std::uint32_t>(std::size(result)),
//        std::data(msg_),
//        static_cast<std::uint32_t>(std::size(msg)),
//        std::data(salt_),
//        static_cast<std::uint32_t>(std::size(salt)),
//        std::data(secret_key_),
//        static_cast<std::uint32_t>(std::size(secret_key)),
//        std::data(assoc_data_),
//        static_cast<std::uint32_t>(std::size(assoc_data)),
//        static_cast<std::uint32_t>(iterations),
//        static_cast<std::uint32_t>(memory),
//        parallelism,
//        parallelism,
//        version,
//        nullptr, nullptr,
//        ARGON2_DEFAULT_FLAGS
//    };
//
//    int rc = argon2d_ctx(&context);
//    if (rc != ARGON2_OK)
//        // TODO: change exception
//        throw exception::CipherInternalError(
//            "argon2", argon2_error_message(rc));

    return result;
}

}
