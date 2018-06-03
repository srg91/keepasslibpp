#pragma once

#include "kdf_parameters.hpp"
#include "pw_uuid.hpp"
#include "typedefs.hpp"

#include <cstdint>
#include <string>

namespace keepasslibpp {
    class KdfEngine {
    public:
        virtual const PwUuid& GetUuid() const = 0;

        virtual KdfParameters GetDefaultParameters() const {
            return KdfParameters(GetUuid());
        }

        // TODO: Add check for uuid and etc?
        virtual void Randomize(KdfParameters& kp) const = 0;
        // TODO: bytes? string? ???
        // TODO: Const msg? How to work without change?
        virtual types::bytes Transform(types::bytes msg, const KdfParameters& kp) const = 0;
        // TODO: Do we need this?
//        virtual KdfParameters GetBestParameters(unsigned int milliseconds);
//    protected:
//        void MaximizeParamUint64(KdfParameters& kp, std::string key, std::uint64_t min, std::uint64_t max, std::uint32_t milliseconds, bool interpSearch);
    };
}
