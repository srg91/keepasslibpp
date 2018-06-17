#include "byte_vector.hpp"
#include "cipher_adapter.hpp"
#include "exception.hpp"

#include <gcrypt.h>

#include <iterator>
#include <string>

namespace keepasspp {

CipherAdapter::CipherAdapter(keepasspp::CipherAlgorithm algorithm,
                             keepasspp::CipherMode mode)
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
    auto error =
        gcry_cipher_setiv(this->handle, std::data(iv), std::size(iv));
    if (error) CipherAdapter::throwError(error);
}

void CipherAdapter::setKey(const ByteVector& key) {
    auto error =
        gcry_cipher_setkey(this->handle, std::data(key), std::size(key));
    if (error) CipherAdapter::throwError(error);
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
        case CipherMode::ecb:
            return GCRY_CIPHER_MODE_ECB;
        case CipherMode::cbc:
            return GCRY_CIPHER_MODE_CBC;
    }
}

std::string CipherAdapter::throwError(gcry_error_t e) {
    throw exception::CipherInternalError(gcry_strsource(e),
                                         gcry_strerror(e));
}

}