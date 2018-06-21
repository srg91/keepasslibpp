#pragma once

#include "cipher_engine.hpp"
#include "uuid.hpp"

namespace keepasspp {

class CipherEngineAes : public CipherEngine {
public:
    const Uuid& getUuid() const noexcept override { return uuid; }

// TODO: we need stream creator witch just get unencrypted stream
// TODO: and return stream-proxy witch will used only when read
private:
    inline static const auto uuid = Uuid::fromByteVector({
        0x31, 0xc1, 0xf2, 0xe6, 0xbf, 0x71, 0x43, 0x50,
        0xbe, 0x58, 0x05, 0x21, 0x6a, 0xfc, 0x5a, 0xff
    });
};

}


