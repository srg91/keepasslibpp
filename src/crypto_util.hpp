#pragma once

#include "typedefs.hpp"

namespace keepasslibpp {

struct CryptoUtil {
    static const std::size_t Sha256DigestLength;

    static type::ByteVector HashSha256(const type::ByteVector& data);
    static type::ByteVector GetRandomBytes(std::size_t count);
};

}