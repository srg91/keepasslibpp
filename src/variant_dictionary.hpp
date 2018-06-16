#pragma once

#include "byte_vector.hpp"
#include "memory_util.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace keepasspp {

// TODO: Use template for container (in src use unordered, but map in tests)
class VariantDictionary {
public:
    // TODO: Rename this all stuff?
    using key_type = std::string;
    using mapped_type = std::variant<
        bool, std::int32_t, std::int64_t, std::uint32_t, std::uint64_t,
        std::string, ByteVector
    >;
    using value_type = std::pair<key_type, mapped_type>;
    using size_type = std::map<key_type, mapped_type>::size_type;

    // TODO: Use '= default' to define a trivial default constructor
    VariantDictionary() {};
    VariantDictionary(VariantDictionary&& vd) noexcept
        : dict(std::move(vd.dict)) {};

    bool empty() const;
    size_type count(const key_type& key) const;
    std::size_t size() const;
    void erase(const std::string& key);
    void clear();

    template <typename T>
    T get(const std::string& key) const;

    template <typename T>
    bool get(const std::string& key, T& value) const noexcept;

    template <typename T>
    void set(const std::string& key, const T& value);

    std::string serialize() const noexcept;
    std::ostream& serialize(std::ostream& stream) const noexcept;

    static VariantDictionary deserialize(std::istream& stream);
    static VariantDictionary deserialize(const std::string& bytes);

    mapped_type& operator [] (const key_type& index);
    const mapped_type& operator [] (const key_type& index) const;

    friend bool operator == (const VariantDictionary& left,
                             const VariantDictionary& right);
private:
    static const uint16_t vdVersion = 0x0100;
    static const uint16_t vdmCritical = 0xff00;
//    static const uint16_t vdmInfo = 0x00ff;

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

    std::map<key_type, mapped_type> dict;

    static void deserialize(std::istream& stream, VariantDictionary& vd);

    // TODO: Can we move it from header?
    struct GuessTypeVisitor
    {
        ValueTypeId operator () (bool) { return ValueTypeId::Bool; }
        ValueTypeId operator () (std::int32_t) { return ValueTypeId::Int32; }
        ValueTypeId operator () (std::int64_t) { return ValueTypeId::Int64; }
        ValueTypeId operator () (std::uint32_t) { return ValueTypeId::UInt32; }
        ValueTypeId operator () (std::uint64_t) { return ValueTypeId::UInt64; }
        ValueTypeId operator () (const std::string&)
            { return ValueTypeId::String; }
        ValueTypeId operator () (const ByteVector&)
            { return ValueTypeId::ByteArray; }
    };

    // TODO: Can we move it from header?
    struct WriteValueVisitor
    {
        std::ostream& stream;
        explicit WriteValueVisitor(std::ostream& stream) : stream(stream) {}

        template <typename T>
        std::size_t guess_size(const T& value) const {
            if constexpr (std::is_trivial_v<T>) {
                return sizeof(T);
            } else {
                return std::size(value);
            }
        }

        template <typename T>
        void operator ()(const T& value) const {
            auto value_size = static_cast<std::int32_t>(guess_size(value));
            MemoryUtil::write(stream, value_size);
            MemoryUtil::write(stream, value);
        }

    };
};

template <typename T>
T VariantDictionary::get(const std::string& key) const {
    // TODO: Add keepasspp errors
    try {
        auto vv = dict.at(key);
        return std::get<T>(vv);
    } catch (const std::exception&) {
        throw exception::ArgumentIsNullError(key);
    }
}

template <typename T>
bool VariantDictionary::get(const std::string& key, T& value) const noexcept {
    if (std::empty(key)) return false;
    try {
        value = this->get<T>(key);
    } catch(const exception::ArgumentIsNullError&) {
        return false;
    }
    return true;
}

template <typename T>
void VariantDictionary::set(const std::string& key, const T& value) {
    if (std::empty(key)) return;
    dict[key] = value;
}

}
