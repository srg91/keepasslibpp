#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

/*
 * Represents an UUID of a password entry or group. Once created,
 * PwUuid objects aren't modifiable anymore (immutable).
 */
class PwUuid {
    // Never empty after constructor
    std::string bytes;

    // Create a new, random UUID.
    void createNew();

    // Checks is the string length equals UuidSize.
    void checkSize(const std::string& s) const;
    template <typename It> void checkSize(It begin, It end) const;
public:
    // Standard size in bytes of a UUID.
    static const size_t UuidSize = 16u;

    // TODO: Rename to Nil?
    // Zero UUID (all bytes are zero).
    static const PwUuid Zero;

    // Construct a new UUID instance with random value.
    PwUuid();

    // Construct a new UUID from another UUID.
    PwUuid(const PwUuid& u);
    PwUuid(PwUuid&& u);

    // TODO: Guess which category iterator is
    // Construct a new UUID instance by range of values.
    template <typename It>
    explicit PwUuid(It begin, It end);

    // Construct a new UUID by another object.
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
    bytes = std::string(begin, end);
}
