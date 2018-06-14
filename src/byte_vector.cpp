#include "byte_vector.hpp"

#include <iomanip>
#include <sstream>
#include <string>

namespace keepasslibpp {

ByteVector to_byte_vector(const std::string& value) {
    return ByteVector(value.begin(), value.end());
}

std::string to_string(const keepasslibpp::ByteVector& value) {
    return std::string(value.begin(), value.end());
}

std::string to_hex_string(const keepasslibpp::ByteVector& value) {
    std::ostringstream os;
    os << std::hex << std::setfill('0');
    for (const auto& i : value) {
        os << std::setw(2) << static_cast<unsigned>(i);
    }
    return os.str();
}

}
