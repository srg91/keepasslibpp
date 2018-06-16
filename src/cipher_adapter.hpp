#pragma once

#include "byte_vector.hpp"

#include <gcrypt.h>

#include <algorithm>
#include <cstddef>

// TODO: Rename?
namespace keepasspp {

enum class CipherAlgorithm {
    aes256
};

enum class CipherMode {
    ecb
};

class CipherAdapter {
public:
    explicit CipherAdapter(CipherAlgorithm algorithm, CipherMode mode);
    ~CipherAdapter();

    static std::size_t getBlockLength(CipherAlgorithm algorithm) noexcept;
    static std::size_t getKeyLength(CipherAlgorithm algorithm) noexcept;

    void setIv(const ByteVector& iv);
    void setKey(const ByteVector& key);

    template <typename InputIt, typename OutputIt>
    void encrypt(InputIt input, OutputIt output, std::size_t count);
    // TODO: implement
    template <typename InputIt, typename OutputIt>
    void decrypt(InputIt input, OutputIt output, std::size_t count);
private:
    const CipherAlgorithm algorithm;
    const CipherMode mode;

    const std::size_t keyLength;
    const std::size_t blockLength;

    gcry_cipher_hd_t handle;
    
    static int getMappedAlgorithm(CipherAlgorithm algorithm) noexcept;
    static int getMappedMode(CipherMode mode) noexcept;
};

template <typename InputIt, typename OutputIt>
void CipherAdapter::encrypt(InputIt input, OutputIt output,
                                  std::size_t count) {
    ByteVector input_buffer(this->blockLength);
    ByteVector output_buffer(this->blockLength);

    // TODO: check length
    while (count > 0) {
        std::copy(input, input + this->blockLength,
                  std::begin(input_buffer));

        // TODO: handle errors
        gcry_cipher_encrypt(
            this->handle,
            std::data(output_buffer), std::size(output_buffer),
            std::data(input_buffer), std::size(input_buffer)
        );

        std::copy(std::begin(output_buffer), std::end(output_buffer), output);

        // TODO: simplfy?
        count -= this->blockLength;
        input += this->blockLength;
        output += this->blockLength;
    }
}

template <typename InputIt, typename OutputIt>
void CipherAdapter::decrypt(InputIt input, OutputIt output,
                                  std::size_t count) {
    ByteVector input_buffer(this->blockLength);
    ByteVector output_buffer(this->blockLength);

    // TODO: check length
    while (count > 0) {
        count -= this->blockLength;
        std::copy(input, input + this->blockLength, std::begin(input_buffer));

        // TODO: handle errors
        gcry_cipher_decrypt(
            this->handle,
            std::data(output_buffer), std::size(output_buffer),
            std::data(input_buffer), std::size(input_buffer)
        );

        std::copy(std::begin(output_buffer), std::end(output_buffer), output);
    }
}

}
