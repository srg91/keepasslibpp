#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

class VariantDictionary {
public:
    // TODO: Rename?
    using byte_array_type = std::vector<uint8_t>;

    VariantDictionary() {};
    VariantDictionary(const VariantDictionary& vd);

    bool Empty();
    size_t Size();
    bool Erase(const std::string& strName);

    // TODO: CopyTo
    // TODO: Add operator=

    // TODO: GetTypeOf

    // TODO: use stream?
    // TODO: const string?
    // TODO: make it general methods?
    // TODO: Make it stream
    static void Serialize(std::ostream& stream, const VariantDictionary& vd);
    static VariantDictionary Deserialize(std::istream& stream);

    // TODO: Is this a code-duplication? Can we make this more simple?
    // Make String and ByteArray as partial spec
    // Another make as template Get / Set
    bool GetBool(const std::string& strName, bool bDefault) const;
    void SetBool(const std::string& strName, bool bValue);
    int32_t GetInt32(const std::string& strName, int32_t uDefault) const;
    void SetInt32(const std::string& strName, int32_t uValue);
    int64_t GetInt64(const std::string& strName, int64_t uDefault) const;
    void SetInt64(const std::string& strName, int64_t uValue);
    uint32_t GetUInt32(const std::string& strName, uint32_t uDefault) const;
    void SetUInt32(const std::string& strName, uint32_t uValue);
    uint64_t GetUInt64(const std::string& strName, uint64_t uDefault) const;
    void SetUInt64(const std::string& strName, uint64_t uValue);
    std::string GetString(const std::string& strName) const;
    void SetString(const std::string& strName, const std::string& strValue);
    std::string GetByteArray(const std::string& strName) const;
    void SetByteArray(const std::string& strName, const std::string& value);
//    const char* GetByteArray(const std::string& strName) const;
//    void SetByteArray(const std::string& strName, const char* pbValue);
    // TODO: Think about byte array
private:
    static const uint16_t VdVersion = 0x0100;
    static const uint16_t VdmCritical = 0xff00;
    static const uint16_t VdmInfo = 0x00ff;

//    std::unordered_map<std::string, object? void* ?
    enum class VdType : int8_t {
        None = 0x00,
        UInt32 = 0x04,
        UInt64 = 0x05,
        Bool = 0x08,
        Int32 = 0x0C,
        Int64 = 0x0D,
        String = 0x18,
        ByteArray = 0x42,
    };

    // TODO: C++17 feature
    using VariantValue = std::variant<
        bool, int32_t, int64_t, uint32_t, uint64_t,
        std::string, byte_array_type
    >;
    std::unordered_map<std::string, VariantValue> m_d;

    template<typename T> bool get(const std::string& strName, T& t) const;
    template<typename T> void set(const std::string& strName, const T& t);

    template <typename T> static VdType guess_type(const T t);
    template <typename> static VdType guess_type(const VariantValue& vv);
    template <typename> static VdType guess_type(const std::string& t);
    template <typename> static VdType guess_type(const byte_array_type& t);

    // TODO: static? pair?
    static void serialize_value(std::ostream& stream, VdType type, const VariantValue& variant_value);
};

template<typename T>
bool VariantDictionary::get(const std::string& strName, T& t) const {
    if (strName.empty()) return false;
    try {
        auto vv = m_d.at(strName);
        t = std::get<T>(vv);
    } catch (const std::exception&) {
        // catch out_of_range and bad_variant_access
        return false;
    }
    return true;
}

template<typename T>
void VariantDictionary::set(const std::string& strName, const T& t) {
    if (strName.empty()) return;
    m_d[strName] = t;
}

template <typename T>
VariantDictionary::VdType VariantDictionary::guess_type(const T t) {
    // TODO: is_same_v is c++17 feature, use is_same::value another cases
    if constexpr (std::is_same_v<T, bool>)
        return VdType::Bool;
    else if constexpr (std::is_same_v<T, int32_t>)
        return VdType::Int32;
    else if constexpr (std::is_same_v<T, int64_t>)
        return VdType::Int64;
    else if constexpr (std::is_same_v<T, uint32_t>)
        return VdType::UInt32;
    else if constexpr (std::is_same_v<T, uint64_t>)
        return VdType::UInt64;
    else
        return VdType::None;
}

template <typename>
VariantDictionary::VdType VariantDictionary::guess_type(const std::string &t) {
    return VdType::String;
}

template <typename>
VariantDictionary::VdType VariantDictionary::guess_type(const VariantDictionary::byte_array_type& t) {
    return VdType::ByteArray;
}

template <typename>
VariantDictionary::VdType VariantDictionary::guess_type(const VariantDictionary::VariantValue& vv) {
    if (std::holds_alternative<byte_array_type>(vv)) {
        return VdType::ByteArray;
    } else if (std::holds_alternative<std::string>(vv)) {
        return VdType::String;
    } else if (std::holds_alternative<uint64_t>(vv)) {
        return VdType::UInt64;
    } else if (std::holds_alternative<int64_t>(vv)) {
        return VdType::Int64;
    } else if (std::holds_alternative<uint32_t>(vv)) {
        return VdType::UInt32;
    } else if (std::holds_alternative<int32_t>(vv)) {
        return VdType::Int32;
    } else if (std::holds_alternative<bool>(vv)) {
        return VdType::Bool;
    } else {
        return VdType::None;
    }
}
