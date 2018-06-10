#pragma once

#include "kdf_parameters.hpp"
#include "uuid.hpp"
#include "typedefs.hpp"

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
    virtual type::ByteVector Transform(type::ByteVector msg, const KdfParameters& kp) const = 0;
    // TODO: Add benchmarks
};

}
