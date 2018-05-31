#include "exception.hpp"
#include "kdf_parameters.hpp"
#include "typedefs.hpp"

#include <sstream>
#include <string>

using namespace keepasslib;

const std::string KdfParameters::uuid_key = "$UUID";

std::string KdfParameters::SerializeExt() const {
    return VariantDictionary::Serialize();
}

std::ostream& KdfParameters::SerializeExt(std::ostream& stream) const {
    return VariantDictionary::Serialize(stream);
}

KdfParameters KdfParameters::DeserializeExt(std::istream& stream) {
    auto vd = VariantDictionary::Deserialize(stream);

    types::bytes kdf_uuid;
    if (vd.Get<types::bytes>(KdfParameters::uuid_key, kdf_uuid)) {
        KdfParameters kp(kdf_uuid);
        // TODO: Is any good way to copy?
        vd.CopyTo(kp);
        return kp;
    } else {
        // TODO: raise another exception
        throw exception::FileCorruptedError();
    }
}

KdfParameters KdfParameters::DeserializeExt(const std::string& bytes) {
    std::istringstream stream(bytes);
    return DeserializeExt(stream);
}
