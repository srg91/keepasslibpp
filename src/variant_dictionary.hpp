#pragma once

#include <boost/variant/variant.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace keepasslib {
    // TODO: Use template for container (in src use unordered, but map in tests)
    class VariantDictionary {
    public:
        using bytes = std::vector<std::uint8_t>;

        using key_type = std::string;
        using mapped_type = boost::variant<
            bool, std::int32_t, std::int64_t, std::uint32_t, std::uint64_t,
            std::string, bytes
        >;
        using value_type = std::pair<key_type, mapped_type>;

        bool Empty() const;
        std::size_t Size() const;
        void Erase(const std::string& key);
        void Clear();

        template <typename T>
        bool Get(const std::string& key, T& value) const;
        template <typename>
        bool Get(const std::string& key, std::string& value) const;
        template <typename>
        bool Get(const std::string& key, bytes& value) const;
        template <typename T>
        void Set(const std::string& key, const T& value);

        std::string Serialize() const;
        std::ostream& Serialize(std::ostream& stream) const;

        static VariantDictionary Deserialize(const std::string& bytes);
        static VariantDictionary Deserialize(std::istream& stream);

        mapped_type& operator[](const key_type& index);
        const mapped_type& operator[](const key_type& index) const;

        friend bool operator ==(const VariantDictionary& left, const VariantDictionary& right);
    };
}
