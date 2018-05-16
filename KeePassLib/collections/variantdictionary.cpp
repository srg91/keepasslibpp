#include "variantdictionary.hpp"

// TODO: Should I copy libs already defined in header?
#include <cstdint>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

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

string VariantDictionary::Serialize(const VariantDictionary &vd) {
    ostringstream stream;

}

VariantDictionary VariantDictionary::Deserialize(const std::string &pb) {

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

void VariantDictionary::SetString(const string &strName, std::string strValue) {
    this->set<string>(strName, strValue);
}

const char* VariantDictionary::GetByteArray(const string &strName) const {
    const char* pb;
    if (this->get<const char*>(strName, pb)) return pb;
    return nullptr;
}

void VariantDictionary::SetByteArray(const std::string &strName, const char *pbValue) {
    this->set<const char*>(strName, pbValue);
}
