#pragma once

#include <cstddef>

namespace keepasslibpp {

class Rand {
public:
    // TODO: Change to iterators?
    static void fillRandom(void* buffer, std::size_t size);
    static void fillStrongRandom(void* buffer, std::size_t size);
};

}
