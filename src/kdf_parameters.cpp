#include "byte_vector.hpp"
#include "exception.hpp"
#include "kdf_parameters.hpp"

#include <sstream>
#include <string>

namespace keepasspp {

Uuid KdfParameters::extractUuid() {
    ByteVector uuid_bytes;
    if (!get<ByteVector>(KdfParameters::uuid_key, uuid_bytes))
        throw exception::InvalidKdfParametersError(
            "cannot find uuid in kdf parameters");
    return Uuid::fromByteVector(uuid_bytes);
}

std::string KdfParameters::SerializeExt() const {
    return VariantDictionary::serialize();
}

std::ostream& KdfParameters::SerializeExt(std::ostream& stream) const {
    return VariantDictionary::serialize(stream);
}

KdfParameters KdfParameters::DeserializeExt(std::istream& stream) {
    return KdfParameters(VariantDictionary::deserialize(stream));
}

KdfParameters KdfParameters::DeserializeExt(const std::string& bytes) {
    std::istringstream stream(bytes);
    return DeserializeExt(stream);
}

}
