#pragma once

#include "exception.hpp"
#include "pw_uuid.hpp"
#include "variant_dictionary.hpp"
#include "typedefs.hpp"

#include <iostream>
#include <string>

namespace keepasslib {
    class KdfParameters final : public VariantDictionary {
    public:
        explicit KdfParameters(const PwUuid& uuid) : kdf_uuid(uuid) {
            Set<types::bytes>(uuid_key, uuid.Bytes());
        };
        const PwUuid& KdfUuid() { return kdf_uuid; }

        std::string SerializeExt() const;
        std::ostream& SerializeExt(std::ostream& stream) const;
        static KdfParameters DeserializeExt(std::istream& stream);
        static KdfParameters DeserializeExt(const std::string& bytes);
    private:
        static const std::string uuid_key;
        const PwUuid kdf_uuid;
    };
}
