#include "rand.hpp"
#include "typedefs.hpp"

#include <gcrypt.h>

namespace keepasslibpp {

void Rand::fillRandom(void* buffer, std::size_t size) noexcept {
    gcry_create_nonce(buffer, size);
}

void Rand::fillStrongRandom(void* buffer, std::size_t size) noexcept {
    gcry_randomize(buffer, size, GCRY_STRONG_RANDOM);
}

type::ByteVector Rand::getRandomBytes(std::size_t count) {
    type::ByteVector buffer(count);
    Rand::fillRandom(buffer.data(), buffer.size());
    return buffer;
}
