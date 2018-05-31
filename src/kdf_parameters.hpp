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
        KdfParameters(VariantDictionary&& vd) : VariantDictionary(std::forward<VariantDictionary>(vd)) {
            types::bytes uuid_bytes;
            if (Get<types::bytes>(uuid_key, uuid_bytes)) {
                kdf_uuid = PwUuid(uuid_bytes);
            } else {
                throw exception::FileCorruptedError();
            }
        }
        const PwUuid& KdfUuid() { return kdf_uuid; }

        std::string SerializeExt() const;
        std::ostream& SerializeExt(std::ostream& stream) const;
        static KdfParameters DeserializeExt(std::istream& stream);
        static KdfParameters DeserializeExt(const std::string& bytes);
    private:
        static const std::string uuid_key;
        PwUuid kdf_uuid;
    };
}
