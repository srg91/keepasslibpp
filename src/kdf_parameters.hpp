#pragma once

#include "byte_vector.hpp"
#include "exception.hpp"
#include "uuid.hpp"
#include "variant_dictionary.hpp"

#include <iostream>
#include <string>

namespace keepasspp {

class KdfParameters final : public VariantDictionary {
public:
    KdfParameters() = delete;
    explicit KdfParameters(const Uuid& uuid) : kdf_uuid(uuid) {
        set<ByteVector>(uuid_key, uuid.byteVector());
    };
    explicit KdfParameters(VariantDictionary&& vd)
        : VariantDictionary(std::forward<VariantDictionary>(vd))
        , kdf_uuid(extractUuid()) {}
    const Uuid& KdfUuid() { return kdf_uuid; }

    // TODO: rename?
    std::string SerializeExt() const;
    std::ostream& SerializeExt(std::ostream& stream) const;
    static KdfParameters DeserializeExt(std::istream& stream);
    static KdfParameters DeserializeExt(const std::string& bytes);
private:
    Uuid extractUuid();

    inline static const std::string uuid_key = "$UUID";
    const Uuid kdf_uuid;
};

}
