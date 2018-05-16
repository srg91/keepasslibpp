#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>

class VariantDictionary {
public:
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
    static std::string Serialize(const VariantDictionary& vd);
    static VariantDictionary Deserialize(const std::string& pb);

    // TODO: Is this a code-duplication? Can we make this more simple?
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
    void SetString(const std::string& strName, std::string strValue);
    // TODO: Do we need byte array?
    const char* GetByteArray(const std::string& strName) const;
    void SetByteArray(const std::string& strName, const char* pbValue);
    // TODO: Think about byte array
private:
    const uint16_t VdVersion = 0x0100;
    const uint16_t VdmCritical = 0xff00;
    const uint16_t VdmInfo = 0x00ff;

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

//    struct Variant {
//        VdType type;
//        std::variant<bool, uint32_t, int32_t, int64_t, std::string> value;
//    };
    // TODO: C++17 feachure
    using VariantValue = std::variant<bool, int32_t, int64_t, uint32_t, uint64_t, std::string, const char*>;
    std::unordered_map<std::string, VariantValue> m_d;

    template<typename T> bool get(const std::string& strName, T& t) const;
    template<typename T> void set(const std::string& strName, const T& t);
};

template<typename T>
bool VariantDictionary::get(const std::string& strName, T& t) const {
    if (strName.empty()) return false;
    if (m_d.find(strName) == m_d.end()) return false;
    try {
        auto vv = m_d.at(strName);
        t = std::get<T>(vv);
    } catch (const std::out_of_range&) {
        // TODO: Is there way to catch two exceptions at once?
        return false;
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