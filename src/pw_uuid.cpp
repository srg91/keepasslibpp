//
// Created by srg91 on 16.04.2018.
//

#include "pw_uuid.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <sstream>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace std;

PwUuid::PwUuid() {
    createNew();
}

PwUuid::PwUuid(const PwUuid& u) {
    bytes = u.bytes;
}

PwUuid::PwUuid(PwUuid&& u) {
    swap(bytes, u.bytes);
}

PwUuid::PwUuid(const string& s) {
    checkSize(s);
    bytes = s;
}

PwUuid::PwUuid(string&& s) {
    checkSize(s);
    swap(bytes, s);
}

const PwUuid PwUuid::Zero = string(16, 0);

string PwUuid::Bytes() const {
    return bytes;
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
    // TODO: cache generator in static attr?
    auto uuid = boost::uuids::random_generator()();
    bytes = string(uuid.begin(), uuid.end());
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
