#pragma once

#include "typedefs.hpp"

#include <cstddef>

namespace keepasslibpp {

struct CryptoUtil {
    static const std::size_t Sha256DigestLength;

    static type::ByteVector hashSha256(const type::ByteVector& data);
    static type::ByteVector getRandomBytes(std::size_t count);
    // TODO: can we use iterator here? But without templates?
    static void fillRandomBytes(void* begin, std::size_t count) noexcept;
};

}