#include "pw_uuid.hpp"
#include "typedefs.hpp"

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace keepasslib;

BOOST_AUTO_TEST_SUITE(test_pw_uuid)
    BOOST_AUTO_TEST_CASE(test_equal_operator) {
        PwUuid u1, u2;
        PwUuid u3(u1);

        BOOST_CHECK_EQUAL(u1, u3);
        BOOST_CHECK_NE(u1, u2);
    }

    BOOST_AUTO_TEST_CASE(test_length) {
        std::string no_throw(PwUuid::UuidSize, 0);
        BOOST_CHECK_NO_THROW(PwUuid(no_throw).ToString());

        std::string expect_throw(PwUuid::UuidSize + 5, 0);
        BOOST_CHECK_THROW(PwUuid(expect_throw).ToString(), std::invalid_argument);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid) {
        PwUuid u1;
        auto byte_string = u1.ByteString();
        PwUuid u2(byte_string);
        BOOST_CHECK_EQUAL(u1, u2);

        PwUuid u3(u2.Bytes());
        BOOST_CHECK_EQUAL(u1, u3);
        BOOST_CHECK_EQUAL(u2, u3);

        const size_t default_size = PwUuid::UuidSize;
        BOOST_CHECK_EQUAL(byte_string.size(), default_size);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_uuid) {
        PwUuid::uuid_t expected_uuid;
        std::string expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        std::copy(expected.begin(), expected.end(), expected_uuid.begin());

        PwUuid u(expected_uuid);
        auto byte_string = u.ByteString();
        BOOST_CHECK_EQUAL(byte_string, expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_string) {
        std::string expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        PwUuid u(expected);
        auto byte_string = u.ByteString();
        BOOST_CHECK_EQUAL(byte_string, expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_bytes) {
        types::bytes expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        PwUuid u(expected);
        auto bytes = u.Bytes();
        BOOST_CHECK(bytes == expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_iter) {
        std::vector<uint8_t> v = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        };

        PwUuid u(v.begin(), v.end());

        auto bytes = u.ByteString();
        std::string expected(v.begin(), v.end());
        BOOST_CHECK_EQUAL(bytes, expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_rv_uuid) {
        PwUuid u([]() -> PwUuid { return PwUuid(); }());
        BOOST_CHECK_NE(u, PwUuid::Nil);
    }

    BOOST_AUTO_TEST_CASE(test_lower_operator) {
        std::string zero_string(16, 0);
        std::string bigger_string = zero_string;
        bigger_string[0] = 0x01;
        BOOST_CHECK_LT(zero_string, bigger_string);

        PwUuid zero(zero_string), bigger(bigger_string);
        BOOST_CHECK_LT(zero, bigger);
    }

    BOOST_AUTO_TEST_CASE(test_unique) {
        PwUuid u1, u2;
        BOOST_CHECK_NE(u1, u2);

        std::vector<PwUuid> v(100);
        std::set<PwUuid> s(v.begin(), v.end());
        BOOST_CHECK_EQUAL(v.size(), s.size());
    }

    BOOST_AUTO_TEST_CASE(test_zero) {
        PwUuid z = std::string(16, 0);
        BOOST_CHECK_EQUAL(PwUuid::Nil, z);

        std::string bytes = z.ByteString();
        std::string expected(16, 0);
        BOOST_CHECK_EQUAL(bytes, expected);
    }

    BOOST_AUTO_TEST_CASE(test_to_string) {
        auto s = PwUuid::Nil.ToString();
        std::string expected("00000000-0000-0000-0000-000000000000");
        BOOST_CHECK_EQUAL(s, expected);

        PwUuid u(std::string({0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x33, 0x33,
                              0x44, 0x44, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}));
        s = u.ToString();
        expected = "11111111-2222-3333-4444-555555555555";
        BOOST_CHECK_EQUAL(s, expected);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_map) {
        std::map<PwUuid, std::string> m;
        int n = 10;
        for (int i = 0; i < n; i++) {
            m[PwUuid()] = "some value";
        }
        BOOST_CHECK_EQUAL(m.size(), n);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_unordered_map) {
        std::unordered_map<PwUuid, std::string> m;
        int n = 10;
        for (int i = 0; i < n; i++) {
            m[PwUuid()] = "some value";
        }
        BOOST_CHECK_EQUAL(m.size(), n);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_set) {
        std::set<PwUuid> s;
        int n = 10;
        for (int i = 0; i < n; i++) {
            s.insert(PwUuid());
        }
        BOOST_CHECK_EQUAL(s.size(), n);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_unordered_set) {
        std::unordered_set<PwUuid> s;
        int n = 10;
        for (int i = 0; i < n; i++) {
            s.insert(PwUuid());
        }
        BOOST_CHECK_EQUAL(s.size(), n);
    }
BOOST_AUTO_TEST_SUITE_END()
