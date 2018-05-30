#pragma once

#include "pw_uuid.hpp"
#include "variant_dictionary.hpp"

namespace keepasslib {
    class KdfParameters final : public VariantDictionary {
    public:
        explicit KdfParameters(const PwUuid& uuid) : kdf_uuid(uuid) {

        };
        const PwUuid& KdfUuid() { return kdf_uuid; }


    private:
        const std::string uuid_param = "$UUID";
        const PwUuid kdf_uuid;
    };
}
