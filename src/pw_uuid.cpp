//
// Created by srg91 on 16.04.2018.
//

#include "pw_uuid.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

using namespace std;

PwUuid::PwUuid(bool bCreateNew) {
    if (bCreateNew) createNew(); else setZero();
}

const PwUuid PwUuid::Zero = PwUuid(false);

string PwUuid::UuidBytes() {
    return m_pbUuid;
}

void PwUuid::createNew() {
    setZero();

    // TODO: Add correct UUID generation (with boost or smt else)
    auto seed = chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    uniform_int_distribution<mt19937::result_type> dist16(0, 15);
    for (auto& c : m_pbUuid) {
        c = static_cast<char>(dist16(gen));
    }
}

void PwUuid::setZero() {
    m_pbUuid = string(UuidSize, 0);
}

bool operator ==(const PwUuid& left, const PwUuid& right) {
    return left.m_pbUuid == right.m_pbUuid;
}

bool operator !=(const PwUuid& left, const PwUuid& right) {
    return !(left == right);
}

// TODO: Remove it, I think we will need to work only with bytes and don't need to print it
ostream& operator <<(ostream &o, const PwUuid& u) {
    const auto flags = o.flags();
    o << setfill('0') << hex;
    for (auto c : u.m_pbUuid) {
        auto i = static_cast<unsigned>(static_cast<unsigned char>(c));
        o << setw(2) << i;
    }
    o.flags(flags);
    return o;
}
