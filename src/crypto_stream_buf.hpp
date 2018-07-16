#pragma once

#include <algorithm>
#include <streambuf>
#include "cipher_adapter.hpp"

#include <iostream>

namespace keepasspp {

// TODO: Add buffer with block size
// TODO: and add underflow
class CryptoStreamBuf : public std::streambuf {
public:
    // TODO: add normal buffer and ability to read/write by symbol?
    // TODO: wait for adapter?
    template <typename C1, typename C2>
    explicit CryptoStreamBuf(std::streambuf* sbuf,
                             CipherAlgorithm algorithm,
                             CipherMode mode,
                             const C1& key,
                             const C2& iv)
        : sbuf(sbuf)
        , cipher(CipherAdapter(algorithm, mode))
          // TODO: move 1024 to param?
        , buf(cipher.isBlock() ? cipher.getBlockLength() : 1024) {
        this->cipher.setKey(key);
        this->cipher.setIv(iv);
    }
private:
    std::streamsize xsgetn(char_type* s, std::streamsize count) final {
        this->sbuf->sgetn(s, count);
        this->cipher.decrypt(s, count);
    }
    std::streamsize xsputn(const char_type* s, std::streamsize count) final {
        // TODO: test length?
        std::copy_n(s, count, std::begin(buf));
        this->cipher.encrypt(std::data(buf), count);
        this->sbuf->sputn(std::data(buf), count);
        this->buf = {0};
    }

    std::streambuf* sbuf;
    CipherAdapter cipher;

    std::vector<char_type> buf;
};

}




