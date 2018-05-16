#include "variantdictionary.hpp"
using namespace std;

bool VariantDictionary::empty() {
    return m_d.empty();
}

size_t VariantDictionary::size() {
    return m_d.size();
}

bool VariantDictionary::erase(const std::string& strName) {
    m_d.erase(strName);
}

uint32_t VariantDictionary::GetUInt32(const std::string &strName, uint32_t uDefault) {
    uint32_t u = 0;
    if (this->get<uint32_t>(strName, u)) return u;
    return uDefault;
}

void VariantDictionary::SetUInt32(const std::string &strName, uint32_t uValue) {
    this->set<uint32_t>(strName, uValue);
}