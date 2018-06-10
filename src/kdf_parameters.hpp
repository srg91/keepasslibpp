#pragma once

#include "exception.hpp"
#include "uuid.hpp"
#include "variant_dictionary.hpp"
#include "typedefs.hpp"

#include <iostream>
#include <string>

namespace keepasslibpp {

class KdfParameters final : public VariantDictionary {
public:
    KdfParameters() = delete;
    explicit KdfParameters(const Uuid& uuid) : kdf_uuid(uuid) {
        Set<type::ByteVector>(uuid_key, uuid.Bytes());
    };
    KdfParameters(VariantDictionary&& vd)
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

    static const std::string uuid_key;
    const Uuid kdf_uuid;
};

}
