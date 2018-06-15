#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace keepasspp {

using ByteVector = std::vector<std::uint8_t>;

// TODO: camelCase?
// TODO: string_view
ByteVector to_byte_vector(const std::string& value);
std::string to_string(const keepasspp::ByteVector& value);
std::string to_hex_string(const keepasspp::ByteVector& value);
}
