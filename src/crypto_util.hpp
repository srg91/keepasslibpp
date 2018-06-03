#pragma once

#include "typedefs.hpp"

namespace keepasslibpp {
    struct CryptoUtil {
        static const std::size_t Sha256DigestLength;

        static type::byte_vector HashSha256(const type::byte_vector& data);
        static type::byte_vector GetRandomBytes(std::size_t count);
    };
}