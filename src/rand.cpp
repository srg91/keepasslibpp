#include "rand.hpp"

#include <gcrypt.h>

namespace keepasslibpp {

void Rand::fillRandom(void* buffer, std::size_t length) {
    gcry_create_nonce(buffer, length);
}

void Rand::fillStrongRandom(void* buffer, std::size_t length) {
    gcry_randomize(buffer, length, GCRY_STRONG_RANDOM);
}

}