#include "byte_vector.hpp"
#include "cipher_adapter.hpp"

#include <gcrypt.h>

#include <iterator>

namespace keepasspp {

CipherAdapter::CipherAdapter(keepasspp::CipherAlgorithm algorithm,
                             keepasspp::CipherMode mode)
        : algorithm(algorithm)
        , mode(mode)
        , keyLength(CipherAdapter::getKeyLength(algorithm))
        , blockLength(CipherAdapter::getBlockLength(algorithm)) {
    // TODO: Handle errors
    gcry_cipher_open(
        &this->handle,
        CipherAdapter::getMappedAlgorithm(this->algorithm),
        CipherAdapter::getMappedMode(this->mode),
        // TODO: do we need ability to change this flag?
        0
    );
};

CipherAdapter::~CipherAdapter() {
    gcry_cipher_close(this->handle);
}

std::size_t CipherAdapter::getBlockLength(
        keepasspp::CipherAlgorithm algorithm) noexcept {
    return gcry_cipher_get_algo_blklen(
        CipherAdapter::getMappedAlgorithm(algorithm)
    );
}

std::size_t CipherAdapter::getKeyLength(
        keepasspp::CipherAlgorithm algorithm) noexcept {
    return gcry_cipher_get_algo_keylen(
        CipherAdapter::getMappedAlgorithm(algorithm)
    );
}

void CipherAdapter::setIv(const ByteVector& iv) {
    // TODO: handle errors
    gcry_cipher_setiv(this->handle, std::data(iv), std::size(iv));
}

void CipherAdapter::setKey(const ByteVector& key) {
    // TODO: handle errors
    gcry_cipher_setkey(this->handle, std::data(key), std::size(key));
}

int CipherAdapter::getMappedAlgorithm(
        keepasspp::CipherAlgorithm algorithm) noexcept {
    switch (algorithm) {
        case CipherAlgorithm::aes256:
            return GCRY_CIPHER_AES256;
    }
}

int CipherAdapter::getMappedMode(keepasspp::CipherMode mode) noexcept {
    switch (mode) {
//        case CipherMode::none:
//            return GCRY_CIPHER_MODE_NONE;
        case CipherMode::ecb:
            return GCRY_CIPHER_MODE_ECB;
    }
}

}