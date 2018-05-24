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
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>

using namespace std;

//PwUuid::PwUuid() {
//    createNew();
//}

PwUuid::PwUuid(const PwUuid& u) {
    uuid = u.uuid;
}

PwUuid::PwUuid(PwUuid&& u) {
    swap(uuid, u.uuid);
}

PwUuid::PwUuid(const string& s) {
    checkSize(s);
    copy(s.begin(), s.end(), uuid.begin());
}

PwUuid::PwUuid(string&& s) {
    checkSize(s);
    move(s.begin(), s.end(), uuid.begin());
}

PwUuid::uuid_generator_t PwUuid::uuid_generator = PwUuid::uuid_generator_t{};
const PwUuid PwUuid::Zero = boost::uuids::nil_generator()();

string PwUuid::Bytes() const {
    return string(uuid.begin(), uuid.end());
}

string PwUuid::ToString() const {
    ostringstream s;
    s << uuid;
    return s.str();
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
    return left.uuid < right.uuid;
}

bool operator ==(const PwUuid& left, const PwUuid& right) {
    return left.uuid == right.uuid;
}

bool operator !=(const PwUuid& left, const PwUuid& right) {
    return !(left == right);
}
