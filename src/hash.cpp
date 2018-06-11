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
    // TODO: handle errors
    // TODO: save gcrypt algo to private var
    gcry_md_open(&handle, Hash::getGcryptAlgorithm(algorithm), 0);
}

Hash::~Hash() {
    gcry_md_close(handle);
}

void Hash::sum(void* out) {
    std::copy_n(
        // TODO: handle errors
        gcry_md_read(handle, Hash::getGcryptAlgorithm(algorithm)),
        // TODO: save length to private var
        Hash::getAlgorithmLength(this->algorithm),
        reinterpret_cast<char*>(out)
    );
}

void Hash::sum(void* in, std::size_t size, void* out) {
    this->write(in, size);
    this->sum(out);
}

std::string Hash::hexSum() {
    std::ostringstream os;
    os << std::hex << std::setfill('0');

    auto it = gcry_md_read(handle, Hash::getGcryptAlgorithm(algorithm));
    for (std::size_t i = 0; i < Hash::getAlgorithmLength(this->algorithm); ++i)
        os << std::setw(2) << static_cast<unsigned>(*it++);
    return os.str();
}

std::string Hash::hexSum(void* in, std::size_t size) {
    this->write(in, size);
    return this->hexSum();
}

void Hash::write(void* in, std::size_t size) {
    gcry_md_write(handle, in, size);
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
