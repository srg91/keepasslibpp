#include "exception.hpp"
#include "kdf_parameters.hpp"
#include "typedefs.hpp"

#include <sstream>
#include <string>

using namespace keepasslib;

const std::string KdfParameters::uuid_key = "$UUID";

PwUuid KdfParameters::extractUuid() {
    types::bytes uuid_bytes;
    if (!Get<types::bytes>(uuid_key, uuid_bytes))
        throw exception::FileCorruptedError();
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
