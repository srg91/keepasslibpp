#include "byte_vector.hpp"
#include "hash.hpp"

#include <gcrypt.h>

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iterator>
#include <sstream>

namespace keepasslibpp {

Hash::Hash(keepasslibpp::HashAlgorithm algorithm) {
    this->algorithm = algorithm;
    this->digestSize = Hash::getAlgorithmLength(this->algorithm);
    // TODO: handle errors
    // TODO: save gcrypt algo to private var
    gcry_md_open(&handle, Hash::getGcryptAlgorithm(algorithm), 0);
}

Hash::~Hash() {
    gcry_md_close(handle);
}

ByteVector Hash::sum() {
    ByteVector result(this->digestSize);
    std::copy_n(
        // TODO: handle errors
        gcry_md_read(handle, Hash::getGcryptAlgorithm(algorithm)),
        this->digestSize,
        std::begin(result)
    );
    return result;
}

std::string Hash::hexSum() {
    return to_hex_string(this->sum());
}

Hash& Hash::writeBuffer(const void* in, std::size_t size) {
    gcry_md_write(handle, in, size);
    return *this;
}

std::size_t Hash::getAlgorithmLength(HashAlgorithm algorithm) noexcept {
    return gcry_md_get_algo_dlen(getGcryptAlgorithm(algorithm));
}

int Hash::getGcryptAlgorithm(keepasslibpp::HashAlgorithm algorithm) noexcept {
    int gcrypt_algorithm = 0;

    switch (algorithm) {
    case HashAlgorithm::sha256:
        gcrypt_algorithm = GCRY_MD_SHA256;
        break;
    }

    return gcrypt_algorithm;
}

}
