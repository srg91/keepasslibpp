#pragma once

#include "byte_vector.hpp"
#include "uuid.hpp"

#include <cstddef>
#include <iostream>

namespace keepasspp {

class CipherEngine {
public:
    virtual const Uuid& getUuid() const noexcept = 0;

    virtual std::size_t getKeyLength() const = 0;
    virtual std::size_t getIvLength() const = 0;

    // TODO: return cryptostream
    virtual std::istream& encrypt(std::istream& input,
                                  const ByteVector& key,
                                  const ByteVector& iv) = 0;
    virtual std::istream& decrypt(std::istream& input,
                                  const ByteVector& key,
                                  const ByteVector& iv) = 0;
};

}
