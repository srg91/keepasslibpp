#include "variant_dictionary.hpp"
#include "mem_util.hpp"

// TODO: Should I copy libs already defined in header?
#include <cstdint>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
using namespace keepasslib;

VariantDictionary::VariantDictionary(const VariantDictionary &vd) {
    // TODO: make begin / end public?
    m_d.insert(
        make_move_iterator(vd.m_d.begin()),
        make_move_iterator(vd.m_d.end())
    );
    // TODO: use map merge from c++17?
//    m_d.merge(vd);
}

bool VariantDictionary::Empty() {
    return m_d.empty();
}

size_t VariantDictionary::Size() {
    return m_d.size();
}

bool VariantDictionary::Erase(const string& strName) {
    m_d.erase(strName);
}

void VariantDictionary::Serialize(std::ostream& stream, const VariantDictionary &vd) {
    mem_util::Write<uint16_t>(stream, VdVersion);

    // TODO: C++17 feature
    for (const auto& [key, value_variant] : vd.m_d) {
        // TODO: Make it MOOOOOOOOOOOOOOOORE simpler
        VdType type = guess_type<VariantDictionary>(value_variant);

        mem_util::Write(stream, static_cast<uint8_t>(type));
        mem_util::Write<int32_t>(stream, key.size());
        mem_util::Write<string>(stream, key);

        serialize_value(stream, type, value_variant);
    }
    mem_util::Write<uint8_t>(stream, static_cast<uint8_t>(VdType::None));
}

void VariantDictionary::serialize_value(std::ostream& stream, VdType type, const VariantValue& variant_value) {
    // TODO: Make it simpler
    switch (type) {
        case VdType::Bool: {
            auto value = std::get<bool>(variant_value);
            int32_t size = sizeof(value);
            mem_util::Write(stream, size);
            mem_util::Write(stream, value);
            return;
        }
        case VdType::Int32: {
            auto value = std::get<int32_t>(variant_value);
            int32_t size = sizeof(value);
            mem_util::Write(stream, size);
            mem_util::Write(stream, value);
            return;
        }
        case VdType::Int64: {
            auto value = std::get<int64_t>(variant_value);
            int32_t size = sizeof(value);
            mem_util::Write(stream, size);
            mem_util::Write(stream, value);
            return;
        }
        case VdType::UInt32: {
            auto value = std::get<uint32_t>(variant_value);
            int32_t size = sizeof(value);
            mem_util::Write(stream, size);
            mem_util::Write(stream, value);
            return;
        }
        case VdType::UInt64: {
            auto value = std::get<uint64_t>(variant_value);
            int32_t size = sizeof(value);
            mem_util::Write(stream, size);
            mem_util::Write(stream, value);
            return;
        }
        case VdType::String: {
            auto& value = std::get<std::string>(variant_value);
            auto size = static_cast<int32_t>(value.size());
            mem_util::Write(stream, size);
            mem_util::Write(stream, value);
            return;
        }
        case VdType::ByteArray: {
            // TODO: make it simplier
            auto& value = std::get<byte_array_type>(variant_value);
            auto size = static_cast<int32_t>(value.size());
            mem_util::Write(stream, size);
            mem_util::Write(stream, string(value.begin(), value.end()));
            return;
        }
    }
}

VariantDictionary VariantDictionary::Deserialize(istream& stream) {
    VariantDictionary vd;

    auto uVersion = mem_util::Read<uint16_t>(stream);
    if ((uVersion & VdmCritical) > (VdVersion & VdmCritical))
        // TODO: Make it FileNewVerReq
        throw invalid_argument("FileNewVerReq");

    while (true) {
        auto type = static_cast<VdType>(mem_util::Read<uint8_t>(stream));
        // if (type < 0) throw EndOfStreamException
        if (type == VdType::None) break;

//        auto cbName = static_cast<size_t>(mem_util::Read<int32_t>(stream));
//        // TODO: static_cast here?
//        string strName = mem_util::ReadString(stream, cbName);
//        if (strName.size() != cbName) throw invalid_argument("EndOfStreamException") ;
//
//        auto cbValue = static_cast<size_t>(mem_util::Read<int32_t>(stream));
//
//        // more simple
//        switch (type) {
//            case VdType::Bool:
//                auto value = mem_util::Read<bool>(stream);
//
//                break;
//        }

    }
    return vd;
}

bool VariantDictionary::GetBool(const string &strName, bool bDefault) const {
    bool b = false;
    if (this->get<bool>(strName, b)) return b;
    return bDefault;
}

void VariantDictionary::SetBool(const string &strName, bool bValue) {
    this->set<bool>(strName, bValue);
}

int32_t VariantDictionary::GetInt32(const string &strName, int32_t uDefault) const {
    int32_t u = 0;
    if (this->get<int32_t>(strName, u)) return u;
    return uDefault;
}

void VariantDictionary::SetInt32(const string &strName, int32_t uValue) {
    this->set<int32_t>(strName, uValue);
}

int64_t VariantDictionary::GetInt64(const string &strName, int64_t uDefault) const {
    int64_t u = 0;
    if (this->get<int64_t>(strName, u)) return u;
    return uDefault;
}

void VariantDictionary::SetInt64(const string &strName, int64_t uValue) {
    this->set<int64_t>(strName, uValue);
}

uint32_t VariantDictionary::GetUInt32(const string &strName, uint32_t uDefault) const {
    uint32_t u = 0;
    if (this->get<uint32_t>(strName, u)) return u;
    return uDefault;
}

void VariantDictionary::SetUInt32(const string &strName, uint32_t uValue) {
    this->set<uint32_t>(strName, uValue);
}

uint64_t VariantDictionary::GetUInt64(const string &strName, uint64_t uDefault) const {
    uint64_t u = 0;
    if (this->get<uint64_t>(strName, u)) return u;
    return uDefault;
}

void VariantDictionary::SetUInt64(const string &strName, uint64_t uValue) {
    this->set<uint64_t>(strName, uValue);
}

string VariantDictionary::GetString(const string &strName) const {
    string s;
    if (this->get<string>(strName, s)) return s;
    return "";
}

void VariantDictionary::SetString(const string &strName, const std::string& strValue) {
    this->set<string>(strName, strValue);
}

std::string VariantDictionary::GetByteArray(const std::string &strName) const {
    VariantDictionary::byte_array_type v;
    if (this->get<VariantDictionary::byte_array_type>(strName, v)) return string(v.begin(), v.end());
    return "";
}

void VariantDictionary::SetByteArray(const std::string &strName, const std::string& value) {
    this->set<VariantDictionary::byte_array_type>(strName, VariantDictionary::byte_array_type(value.begin(), value.end()));
}
