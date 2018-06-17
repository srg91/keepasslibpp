#pragma once

#include "byte_vector.hpp"
#include "exception.hpp"

#include <gcrypt.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <cstddef>

// TODO: Rename?
namespace keepasspp {

enum class CipherAlgorithm {
    aes256
};

enum class CipherMode {
    ecb,
    cbc
};

// TODO: add check algorithm exists in gcrypt
// TODO: rename to facade?
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
    template <typename InputIt, typename OutputIt>
    void decrypt(InputIt input, OutputIt output, std::size_t count);
private:
    const CipherAlgorithm algorithm;
    const CipherMode mode;

    const std::size_t keyLength;
    const std::size_t blockLength;

    gcry_cipher_hd_t handle;

    enum class Action {
        encrypt,
        decrypt
    };

    template <typename InputIt, typename OutputIt>
    void cipher(InputIt input, OutputIt output, std::size_t count,
                Action action);
    
    static int getMappedAlgorithm(CipherAlgorithm algorithm) noexcept;
    static int getMappedMode(CipherMode mode) noexcept;
    // TODO: string_view?
    static std::string throwError(gcry_error_t e);
};

template <typename InputIt, typename OutputIt>
void CipherAdapter::cipher(InputIt input, OutputIt output, std::size_t count,
                           CipherAdapter::Action action) {
    if ((count % this->blockLength) != 0)
        // TODO: more beautiful message
        throw exception::InputNotMultipleByBlockSize(count, this->blockLength);

    std::function<decltype(gcry_cipher_encrypt)> cipher_func;
    switch (action) {
        case CipherAdapter::Action::encrypt:
            cipher_func = gcry_cipher_encrypt;
            break;
        case CipherAdapter::Action::decrypt:
            cipher_func = gcry_cipher_decrypt;
            break;
    }

    ByteVector input_buffer(this->blockLength);
    ByteVector output_buffer(this->blockLength);

    while (count > 0) {
        std::copy(input, std::next(input, this->blockLength),
                  std::begin(input_buffer));

        auto error = cipher_func(
            this->handle,
            std::data(output_buffer), std::size(output_buffer),
            std::data(input_buffer), std::size(input_buffer)
        );
        if (error) CipherAdapter::throwError(error);

        std::copy(std::begin(output_buffer), std::end(output_buffer),
                  output);

        // TODO: simplfy?
        count -= this->blockLength;
        std::advance(input, this->blockLength);
        std::advance(output, this->blockLength);
    }
};

template <typename InputIt, typename OutputIt>
void CipherAdapter::encrypt(InputIt input, OutputIt output, std::size_t count) {
    this->cipher(input, output, count, CipherAdapter::Action::encrypt);
}

template <typename InputIt, typename OutputIt>
void CipherAdapter::decrypt(InputIt input, OutputIt output, std::size_t count) {
    this->cipher(input, output, count, CipherAdapter::Action::decrypt);
}

}
