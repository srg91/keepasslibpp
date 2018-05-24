#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

/*
 * Represents an UUID of a password entry or group. Once created,
 * PwUuid objects aren't modifiable anymore (immutable).
 */
class PwUuid {
public:
    // Underlying uuid type
    using uuid_t = boost::uuids::uuid;
    // Underlying uuid generator type
    using uuid_generator_t = boost::uuids::random_generator;

    // Standard size in bytes of a UUID.
    static const size_t UuidSize = uuid_t::static_size();

    // Nil UUID (all bytes are zero).
    static const PwUuid Nil;

    // Construct a new UUID instance with random value.
    PwUuid() : uuid(uuid_generator()) {};

    // Construct a new UUID from another UUID.
    PwUuid(const PwUuid& u);
    PwUuid(PwUuid&& u);

    // TODO: Guess which category iterator is
    // Construct a new UUID instance by range of values.
    template <typename It>
    explicit PwUuid(It begin, It end);

    // TODO: Fix ambiguous in aggregate initialization
    // TODO: (In cases like u = {1, 2, 3, ...};
    // Construct a new UUID by another object.
    PwUuid(const uuid_t& u) : uuid(u) {};
    PwUuid(uuid_t&& u) : uuid(std::forward<decltype(u)>(u)) {};
    PwUuid(const std::string& s);
    PwUuid(std::string&& s);

    // Get the 16 UUID bytes.
    std::string Bytes() const;
    // TODO: Optional separators?
    // Get the 32 hexadecimal digits in five groups separated by hyphens.
    std::string ToString() const;

    friend bool operator <(const PwUuid& left, const PwUuid& right);
    friend bool operator ==(const PwUuid& left, const PwUuid& right);
    friend bool operator !=(const PwUuid& left, const PwUuid& right);
private:
    // Never empty after constructor.
    uuid_t uuid;

    // Boost random generator.
    static uuid_generator_t uuid_generator;

    // Checks is the string length equals UuidSize.
    void checkSize(const std::string& s) const;
    template <typename It> void checkSize(It begin, It end) const;
};

std::ostream& operator <<(std::ostream& stream, const PwUuid& u);

template <typename It>
void PwUuid::checkSize(It begin, It end) const {
    auto it_size = end - begin;
    if (it_size != UuidSize) {
        std::string message = "invalid interval size: ";
        message += std::to_string(it_size);
        message += " != ";
        message += std::to_string(UuidSize);
        throw std::invalid_argument(message);
    }
}

template <typename It>
PwUuid::PwUuid(It begin, It end) {
    checkSize(begin, end);
    std::copy(begin, end, uuid.begin());
}
