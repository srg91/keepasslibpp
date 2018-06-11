#include "rand.hpp"

#include <gcrypt.h>

namespace keepasslibpp {

void Rand::fillRandom(void* buffer, std::size_t size) {
    gcry_create_nonce(buffer, size);
}

void Rand::fillStrongRandom(void* buffer, std::size_t size) {
    gcry_randomize(buffer, size, GCRY_STRONG_RANDOM);
}

}