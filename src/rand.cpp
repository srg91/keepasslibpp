#include "byte_vector.hpp"
#include "rand.hpp"

#include <gcrypt.h>

#include <cstddef>

namespace keepasslibpp {

ByteVector Rand::get(std::size_t count) const noexcept {
    ByteVector result(count);
    this->fill(result);
    return result;
}

void Rand::fillBuffer(void* buffer, std::size_t size) const noexcept {
    if (this->strength == RandomStrength::strong) {
        gcry_randomize(buffer, size, GCRY_STRONG_RANDOM);
    } else if (this->strength == RandomStrength::weak) {
        gcry_create_nonce(buffer, size);
    } else {
        // TODO: exception?
    }
}

}
