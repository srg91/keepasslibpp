#pragma once

#include "byte_vector.hpp"
#include "kdf_parameters.hpp"
#include "uuid.hpp"

#include <cstdint>
#include <string>

namespace keepasspp {

class KdfEngine {
public:
    virtual const Uuid& getUuid() const noexcept = 0;

    virtual KdfParameters getDefaultParameters() const {
        return KdfParameters(getUuid());
    }

    // TODO: Add check for uuid and etc?
    virtual void randomize(KdfParameters& kp) const = 0;
    // TODO: ByteVector? string? ???
    // TODO: Const msg? How to work without change?
    virtual ByteVector transform(const ByteVector& msg,
                                 const KdfParameters& kp) const = 0;
    // TODO: Add benchmarks
};

}
