#include "exception.hpp"
#include "kdf_parameters.hpp"
#include "typedefs.hpp"

#include <sstream>
#include <string>

using namespace keepasslibpp;

const std::string KdfParameters::uuid_key = "$UUID";

Uuid KdfParameters::extractUuid() {
    type::ByteVector uuid_bytes;
    if (!Get<type::ByteVector>(uuid_key, uuid_bytes))
        throw exception::InvalidKdfParametersError();
    return uuid_bytes;
}

std::string KdfParameters::SerializeExt() const {
    return VariantDictionary::Serialize();
}

std::ostream& KdfParameters::SerializeExt(std::ostream& stream) const {
    return VariantDictionary::Serialize(stream);
}

KdfParameters KdfParameters::DeserializeExt(std::istream& stream) {
    return VariantDictionary::Deserialize(stream);
}

KdfParameters KdfParameters::DeserializeExt(const std::string& bytes) {
    std::istringstream stream(bytes);
    return DeserializeExt(stream);
}
