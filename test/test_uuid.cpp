#include "byte_vector.hpp"
#include "uuid.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace keepasslibpp;

TEST(TestUuid, EqualUuids) {
    Uuid u1, u2;
    Uuid u3(u1);

    EXPECT_EQ(u1, u3);
    EXPECT_NE(u1, u2);
}

TEST(TestUuid, CompareLength) {
    std::string no_throw(UUID_SIZE, 0);
    EXPECT_NO_THROW(Uuid::fromString(no_throw).string());

    std::string expect_throw(UUID_SIZE + 5, 0);
    EXPECT_THROW(Uuid::fromString(expect_throw).string(),
                 std::invalid_argument);
}

TEST(TestUuid, NewUuid) {
    Uuid u1;
    auto byte_string = u1.string();
    auto u2 = Uuid::fromString(byte_string);
    EXPECT_EQ(u1, u2);

    auto u3 = Uuid::fromByteVector(u2.byteVector());
    EXPECT_EQ(u1, u3);
    EXPECT_EQ(u2, u3);

    const size_t default_size = UUID_SIZE;
    EXPECT_EQ(std::size(byte_string), default_size);
}

TEST(TestUuid, NewUuidByUuid) {
    Uuid::uuid_t expected_uuid;
    std::string expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    std::copy(expected.begin(), expected.end(), expected_uuid.begin());

    Uuid u(expected_uuid);
    auto byte_string = u.string();
    EXPECT_EQ(byte_string, expected);
}

TEST(TestUuid, NewUuidByString) {
    std::string expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    auto u = Uuid::fromString(expected);
    auto byte_string = u.string();
    EXPECT_EQ(byte_string, expected);
}

TEST(TestUuid, NewUuidByByteVector) {
    ByteVector expected = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    auto u = Uuid::fromByteVector(expected);
    auto bytes = u.byteVector();
    EXPECT_TRUE(bytes == expected);
}

TEST(TestUuid, NewUuidByIterator) {
    std::vector<uint8_t> v = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };

    Uuid u(v.begin(), v.end());

    auto bytes = u.string();
    std::string expected(v.begin(), v.end());
    EXPECT_EQ(bytes, expected);
}

TEST(TestUuid, NewUuidByRValue) {
    Uuid u([]() -> Uuid { return Uuid(); }());
    EXPECT_NE(u, UUID_NIL);
}

TEST(TestUuid, LowerOperator) {
    std::string zero_string(16, 0);
    std::string bigger_string = zero_string;
    bigger_string[0] = 0x01;
    EXPECT_LT(zero_string, bigger_string);

    auto zero = Uuid::fromString(zero_string);
    auto bigger = Uuid::fromString(bigger_string);
    EXPECT_LT(zero, bigger);
}

TEST(TestUuid, IsUnique) {
    Uuid u1, u2;
    EXPECT_NE(u1, u2);

    std::vector<Uuid> v(100);
    std::set<Uuid> s(v.begin(), v.end());
    EXPECT_EQ(std::size(v), std::size(s));
}

TEST(TestUuid, IsNil) {
    auto z = Uuid::fromString(std::string(16, 0));
    EXPECT_EQ(z, UUID_NIL);

    std::string bytes = z.string();
    std::string expected(16, 0);
    EXPECT_EQ(bytes, expected);
}

TEST(TestUuid, FormatHex) {
    auto s = UUID_NIL.hex();
    std::string expected("00000000-0000-0000-0000-000000000000");
    EXPECT_EQ(s, expected);

    auto u = Uuid::fromString({
        0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x33, 0x33,
        0x44, 0x44, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55
    });
    s = u.hex();
    expected = "11111111-2222-3333-4444-555555555555";
    EXPECT_EQ(s, expected);
}

TEST(TestUuid, AddToMap) {
    std::map<Uuid, std::string> m;
    int n = 10;
    for (int i = 0; i < n; i++) {
        m[Uuid()] = "some value";
    }
    EXPECT_EQ(std::size(m), n);
}

TEST(TestUuid, AddToUnorderedMap) {
    std::unordered_map<Uuid, std::string> m;
    int n = 10;
    for (int i = 0; i < n; i++) {
        m[Uuid()] = "some value";
    }
    EXPECT_EQ(std::size(m), n);
}

TEST(TestUuid, AddToSet) {
    std::set<Uuid> s;
    int n = 10;
    for (int i = 0; i < n; i++) {
        s.insert(Uuid());
    }
    EXPECT_EQ(std::size(s), n);
}

TEST(TestUuid, AddToUnorderedSet) {
    std::unordered_set<Uuid> s;
    int n = 10;
    for (int i = 0; i < n; i++) {
        s.insert(Uuid());
    }
    EXPECT_EQ(std::size(s), n);
}
