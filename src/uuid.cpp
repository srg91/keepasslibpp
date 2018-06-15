#include "byte_vector.hpp"
#include "rand.hpp"
#include "uuid.hpp"

#include <array>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>

namespace keepasspp {

// TODO: Template? Inline?
Uuid Uuid::fromByteVector(const ByteVector& bv) {
    return Uuid(bv.begin(), bv.end());
}

// TODO: Template? Inline?
Uuid Uuid::fromString(const std::string& s) {
    return Uuid(s.begin(), s.end());
}

ByteVector Uuid::byteVector() const {
    ByteVector bv(UUID_SIZE);
    std::copy(uuid.begin(), uuid.end(), bv.begin());
    return bv;
}

std::string Uuid::string() const {
    std::string s;
    s.resize(UUID_SIZE);
    std::copy(uuid.begin(), uuid.end(), s.begin());
    return s;
}

std::string Uuid::hex() const {
    std::ostringstream os;
    os << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < UUID_SIZE; i++) {
        os << std::setw(2) << static_cast<unsigned int>(uuid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) os << '-';
    }
    return os.str();
}

Uuid::uuid_t Uuid::createNew() noexcept {
    uuid_t u;

    Rand(RandomStrength::weak).fill(u);

    // set variant as rfc4122
    u[8] &= 0xbf;
    u[8] |= 0x80;

    // set version to 4
    u[6] &= 0x0f;
    u[6] |= 0x40;

    return u;
}

Uuid::uuid_t Uuid::createNil() noexcept {
    return {0};
}

std::size_t Uuid::hash() const noexcept {
    return uuid_t_hash_func({reinterpret_cast<const char*>(std::data(uuid)),
                             std::size(uuid)});
}

std::ostream& operator <<(std::ostream& stream, const Uuid& u) {
    return stream << u.hex();
}

bool operator <(const Uuid& left, const Uuid& right) {
    return left.uuid < right.uuid;
}

bool operator ==(const Uuid& left, const Uuid& right) {
    return left.uuid == right.uuid;
}

bool operator !=(const Uuid& left, const Uuid& right) {
    return !(left == right);
}

}
