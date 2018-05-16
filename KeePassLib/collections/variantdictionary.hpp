#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>


class VariantDictionary {
    const uint16_t VdVersion = 0x0100;
    const uint16_t VdmCritical = 0xff00;
    const uint16_t VdmInfo = 0x00ff;

//    std::unordered_map<std::string, object? void* ?
    enum class VdType : int8_t {
        None = 0x00,
        UInt32 = 0x04,
        Bool = 0x08,
        Int32 = 0x0C,
        Int64 = 0x0D,
        String = 0x18,
        ByteArray = 0x42,
    };

//    struct Variant {
//        VdType type;
//        std::variant<bool, uint32_t, int32_t, int64_t, std::string> value;
//    };
    using VariantValue = std::variant<bool, uint32_t, int32_t, int64_t, std::string>;
    std::unordered_map<std::string, VariantValue> m_d;

    template<typename T> bool get(const std::string& strName, T& t) const;
    template<typename T> void set(const std::string& strName, const T& t);
public:
    bool empty();
    size_t size();
    bool erase(const std::string& strName);

    uint32_t GetUInt32(const std::string& strName, uint32_t uDefault);
    void SetUInt32(const std::string& strName, uint32_t uValue);
};

template<typename T>
bool VariantDictionary::get(const std::string& strName, T& t) const {
    if (strName.empty()) return false;
    if (m_d.find(strName) == m_d.end()) return false;
    auto vv = m_d.at(strName);
    try {
        t = std::get<T>(vv);
    } catch (const std::bad_variant_access&) {
        return false;
    }
    return true;
}

template<typename T>
void VariantDictionary::set(const std::string& strName, const T& t) {
    if (strName.empty()) return;
    m_d[strName] = t;
}