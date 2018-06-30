#include "byte_vector.hpp"
#include "cipher_adapter.hpp"
#include "exception.hpp"

#include <gcrypt.h>

#include <iterator>
#include <string>

namespace keepasspp {

CipherAdapter::CipherAdapter(CipherAlgorithm algorithm,
                             CipherMode mode)
        : algorithm(algorithm)
        , mode(mode)
        , keyLength(CipherAdapter::getKeyLength(algorithm))
        , blockLength(CipherAdapter::getBlockLength(algorithm)) {
    auto error = gcry_cipher_open(
        &this->handle,
        CipherAdapter::getMappedAlgorithm(this->algorithm),
        CipherAdapter::getMappedMode(this->mode),
        // TODO: do we need ability to change this flag?
        0
    );
    if (error) CipherAdapter::throwError(error);
};

CipherAdapter::~CipherAdapter() {
    gcry_cipher_close(this->handle);
}

std::size_t CipherAdapter::getBlockLength(CipherAlgorithm algorithm) noexcept {
    return gcry_cipher_get_algo_blklen(
        CipherAdapter::getMappedAlgorithm(algorithm)
    );
}

std::size_t CipherAdapter::getKeyLength(CipherAlgorithm algorithm) noexcept {
    return gcry_cipher_get_algo_keylen(
        CipherAdapter::getMappedAlgorithm(algorithm)
    );
}

int CipherAdapter::getMappedAlgorithm(CipherAlgorithm algorithm) noexcept {
    switch (algorithm) {
        case CipherAlgorithm::aes256:
            return GCRY_CIPHER_AES256;
        case CipherAlgorithm::chacha20:
            return GCRY_CIPHER_CHACHA20;
    }
}

int CipherAdapter::getMappedMode(CipherMode mode) noexcept {
    switch (mode) {
        case CipherMode::ecb:
            return GCRY_CIPHER_MODE_ECB;
        case CipherMode::cbc:
            return GCRY_CIPHER_MODE_CBC;
        case CipherMode::stream:
            return GCRY_CIPHER_MODE_STREAM;
    }
}

void CipherAdapter::throwError(gcry_error_t e) {
    if (!e) return;
    throw exception::CipherInternalError(gcry_strsource(e),
                                         gcry_strerror(e));
}

void CipherAdapter::cipher(const void* input, std::size_t input_size,
                           void* output, std::size_t output_size,
                           CipherAdapter::Direction direction) {
    // TODO: should we check input / output size?
    // TODO: is it can be different?
    std::function<decltype(gcry_cipher_encrypt)> cipher_func;
    switch (direction) {
        case CipherAdapter::Direction::encrypt:
            cipher_func = gcry_cipher_encrypt;
            break;
        case CipherAdapter::Direction::decrypt:
            cipher_func = gcry_cipher_decrypt;
            break;
    }

    auto error = cipher_func(
        this->handle,
        output, output_size,
        input, input_size
    );
    if (error) CipherAdapter::throwError(error);
}

void CipherAdapter::encrypt(const void* input, std::size_t input_size,
                            void* output, std::size_t output_size) {
    this->cipher(input, input_size, output, output_size,
                 CipherAdapter::Direction::encrypt);
}

void CipherAdapter::encrypt(void* input, std::size_t input_size) {
    this->cipher(input, input_size, input, input_size,
                 CipherAdapter::Direction::encrypt);
}


//TODO: can we remove this copy paste?
void CipherAdapter::decrypt(const void* input, std::size_t input_size,
                            void* output, std::size_t output_size) {
    this->cipher(input, input_size, output, output_size,
                 CipherAdapter::Direction::decrypt);
}

void CipherAdapter::decrypt(void* input, std::size_t input_size) {
    this->cipher(input, input_size, input, input_size,
                 CipherAdapter::Direction::decrypt);
}

}