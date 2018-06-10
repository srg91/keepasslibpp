#pragma once

#include "mem_util.hpp"
#include "typedefs.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace keepasslibpp {

// TODO: Use template for container (in src use unordered, but map in tests)
class VariantDictionary {
public:
    // TODO: Rename this all stuff?
    using key_type = std::string;
    using mapped_type = std::variant<
        bool, std::int32_t, std::int64_t, std::uint32_t, std::uint64_t,
        std::string, type::ByteVector
    >;
    using value_type = std::pair<key_type, mapped_type>;
    using size_type = std::map<key_type, mapped_type>::size_type;
//
//    using iterator = std::map<key_type, mapped_type>::iterator;
//    using const_iterator = std::map<key_type, mapped_type>::const_iterator;

//    iterator begin() { return mDict.begin(); }
//    const_iterator begin() const { return mDict.begin(); }
//    iterator end() { return mDict.end(); }
//    const_iterator end() const { return mDict.end(); }

    VariantDictionary() {};
    VariantDictionary(VariantDictionary&& vd) : mDict(std::move(vd.mDict)) {};

    bool empty() const;
    size_type count(const key_type& key) const;
    std::size_t size() const;
    void erase(const std::string& key);
    void clear();

    template <typename T>
    bool get(const std::string& key, T& value) const noexcept ;

    template <typename T>
    void set(const std::string& key, const T& value);

    std::string serialize() const;
    std::ostream& serialize(std::ostream& stream) const;

    static VariantDictionary deserialize(std::istream& stream);
    static VariantDictionary deserialize(const std::string& bytes);

    mapped_type& operator [](const key_type& index);
    const mapped_type& operator [](const key_type& index) const;

    friend bool operator ==(const VariantDictionary& left,
                            const VariantDictionary& right);
private:
    static const uint16_t kVdVersion = 0x0100;
    static const uint16_t kVdmCritical = 0xff00;
//    static const uint16_t kVdmInfo = 0x00ff;

    // TODO: Rename
    enum class ValueTypeId : std::int8_t {
        None = 0x00,
        UInt32 = 0x04,
        UInt64 = 0x05,
        Bool = 0x08,
        Int32 = 0x0C,
        Int64 = 0x0D,
        String = 0x18,
        ByteArray = 0x42,
    };

    std::map<key_type, mapped_type> mDict;

    static void deserialize(std::istream& stream, VariantDictionary& vd);

    // TODO: Can we move it from header?
    struct guess_type_visitor
    {
        ValueTypeId operator () (bool) { return ValueTypeId::Bool; }
        ValueTypeId operator () (std::int32_t) { return ValueTypeId::Int32; }
        ValueTypeId operator () (std::int64_t) { return ValueTypeId::Int64; }
        ValueTypeId operator () (std::uint32_t) { return ValueTypeId::UInt32; }
        ValueTypeId operator () (std::uint64_t) { return ValueTypeId::UInt64; }
        ValueTypeId operator () (const std::string&)
            { return ValueTypeId::String; }
        ValueTypeId operator () (const type::ByteVector&)
            { return ValueTypeId::ByteArray; }
    };

    // TODO: Can we move it from header?
    struct write_value_visitor
    {
        std::ostream& stream;
        explicit write_value_visitor(std::ostream& stream) : stream(stream) {}

        template <
            typename T,
            typename = std::enable_if_t<std::is_trivial<T>::value>
        >
        std::size_t guess_size(T) const { return sizeof(T);}

        std::size_t guess_size(const std::string& value) const {
            return value.size();
        }

        std::size_t guess_size(const type::ByteVector& value) const {
            return value.size();
        }

        template <typename T>
        void operator ()(const T& value) const {
            auto value_size = static_cast<std::int32_t>(guess_size(value));
            mem_util::Write(stream, value_size);
            mem_util::Write(stream, value);
        }

        void operator ()(const type::ByteVector& value) const {
            auto value_size = static_cast<std::int32_t>(guess_size(value));
            mem_util::Write(stream, value_size);
            mem_util::Write(stream, value);
        }

    };
};

template <typename T>
bool VariantDictionary::get(const std::string& key, T& value) const noexcept {
    if (key.empty()) return false;
    try {
        auto vv = mDict.at(key);
        value = std::get<T>(vv);
    } catch(const std::exception&) {
        return false;
    }
    return true;
}

template <typename T>
void VariantDictionary::set(const std::string& key, const T& value) {
    if (key.empty()) return;
    mDict[key] = value;
}

}
