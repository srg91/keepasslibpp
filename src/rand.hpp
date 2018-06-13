#pragma once

#include "typedefs.hpp"

#include <cstddef>

namespace keepasslibpp {

class Rand {
public:
    // TODO: Change to iterators?
    static void fillRandom(void* buffer, std::size_t size) noexcept;
    static void fillStrongRandom(void* buffer, std::size_t size) noexcept;

    // TODO: Add fill strong random. Add type of random to constructor?
    static type::ByteVector getRandomBytes(std::size_t count);
};

}
