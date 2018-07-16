#pragma once

#include "cipher_adapter.hpp"
#include "crypto_stream_buf.hpp"

#include <iostream>

namespace keepasspp {

class CryptoStream : public std::iostream {
public:
    // TODO: iostream?
    // TODO: remove duplication?
    template <typename C1, typename C2>
    explicit CryptoStream(std::istream& stream,
                          CipherAlgorithm algorithm,
                          CipherMode mode,
                          const C1& key,
                          const C2& iv)
        : buf(stream.rdbuf(), algorithm, mode, key, iv)
        , std::iostream(&buf) {}

    template <typename C1, typename C2>
    explicit CryptoStream(std::ostream& stream,
                          CipherAlgorithm algorithm,
                          CipherMode mode,
                          const C1& key,
                          const C2& iv)
        : buf(stream.rdbuf(), algorithm, mode, key, iv)
        , std::iostream(&buf){}

    // TODO: add read for byte vectors?
private:
    CryptoStreamBuf buf;
};

}