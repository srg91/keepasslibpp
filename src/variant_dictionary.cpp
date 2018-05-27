#include "mem_util.hpp"
#include "variant_dictionary.hpp"

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
    return vd;
}

VariantDictionary VariantDictionary::Deserialize(const std::string& bytes) {
    VariantDictionary vd;
    return vd;
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
