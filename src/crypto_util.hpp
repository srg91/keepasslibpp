#pragma once

#include "typedefs.hpp"

namespace keepasslib {
    struct CryptoUtil {
        static const std::size_t Sha256DigestLength;

        static types::bytes HashSha256(const types::bytes& data);
        static types::bytes GetRandomBytes(std::size_t count);
    };
}