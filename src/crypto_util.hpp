#pragma once

#include "byte_vector.hpp"

#include <cstddef>

namespace keepasslibpp {

struct CryptoUtil {
    static const std::size_t Sha256DigestLength;

    static ByteVector hashSha256(const ByteVector& data);
    static ByteVector getRandomBytes(std::size_t count);
    // TODO: can we use iterator here? But without templates?
    static void fillRandomBytes(void* begin, std::size_t count) noexcept;
};

}