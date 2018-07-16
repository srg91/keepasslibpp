#pragma once

#include "byte_vector.hpp"
#include "exception.hpp"
#include "iterator_reference.hpp"

#include <gcrypt.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <cstddef>

// TODO: Rename?
namespace keepasspp {

enum class CipherAlgorithm {
    aes256,
    chacha20
};

enum class CipherMode {
    ecb,
    cbc,
    stream
};

// TODO: add check algorithm exists in gcrypt
// TODO: rename to facade?
class CipherAdapter {
public:
    explicit CipherAdapter(CipherAlgorithm algorithm, CipherMode mode);
    ~CipherAdapter();

    // TODO: rename?
    std::size_t getBlockLength() const noexcept { return this->blockLength; }
    std::size_t getKeyLength() const noexcept { return this->keyLength; }

    // TODO: or rename this?
    static std::size_t getBlockLength(CipherAlgorithm algorithm) noexcept;
    static std::size_t getKeyLength(CipherAlgorithm algorithm) noexcept;

    bool isStream() { return this->mode == CipherMode::stream; }
    bool isBlock() { return !this->isStream(); }

    // TODO: setIv have to be used after setKey. Should we force check it?
    template <typename Container>
    void setKey(const Container& key);
    template <typename Container>
    void setIv(const Container& iv);

    // TODO: how we can return new output?
    template <typename Container>
    void encrypt(Container& input);
    // TODO: Add check is has std::data?
    template <typename C1, typename C2, typename = std::enable_if_t<!std::is_trivial_v<C2>>>
    void encrypt(const C1& input, C2& output);
    void encrypt(void* input, std::size_t input_size);
    void encrypt(const void* input, std::size_t input_size,
                 void* output, std::size_t output_size);

    template <typename Container>
    void decrypt(Container& input);
    // TODO: Add check is has std::data?
    template <typename C1, typename C2, typename = std::enable_if_t<!std::is_trivial_v<C2>>>
    void decrypt(const C1& input, C2& output);
    void decrypt(void* input, std::size_t input_size);
    void decrypt(const void* input, std::size_t input_size,
                 void* output, std::size_t output_size);
private:
    const CipherAlgorithm algorithm;
    const CipherMode mode;

    const std::size_t keyLength;
    const std::size_t blockLength;

    gcry_cipher_hd_t handle;

    enum class Direction {
        encrypt,
        decrypt
    };

    // TODO: change it to span in c++20? or iterators?
    void cipher(const void* input, std::size_t input_size,
                void* output, std::size_t output_size,
                Direction direction);

    static int getMappedAlgorithm(CipherAlgorithm algorithm) noexcept;
    static int getMappedMode(CipherMode mode) noexcept;
    // TODO: string_view?
    static void throwError(gcry_error_t e);
};

template <typename Container>
void CipherAdapter::setKey(const Container& key) {
    auto error =
        gcry_cipher_setkey(this->handle, std::data(key), std::size(key));
    if (error) CipherAdapter::throwError(error);
}

template <typename Container>
void CipherAdapter::setIv(const Container& iv) {
    auto error =
        gcry_cipher_setiv(this->handle, std::data(iv), std::size(iv));
    if (error) CipherAdapter::throwError(error);
}

template <typename C1, typename C2, typename>
void CipherAdapter::encrypt(const C1& input, C2& output) {
    this->encrypt(
        std::data(input), std::size(input),
        std::data(output), std::size(output)
    );
}

template <typename Container>
void CipherAdapter::encrypt(Container& input) {
    this->encrypt(input, input);
}

template <typename C1, typename C2, typename>
void CipherAdapter::decrypt(const C1& input, C2& output) {
    this->decrypt(
        std::data(input), std::size(input),
        std::data(output), std::size(output)
    );
}

template <typename Container>
void CipherAdapter::decrypt(Container& input) {
    this->decrypt(input, input);
}

}
