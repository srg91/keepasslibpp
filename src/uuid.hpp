#pragma once

#include "typedefs.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace keepasslibpp {

/*
 * Represents an UUID of a password entry or group. Once created,
 * Uuid objects aren't modifiable anymore (immutable).
 */
class Uuid {
public:
    // Underlying uuid type
    using uuid_t = boost::uuids::uuid;
    // Underlying uuid generator type
    using uuid_generator_t = boost::uuids::random_generator;

    // Standard size in ByteVector of a UUID.
    static const size_t UuidSize = uuid_t::static_size();

    // Nil UUID (all ByteVector are zero).
    static const Uuid Nil;

    // Construct a new UUID instance with random value.
    Uuid() : uuid(uuid_generator()) {};

    // Construct a new UUID from another UUID.
    Uuid(const Uuid& u);
    Uuid(Uuid&& u);

    // TODO: Guess which category iterator is
    // Construct a new UUID instance by range of values.
    template <typename It>
    explicit Uuid(It begin, It end);

    // TODO: Fix ambiguous in aggregate initialization
    // TODO: (In cases like u = {1, 2, 3, ...};
    // Construct a new UUID by another object.
    Uuid(const uuid_t& u) : uuid(u) {};
    Uuid(uuid_t&& u) : uuid(std::forward<decltype(u)>(u)) {};
    Uuid(const std::string& s);
    Uuid(std::string&& s);
    Uuid(const type::ByteVector& b);
    Uuid(type::ByteVector&& b);

    // Get the 16 UUID ByteVector.
    type::ByteVector Bytes() const;
    // Get the 16 UUID ByteVector string.
    std::string ByteString() const;
    // TODO: Optional separators?
    // Get the 32 hexadecimal digits in five groups separated by hyphens.
    std::string ToString() const;
    // Hash of current UUID
    inline std::size_t Hash() const { return boost::uuids::hash_value(uuid); };

    Uuid& operator=(const Uuid& u) {
        uuid = u.uuid;
    }

    Uuid& operator=(Uuid&& u) {
        std::swap(uuid, u.uuid);
    }

    friend bool operator <(const Uuid& left, const Uuid& right);
    friend bool operator ==(const Uuid& left, const Uuid& right);
    friend bool operator !=(const Uuid& left, const Uuid& right);
private:
    // Never empty after constructor.
    uuid_t uuid;

    // Boost random generator.
    static uuid_generator_t uuid_generator;

    // Checks is the string length equals UuidSize.
    template <typename T> void checkSize(const T& s) const;
    template <typename It> void checkSize(It begin, It end) const;
};

std::ostream& operator <<(std::ostream& stream, const Uuid& u);

template <typename T>
void Uuid::checkSize(const T& s) const {
    if (s.size() != Uuid::UuidSize) {
        std::ostringstream es;
        es << "value " << '"';
        es << std::hex << std::setfill('0');
        for (const auto i : s) {
            es << std::setw(2) << static_cast<unsigned>(static_cast<unsigned char>(i));
        }
        es << '"' << " has incorrect size: "
           << std::dec << s.size() << " != " << Uuid::UuidSize;
        throw std::invalid_argument(es.str());
    }
}

template <typename It>
void Uuid::checkSize(It begin, It end) const {
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
Uuid::Uuid(It begin, It end) {
    checkSize(begin, end);
    std::copy(begin, end, uuid.begin());
}

}

namespace std {
    template<>
    struct hash<keepasslibpp::Uuid>
    {
        std::size_t operator()(const keepasslibpp::Uuid& u) const {
            return u.Hash();
        }
    };
}
