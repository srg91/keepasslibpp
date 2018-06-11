#pragma once

#include <cstddef>

namespace keepasslibpp {

class Rand {
public:
    static void fillRandom(void* buffer, std::size_t length);
    static void fillStrongRandom(void* buffer, std::size_t length);
};

}
