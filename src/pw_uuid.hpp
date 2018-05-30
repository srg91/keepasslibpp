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

namespace keepasslib {
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
        PwUuid(const types::bytes& b);
        PwUuid(types::bytes&& b);

        // Get the 16 UUID bytes.
        types::bytes Bytes() const;
        // Get the 16 UUID bytes string.
        std::string ByteString() const;
        // TODO: Optional separators?
        // Get the 32 hexadecimal digits in five groups separated by hyphens.
        std::string ToString() const;
        // Hash of current UUID
        inline std::size_t Hash() const { return boost::uuids::hash_value(uuid); };

        PwUuid& operator=(const PwUuid& u) {
            uuid = u.uuid;
        }

        PwUuid& operator=(PwUuid&& u) {
            std::swap(uuid, u.uuid);
        }

        friend bool operator <(const PwUuid& left, const PwUuid& right);
        friend bool operator ==(const PwUuid& left, const PwUuid& right);
        friend bool operator !=(const PwUuid& left, const PwUuid& right);
    private:
        // Never empty after constructor.
        uuid_t uuid;

        // Boost random generator.
        static uuid_generator_t uuid_generator;

        // Checks is the string length equals UuidSize.
        template <typename T> void checkSize(const T& s) const;
        template <typename It> void checkSize(It begin, It end) const;
    };

    std::ostream& operator <<(std::ostream& stream, const PwUuid& u);

    template <typename T>
    void PwUuid::checkSize(const T& s) const {
        if (s.size() != PwUuid::UuidSize) {
            std::ostringstream es;
            es << "value " << '"';
            es << std::hex << std::setfill('0');
            for (const auto i : s) {
                es << std::setw(2) << static_cast<unsigned>(static_cast<unsigned char>(i));
            }
            es << '"' << " has incorrect size: "
               << std::dec << s.size() << " != " << PwUuid::UuidSize;
            throw std::invalid_argument(es.str());
        }
    }

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
}

namespace std {
    template<>
    struct hash<keepasslib::PwUuid>
    {
        std::size_t operator()(const keepasslib::PwUuid& u) const {
            return u.Hash();
        }
    };
}
