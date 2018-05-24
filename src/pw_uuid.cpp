//
// Created by srg91 on 16.04.2018.
//

#include "pw_uuid.hpp"

#include <array>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <sstream>
#include <string>
#include <utility>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

using namespace std;

PwUuid::PwUuid() {
    createNew();
}

PwUuid::PwUuid(const PwUuid& u) {
    bytes = u.bytes;
}

PwUuid::PwUuid(PwUuid&& u) {
    bytes.swap(u.bytes);
}

PwUuid::PwUuid(const string& s) {
    checkSize(s);
    copy(s.begin(), s.end(), bytes.begin());
}

PwUuid::PwUuid(string&& s) {
    checkSize(s);
    move(s.begin(), s.end(), bytes.begin());
}

boost::uuids::random_generator PwUuid::uuid_generator = boost::uuids::random_generator();
const PwUuid PwUuid::Zero = string(16, 0);

string PwUuid::Bytes() const {
    return string(bytes.begin(), bytes.end());
//    return bytes;
}

string PwUuid::ToString() const {
    ostringstream s;
    s << setfill('0') << hex;
    for (size_t i = 0; i < bytes.size(); i++) {
        switch (i) {
            case 4:
            case 4 + 2:
            case 4 + 2 + 2:
            case 4 + 2 + 2 + 2:
                s << '-';
            default:
                s << setw(2) << static_cast<unsigned>(bytes[i]);
        }
    }
    return s.str();
}

void PwUuid::createNew() {
    // TODO: Add length check
    auto uuid = PwUuid::uuid_generator();
    copy(uuid.begin(), uuid.end(), bytes.begin());
}

void PwUuid::checkSize(const string& s) const {
    if (s.size() != PwUuid::UuidSize) {
        ostringstream es;
        // TODO: implement print hex string
        es << "string " << '"' << s << '"' << " has incorrect size: "
           << s.size() << " != " << PwUuid::UuidSize;
        throw invalid_argument(es.str());
    }
}

ostream& operator <<(ostream& stream, const PwUuid& u) {
    return stream << u.ToString();
}

bool operator <(const PwUuid& left, const PwUuid& right) {
    return left.bytes < right.bytes;
}

bool operator ==(const PwUuid& left, const PwUuid& right) {
    return left.bytes == right.bytes;
}

bool operator !=(const PwUuid& left, const PwUuid& right) {
    return !(left == right);
}
