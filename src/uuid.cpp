//
// Created by srg91 on 16.04.2018.
//

#include "uuid.hpp"
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
using namespace keepasslibpp;

Uuid::Uuid(const Uuid& u) {
    uuid = u.uuid;
}

Uuid::Uuid(Uuid&& u) {
    swap(uuid, u.uuid);
}

Uuid::Uuid(const string& s) {
    checkSize(s);
    copy(s.begin(), s.end(), uuid.begin());
}

Uuid::Uuid(string&& s) {
    checkSize(s);
    move(s.begin(), s.end(), uuid.begin());
}

Uuid::Uuid(const type::byte_vector& b) {
    checkSize(b);
    copy(b.begin(), b.end(), uuid.begin());
}

Uuid::Uuid(type::byte_vector&& b) {
    checkSize(b);
    move(b.begin(), b.end(), uuid.begin());
}

Uuid::uuid_generator_t Uuid::uuid_generator = Uuid::uuid_generator_t{};
const Uuid Uuid::Nil = boost::uuids::nil_generator()();

type::byte_vector Uuid::Bytes() const {
    return type::byte_vector(uuid.begin(), uuid.end());
}

std::string Uuid::ByteString() const {
    return std::string(uuid.begin(), uuid.end());
}

string Uuid::ToString() const {
    ostringstream s;
    s << uuid;
    return s.str();
}

namespace keepasslibpp {
    ostream& operator <<(ostream& stream, const Uuid& u) {
        return stream << u.ToString();
    }

    bool operator <(const Uuid& left, const Uuid& right) {
        return left.uuid < right.uuid;
    }

    bool operator ==(const Uuid& left, const Uuid& right) {
        return left.uuid == right.uuid;
    }

    bool operator !=(const Uuid& left, const Uuid& right) {
        return !(left == right);
    }
}
