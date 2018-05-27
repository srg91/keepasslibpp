#pragma once

#include <boost/variant/variant.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

namespace keepasslib {
    // TODO: Rename (like SerelializationTypeMap or smth else)
    // TODO: And move under VD class
    enum class VDST : std::int8_t {
        None = 0x00,
        UInt32 = 0x04,
        UInt64 = 0x05,
        Bool = 0x08,
        Int32 = 0x0C,
        Int64 = 0x0D,
        String = 0x18,
        ByteArray = 0x42,
    };

    // TODO: Use template for container (in src use unordered, but map in tests)
    class VariantDictionary {
    public:
        using key_type = std::string;
        using mapped_type = boost::variant<
            bool, std::int32_t, std::int64_t, std::uint32_t, std::uint64_t,
            std::string // TODO: byte array type
        >;
        using value_type = std::pair<key_type, mapped_type>;

        bool Empty() const;
        std::size_t Size() const;
        bool Erase(const std::string& key);

        template <typename T>
        T Get(const std::string& key) const;
        template <typename T>
        T Set(const std::string& key, const T& value);

        std::string Serialize() const;
        std::ostream& Serialize(std::ostream& stream) const;

        static VariantDictionary Deserialize(const std::string& bytes);
        static VariantDictionary Deserialize(std::istream& stream);

        mapped_type& operator[](const key_type& index);
        const mapped_type& operator[](const key_type& index) const;

        friend bool operator ==(const VariantDictionary& left, const VariantDictionary& right);
    };
}
