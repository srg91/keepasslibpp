#include "byte_vector.hpp"

#include <string>

namespace keepasslibpp {

ByteVector to_byte_vector(const std::string& value) {
    return ByteVector(value.begin(), value.end());
}

}

namespace std {
    std::string to_string(const keepasslibpp::ByteVector& value) {
        return std::string(value.begin(), value.end());
    }
}
