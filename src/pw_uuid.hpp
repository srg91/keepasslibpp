#pragma once

#include <iostream>
#include <string>

/*
 * Represents an UUID of a password entry or group. Once created,
 * PwUuid objects aren't modifiable anymore (immutable).
 */
class PwUuid {
    // TODO: Is this a good way to use string for this?
    // Never "" after constructor
    std::string bytes;

    // Create a new, random UUID.
    void createNew();
public:
    // Standard size in bytes of a UUID.
    static const unsigned UuidSize = 16u;

    // TODO: Rename to Nil?
    // Zero UUID (all bytes are zero).
    static const PwUuid Zero;

    // Construct a new UUID instance with random value.
    PwUuid();

    // Construct a new UUID from another UUID.
    PwUuid(const PwUuid& u);
    PwUuid(const PwUuid&& u);

    // TODO: Guess which category iterator is
    // Construct a new UUID instance by range of values.
    template <typename It>
    explicit PwUuid(It begin, It end);

    // Construct a new UUID by another object.
    PwUuid(const std::string& s);
    PwUuid(const std::string&& s);

    // Get the 16 UUID bytes.
    std::string Bytes();
    // TODO: Optional separators?
    // Get the 32 hexadecimal digits in five groups separated by hyphens.
    std::string ToString();

    friend bool operator <(const PwUuid& left, const PwUuid& right);
    friend bool operator ==(const PwUuid& left, const PwUuid& right);
    friend bool operator !=(const PwUuid& left, const PwUuid& right);
    // TODO: Remove me?
    friend std::ostream& operator <<(std::ostream& o, const PwUuid& u);
};
