#include "byte_vector.hpp"
#include "exception.hpp"
#include "memory_util.hpp"
#include "variant_dictionary.hpp"

#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <variant>

using namespace keepasspp;

namespace keepasspp {

bool VariantDictionary::empty() const {
    return std::empty(dict);
}

VariantDictionary::size_type VariantDictionary::count(
        const VariantDictionary::key_type& key) const {
    return dict.count(key);
}

std::size_t VariantDictionary::size() const {
    return std::size(dict);
}

void VariantDictionary::erase(const std::string& key) {
    dict.erase(key);
}

void VariantDictionary::clear() {
    dict.clear();
}

std::ostream& VariantDictionary::serialize(std::ostream& stream) const noexcept {
    MemoryUtil::write(stream, vdVersion);

    for (const auto& [key, value] : dict) {
        // write type id
        ValueTypeId value_type = std::visit(GuessTypeVisitor(), value);
        MemoryUtil::write(stream, static_cast<std::uint8_t>(value_type));

        // write key
        MemoryUtil::write(stream, static_cast<std::int32_t>(std::size(key)));
        MemoryUtil::write(stream, key);

        // write value
        std::visit(WriteValueVisitor(stream), value);
    }

    MemoryUtil::write(stream, static_cast<std::uint8_t>(ValueTypeId::None));
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
    } catch (const exception::NotEnoughBytesError&) {
        throw exception::FileCorruptedError("cannot read enough bytes");
    }
    return vd;
}

void VariantDictionary::deserialize(std::istream& stream,
                                    VariantDictionary& vd) {
    auto version = MemoryUtil::read<std::uint16_t>(stream);
    if ((version & vdmCritical) > (vdVersion & vdmCritical))
        throw exception::NewVersionRequiredError(
            "newer variant dictionary version found");

    for (;;) {
        if (stream.peek() < 0)
            throw exception::FileCorruptedError("unexpected symbol");
        auto value_type = static_cast<ValueTypeId>(
            MemoryUtil::read<std::int8_t>(stream));
        if (value_type == ValueTypeId::None) break;

        auto key_size = MemoryUtil::read<std::int32_t>(stream);
        auto key = MemoryUtil::read<std::string>(
            stream, static_cast<std::size_t>(key_size));

        auto value_size = static_cast<std::size_t>(
            MemoryUtil::read<std::int32_t>(stream));
        mapped_type value;
        switch (value_type) {
            case ValueTypeId::Bool:
                value = MemoryUtil::read<bool>(stream);
                break;
            case ValueTypeId::Int32:
                value = MemoryUtil::read<std::int32_t>(stream);
                break;
            case ValueTypeId::Int64:
                value = MemoryUtil::read<std::int64_t>(stream);
                break;
            case ValueTypeId::UInt32:
                value = MemoryUtil::read<std::uint32_t>(stream);
                break;
            case ValueTypeId::UInt64:
                value = MemoryUtil::read<std::uint64_t>(stream);
                break;
            case ValueTypeId::String:
                value = MemoryUtil::read<std::string>(stream, value_size);
                break;
            case ValueTypeId::ByteArray:
                value = MemoryUtil::read<ByteVector>(stream, value_size);
                break;
            default:
                throw exception::NewVersionRequiredError(
                    "unknown variant dictionary type");
        }
        vd[key] = value;

        if (!stream)
            throw exception::FileCorruptedError("unexpected end on stream");
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
