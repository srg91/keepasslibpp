#pragma once

#include "byte_vector.hpp"
#include "kdf_parameters.hpp"
#include "uuid.hpp"

#include <cstdint>
#include <string>

namespace keepasslibpp {

class KdfEngine {
public:
    virtual const Uuid& GetUuid() const = 0;

    virtual KdfParameters GetDefaultParameters() const {
        return KdfParameters(GetUuid());
    }

    // TODO: Add check for uuid and etc?
    virtual void Randomize(KdfParameters& kp) const = 0;
    // TODO: ByteVector? string? ???
    // TODO: Const msg? How to work without change?
    virtual ByteVector Transform(ByteVector msg, const KdfParameters& kp) const = 0;
    // TODO: Add benchmarks
};

}
