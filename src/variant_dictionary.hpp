#pragma once

#include "mem_util.hpp"
#include "typedefs.hpp"

#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace keepasslib {
    // TODO: Use template for container (in src use unordered, but map in tests)
    class VariantDictionary {
    public:
        using key_type = std::string;
        using mapped_type = boost::variant<
            bool, std::int32_t, std::int64_t, std::uint32_t, std::uint64_t,
            std::string, types::bytes
        >;
        using value_type = std::pair<key_type, mapped_type>;

        using size_type = std::map<key_type, mapped_type>::size_type;

        using iterator = std::map<key_type, mapped_type>::iterator;
        using const_iterator = std::map<key_type, mapped_type>::const_iterator;

        iterator begin() { return dict.begin(); }
        const_iterator begin() const { return dict.begin(); }
        iterator end() { return dict.end(); }
        const_iterator end() const { return dict.end(); }

        VariantDictionary() {};
        VariantDictionary(VariantDictionary&& vd) : dict(std::move(vd.dict)) {};

        bool Empty() const;
        size_type Count(const key_type& key) const;
        std::size_t Size() const;
        void Erase(const std::string& key);
        void Clear();

        template <typename T>
        bool Get(const std::string& key, T& value) const noexcept ;

        template <typename T>
        void Set(const std::string& key, const T& value);

        std::string Serialize() const;
        std::ostream& Serialize(std::ostream& stream) const;

        static VariantDictionary Deserialize(std::istream& stream);
        static VariantDictionary Deserialize(const std::string& bytes);

        mapped_type& operator [](const key_type& index);
        const mapped_type& operator [](const key_type& index) const;

        friend bool operator ==(const VariantDictionary& left, const VariantDictionary& right);
    private:
        static const uint16_t vd_version = 0x0100;
        static const uint16_t vdm_critical = 0xff00;
//        static const uint16_t vdm_info = 0x00ff;

        enum class serialization_type : std::int8_t {
            None = 0x00,
            UInt32 = 0x04,
            UInt64 = 0x05,
            Bool = 0x08,
            Int32 = 0x0C,
            Int64 = 0x0D,
            String = 0x18,
            ByteArray = 0x42,
        };

        std::map<key_type, mapped_type> dict;

        static void deserialize(std::istream& stream, VariantDictionary& vd);

        struct guess_type_visitor : public boost::static_visitor<serialization_type>
        {
            // TODO: Or many small operators?
            template <typename T>
            serialization_type operator ()(const T& value) const {
                if (std::is_same<T, bool>::value) {
                    return serialization_type::Bool;
                } else if (std::is_same<T, std::int32_t>::value) {
                    return serialization_type::Int32;
                } else if (std::is_same<T, std::int64_t>::value) {
                    return serialization_type::Int64;
                } else if (std::is_same<T, std::uint32_t>::value) {
                    return serialization_type::UInt32;
                } else if (std::is_same<T, std::uint64_t>::value) {
                    return serialization_type::UInt64;
                } else if (std::is_same<T, std::string>::value) {
                    return serialization_type::String;
                } else if (std::is_same<T, types::bytes>::value) {
                    return serialization_type::ByteArray;
                } else {
                    return serialization_type::None;
                }
            }
        };

        struct write_value_visitor : public boost::static_visitor<>
        {
            std::ostream& stream;
            explicit write_value_visitor(std::ostream& stream) : stream(stream) {}

            // TODO: Rework this!!
            template <typename T, typename = std::enable_if_t<std::is_trivial<T>::value>>
            std::size_t guess_size(const T& value) const {
                return sizeof(T);
            }

            // TODO: Rework this!!
            std::size_t guess_size(const std::string& value) const {
                return value.size();
            }

            // TODO: Rework this!!
            std::size_t guess_size(const types::bytes& value) const {
                return value.size();
            }

            template <typename T>
            void operator ()(const T& value) const {
                auto value_size = static_cast<std::int32_t>(guess_size(value));
                mem_util::Write(stream, value_size);
                mem_util::Write(stream, value);
            }

            void operator ()(const types::bytes& value) const {
                auto value_size = static_cast<std::int32_t>(guess_size(value));
                mem_util::Write(stream, value_size);
                mem_util::Write(stream, value);
            }

        };
    };

    template <typename T>
    bool VariantDictionary::Get(const std::string& key, T& value) const noexcept {
        if (key.empty()) return false;
        try {
            auto vv = dict.at(key);
            value = boost::get<T>(vv);
        } catch(const std::exception&) {
            return false;
        }
        return true;
    }

    template <typename T>
    void VariantDictionary::Set(const std::string& key, const T& value) {
        if (key.empty()) return;
        dict[key] = value;
    }
}
