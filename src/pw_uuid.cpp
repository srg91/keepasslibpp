//
// Created by srg91 on 16.04.2018.
//

#include "pw_uuid.hpp"
#include "typedefs.hpp"

#include <array>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <utility>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>

using namespace std;
using namespace keepasslib;

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

PwUuid::PwUuid(const types::bytes& b) {
    checkSize(b);
    copy(b.begin(), b.end(), uuid.begin());
}

PwUuid::PwUuid(types::bytes&& b) {
    checkSize(b);
    move(b.begin(), b.end(), uuid.begin());
}

PwUuid::uuid_generator_t PwUuid::uuid_generator = PwUuid::uuid_generator_t{};
const PwUuid PwUuid::Nil = boost::uuids::nil_generator()();

types::bytes PwUuid::Bytes() const {
    return types::bytes(uuid.begin(), uuid.end());
}

std::string PwUuid::ByteString() const {
    return std::string(uuid.begin(), uuid.end());
}

string PwUuid::ToString() const {
    ostringstream s;
    s << uuid;
    return s.str();
}

namespace keepasslib {
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
}
