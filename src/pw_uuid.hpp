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
    std::string m_pbUuid;

    // Create a new, random UUID.
    void createNew();
    void setZero();
public:
    // Standard size in bytes of a UUID.
    const unsigned UuidSize = 16u;

    // Zero UUID (all bytes are zero).
    static const PwUuid Zero;

    // Construct a new UUID instance.
    // If bCreateNew parameter is true, a new UUID is generated.
    // If it is false, the UUID is initialized to zero.
    explicit PwUuid(bool bCreateNew);

    // Get the 16 UUID bytes.
    std::string UuidBytes();

    friend bool operator ==(const PwUuid& left, const PwUuid& right);
    friend bool operator !=(const PwUuid& left, const PwUuid& right);
    // TODO: Remove me?
    friend std::ostream& operator <<(std::ostream& o, const PwUuid& u);
};
