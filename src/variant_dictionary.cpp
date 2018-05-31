#include "exception.hpp"
#include "mem_util.hpp"
#include "variant_dictionary.hpp"
#include "typedefs.hpp"

#include <cstdint>
#include <iostream>
#include <sstream>

using namespace keepasslib;

bool VariantDictionary::Empty() const {
    return dict.empty();
}

std::size_t VariantDictionary::Size() const {
    return dict.size();
}

void VariantDictionary::Erase(const std::string& key) {
    dict.erase(key);
}

void VariantDictionary::CopyTo(VariantDictionary& vd) {
    vd.dict = dict;
}

void VariantDictionary::Clear() {
    dict.clear();
}

std::ostream& VariantDictionary::Serialize(std::ostream& stream) const {
    mem_util::Write<uint16_t>(stream, vd_version);

    for (const auto& item : dict) {
        // write type id
        auto gv = guess_type_visitor();
        serialization_type value_type = item.second.apply_visitor(gv);
        mem_util::Write(stream, static_cast<std::uint8_t>(value_type));

        // write key
        mem_util::Write(stream, static_cast<std::int32_t>(item.first.size()));
        mem_util::Write(stream, item.first);

        // write value
        auto wv = write_value_visitor(stream);
        item.second.apply_visitor(wv);
    }

    mem_util::Write(stream, static_cast<std::uint8_t>(serialization_type::None));
}

std::string VariantDictionary::Serialize() const {
    std::ostringstream stream;
    VariantDictionary::Serialize(stream);
    return stream.str();
}

VariantDictionary VariantDictionary::Deserialize(std::istream& stream) {
    VariantDictionary vd;
    try {
        deserialize(stream, vd);
    } catch (std::length_error&) {
        // TODO: Add error message?
        throw exception::FileCorruptedError();
    }
    return vd;
}

void VariantDictionary::deserialize(std::istream& stream, keepasslib::VariantDictionary& vd) {
    auto version = mem_util::Read<std::uint16_t>(stream);
    if ((version & vdm_critical) > (vd_version & vdm_critical))
        // TODO: Add error message?
        throw exception::NewVersionRequiredError();

    for (;;) {
        // TODO: Add error message?
        if (stream.peek() < 0) throw exception::FileCorruptedError();
        auto value_type = static_cast<serialization_type>(mem_util::Read<std::int8_t>(stream));
        if (value_type == serialization_type::None) break;

        auto key_size = mem_util::Read<std::int32_t>(stream);
        auto key = mem_util::Read<std::string>(stream, static_cast<std::size_t>(key_size));
        auto value_size = static_cast<std::size_t>(mem_util::Read<std::int32_t>(stream));

        mapped_type value;
        switch (value_type) {
            case serialization_type::Bool:
                value = mem_util::Read<bool>(stream);
                break;
            case serialization_type::Int32:
                value = mem_util::Read<std::int32_t>(stream);
                break;
            case serialization_type::Int64:
                value = mem_util::Read<std::int64_t>(stream);
                break;
            case serialization_type::UInt32:
                value = mem_util::Read<std::uint32_t>(stream);
                break;
            case serialization_type::UInt64:
                value = mem_util::Read<std::uint64_t>(stream);
                break;
            case serialization_type::String:
                value = mem_util::Read<std::string>(stream, value_size);
                break;
            case serialization_type::ByteArray:
                value = mem_util::Read<types::bytes>(stream, value_size);
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

VariantDictionary VariantDictionary::Deserialize(const std::string& bytes) {
    std::istringstream stream(bytes);
    return VariantDictionary::Deserialize(stream);
}

VariantDictionary::mapped_type& VariantDictionary::operator [](const VariantDictionary::key_type& index) {
    return dict[index];
}

const VariantDictionary::mapped_type& VariantDictionary::operator [](const keepasslib::VariantDictionary::key_type& index) const {
    return dict.at(index);
}

namespace keepasslib {
    bool operator ==(const VariantDictionary& left, const VariantDictionary& right) {
        return left.dict == right.dict;
    }
}
