#pragma once

#include "byte_vector.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace keepasspp {

// TODO: How to documentate consts?
/// Standard size in bytes of a UUID.
const std::size_t UUID_SIZE = 16;

/**
 * @brief Represents an UUID of a password entry or group
 *
 * Once created, Uuid objects aren't modifiable anymore (immutable).
 */
class Uuid {
public:
    /// Underlying uuid type.
    using uuid_t = std::array<std::uint8_t, UUID_SIZE>;

    /// Construct a new UUID instance with random value.
    Uuid() : uuid(createNew()) {};

    // TODO: Move to private? Should we test this?
    /// Construct a new UUID from underlying type.
    explicit Uuid(const uuid_t& value) : uuid(value) {};
    explicit Uuid(uuid_t&& value) : uuid(value) {};

    /// Construct a new UUID instance by range of values.
    template <typename RandomIt>
    explicit Uuid(RandomIt begin, RandomIt end)
        : uuid(createNew(begin, end)) {};

    /// Create nil UUID (all bytes are zero).
    static Uuid nil() noexcept { return Uuid(createNil()); }

    // TODO: initialization_list?
    /// Construct a new UUID from another object.
    static Uuid fromByteVector(const ByteVector& bv);
    static Uuid fromString(const std::string& s);

    /// Returns the 16 bytes size byte vector.
    ByteVector byteVector() const;
    /// Returns the 16 bytes size string.
    std::string string() const;
    // TODO: Optional separators?
    /// Returns the 32 hexadecimal digits in five groups separated by hyphens.
    std::string hex() const;
    /// Fast hash of current UUID bytes.
    std::size_t hash() const noexcept;

    friend bool operator <(const Uuid& left, const Uuid& right);
    friend bool operator ==(const Uuid& left, const Uuid& right);
    friend bool operator !=(const Uuid& left, const Uuid& right);
private:
    uuid_t uuid;

    template <typename RandomIt>
    static void checkSize(RandomIt begin, RandomIt end);

    static uuid_t createNew() noexcept;
    template <typename RandomIt>
    static uuid_t createNew(RandomIt begin, RandomIt end);
    static uuid_t createNil() noexcept;

    constexpr static const auto uuid_t_hash_func =
        std::hash<std::string_view>{};
};

/// Nil UUID (all bytes are zero).
const Uuid UUID_NIL = Uuid::nil();

std::ostream& operator <<(std::ostream& stream, const Uuid& u);

template <typename RandomIt>
void Uuid::checkSize(RandomIt begin, RandomIt end) {
    // TODO: Make keepasspp exception
    auto it_size = end - begin;
    if (it_size != UUID_SIZE) {
        std::string message = "invalid interval size: ";
        message += std::to_string(it_size);
        message += " != ";
        message += std::to_string(UUID_SIZE);
        throw std::invalid_argument(message);
    }
}

template <typename RandomIt>
Uuid::uuid_t Uuid::createNew(RandomIt begin, RandomIt end) {
    Uuid::checkSize(begin, end);

    uuid_t u;
    std::copy(begin, end, u.begin());
    return u;
}

}

namespace std {
    template<>
    struct hash<keepasspp::Uuid>
    {
        using result_type = std::size_t;

        result_type operator()(const keepasspp::Uuid& u) const {
            return u.hash();
        }
    };
}
