#include "exception.hpp"
#include "mem_util.hpp"
#include "variant_dictionary.hpp"
#include "typedefs.hpp"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <variant>

using namespace keepasslibpp;

namespace keepasslibpp {

bool VariantDictionary::empty() const {
    return dict.empty();
}

VariantDictionary::size_type VariantDictionary::count(
        const VariantDictionary::key_type& key) const {
    return dict.count(key);
}

std::size_t VariantDictionary::size() const {
    return dict.size();
}

void VariantDictionary::erase(const std::string& key) {
    dict.erase(key);
}

void VariantDictionary::clear() {
    dict.clear();
}

std::ostream& VariantDictionary::serialize(std::ostream& stream) const noexcept {
    mem_util::Write<uint16_t>(stream, vdVersion);

    for (const auto& [key, value] : dict) {
        // write type id
        ValueTypeId value_type = std::visit(GuessTypeVisitor(), value);
        mem_util::Write(stream, static_cast<std::uint8_t>(value_type));

        // write key
        mem_util::Write(stream, static_cast<std::int32_t>(key.size()));
        mem_util::Write(stream, key);

        // write value
        std::visit(WriteValueVisitor(stream), value);
    }

    mem_util::Write(stream, static_cast<std::uint8_t>(ValueTypeId::None));
}

std::string VariantDictionary::serialize() const noexcept {
    std::ostringstream stream;
    VariantDictionary::serialize(stream);
    return stream.str();
}

VariantDictionary VariantDictionary::deserialize(std::istream& stream) {
    VariantDictionary vd;
    try {
        deserialize(stream, vd);
    } catch (std::length_error&) {
        // TODO: Add error message?
        throw exception::FileCorruptedError();
    }
    return vd;
}

void VariantDictionary::deserialize(std::istream& stream,
                                    VariantDictionary& vd) {
    auto version = mem_util::Read<std::uint16_t>(stream);
    if ((version & vdmCritical) > (vdVersion & vdmCritical))
        // TODO: Add error message?
        throw exception::NewVersionRequiredError();

    for (;;) {
        // TODO: Add error message?
        if (stream.peek() < 0) throw exception::FileCorruptedError();
        auto value_type = static_cast<ValueTypeId>(
            mem_util::Read<std::int8_t>(stream));
        if (value_type == ValueTypeId::None) break;

        auto key_size = mem_util::Read<std::int32_t>(stream);
        auto key = mem_util::Read<std::string>(
            stream, static_cast<std::size_t>(key_size));

        auto value_size = static_cast<std::size_t>(
            mem_util::Read<std::int32_t>(stream));
        mapped_type value;
        switch (value_type) {
            case ValueTypeId::Bool:
                value = mem_util::Read<bool>(stream);
                break;
            case ValueTypeId::Int32:
                value = mem_util::Read<std::int32_t>(stream);
                break;
            case ValueTypeId::Int64:
                value = mem_util::Read<std::int64_t>(stream);
                break;
            case ValueTypeId::UInt32:
                value = mem_util::Read<std::uint32_t>(stream);
                break;
            case ValueTypeId::UInt64:
                value = mem_util::Read<std::uint64_t>(stream);
                break;
            case ValueTypeId::String:
                value = mem_util::Read<std::string>(stream, value_size);
                break;
            case ValueTypeId::ByteArray:
                value = mem_util::Read<type::ByteVector>(stream, value_size);
                break;
            default:
                // TODO: Add error message?
                throw exception::NewVersionRequiredError();
        }
        vd[key] = value;

        // TODO: Add error message?
        if (!stream) throw exception::FileCorruptedError();
    }
}

VariantDictionary VariantDictionary::deserialize(const std::string& bytes) {
    std::istringstream stream(bytes);
    return VariantDictionary::deserialize(stream);
}

VariantDictionary::mapped_type& VariantDictionary::operator [] (
        const VariantDictionary::key_type& index) {
    return dict[index];
}

const VariantDictionary::mapped_type& VariantDictionary::operator [] (
        const VariantDictionary::key_type& index) const {
    return dict.at(index);
}

bool operator == (const VariantDictionary& left,
                  const VariantDictionary& right) {
    return left.dict == right.dict;
}

}
