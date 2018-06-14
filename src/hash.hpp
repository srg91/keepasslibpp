#pragma once

#include "byte_vector.hpp"

// TODO: move to facade?
#include <gcrypt.h>

#include <cstddef>
#include <iterator>
#include <string>

namespace keepasslibpp {

// TODO: Move to enums?
enum class HashAlgorithm {
    sha256
};

// TODO: Rework?
class Hash {
public:
    explicit Hash(HashAlgorithm algorithm);
    ~Hash();

    ByteVector sum();
    std::string hexSum();

    template <typename Container>
    ByteVector sum(const Container& container);

    template <typename Container>
    std::string hexSum(const Container& container);

    // TODO: Return hash and remove container from sum and hexSum?
    template <typename Container>
    Hash& write(const Container& container);

    static std::size_t getAlgorithmLength(HashAlgorithm algorithm) noexcept;
private:
    // TODO: move this to another class?
    gcry_md_hd_t handle;
    HashAlgorithm algorithm;
    std::size_t digestSize;

    // TODO: move this to another class?
    static int getGcryptAlgorithm(HashAlgorithm algorithm) noexcept;

    Hash& writeBuffer(const void* in, std::size_t size);
};

template <typename Container>
ByteVector Hash::sum(const Container& container) {
    this->write(container);
    return this->sum();
}

template <typename Container>
std::string Hash::hexSum(const Container& container) {
    this->write(container);
    return this->hexSum();
}

template <typename Container>
Hash& Hash::write(const Container& container) {
    return this->writeBuffer(std::data(container), std::size(container));
}


}
