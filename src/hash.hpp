#pragma once

// TODO: move to facade?
#include <gcrypt.h>

#include <cstddef>
#include <string>

namespace keepasslibpp {

// TODO: Move to enums?
enum class HashAlgorithm {
    sha256
};

class Hash {
public:
    explicit Hash(HashAlgorithm algorithm);
    ~Hash();

    // TODO: change to iterators
    void sum(void* out);
    void sum(void* in, std::size_t size, void* out);

    // TODO: change to iterators
    std::string hexSum();
    std::string hexSum(void* in, std::size_t size);

    // TODO: Change to iterators
    void write(void* in, std::size_t size);

    static std::size_t getAlgorithmLength(HashAlgorithm algorithm) noexcept;
private:
    // TODO: move this to another class?
    gcry_md_hd_t handle;
    HashAlgorithm algorithm;

    // TODO: move this to another class?
    static int getGcryptAlgorithm(HashAlgorithm algorithm) noexcept;
};

}
