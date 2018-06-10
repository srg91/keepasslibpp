#include "uuid.hpp"
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

using namespace keepasslibpp;

BOOST_AUTO_TEST_SUITE(test_uuid)
    BOOST_AUTO_TEST_CASE(test_equal_operator) {
        Uuid u1, u2;
        Uuid u3(u1);

        BOOST_CHECK_EQUAL(u1, u3);
        BOOST_CHECK_NE(u1, u2);
    }

    BOOST_AUTO_TEST_CASE(test_length) {
        std::string no_throw(UUID_SIZE, 0);
        BOOST_CHECK_NO_THROW(Uuid::fromString(no_throw).string());

        std::string expect_throw(UUID_SIZE + 5, 0);
        BOOST_CHECK_THROW(Uuid::fromString(expect_throw).string(),
                          std::invalid_argument);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid) {
        Uuid u1;
        auto byte_string = u1.string();
        auto u2 = Uuid::fromString(byte_string);
        BOOST_CHECK_EQUAL(u1, u2);

        auto u3 = Uuid::fromByteVector(u2.byteVector());
        BOOST_CHECK_EQUAL(u1, u3);
        BOOST_CHECK_EQUAL(u2, u3);

        const size_t default_size = UUID_SIZE;
        BOOST_CHECK_EQUAL(byte_string.size(), default_size);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_uuid) {
        Uuid::uuid_t expected_uuid;
        std::string expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        std::copy(expected.begin(), expected.end(), expected_uuid.begin());

        Uuid u(expected_uuid);
        auto byte_string = u.string();
        BOOST_CHECK_EQUAL(byte_string, expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_string) {
        std::string expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto u = Uuid::fromString(expected);
        auto byte_string = u.string();
        BOOST_CHECK_EQUAL(byte_string, expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_bytes) {
        type::ByteVector expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        auto u = Uuid::fromByteVector(expected);
        auto bytes = u.byteVector();
        BOOST_CHECK(bytes == expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_iter) {
        std::vector<uint8_t> v = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        };

        Uuid u(v.begin(), v.end());

        auto bytes = u.string();
        std::string expected(v.begin(), v.end());
        BOOST_CHECK_EQUAL(bytes, expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_rv_uuid) {
        Uuid u([]() -> Uuid { return Uuid(); }());
        BOOST_CHECK_NE(u, UUID_NIL);
    }

    BOOST_AUTO_TEST_CASE(test_lower_operator) {
        std::string zero_string(16, 0);
        std::string bigger_string = zero_string;
        bigger_string[0] = 0x01;
        BOOST_CHECK_LT(zero_string, bigger_string);

        auto zero = Uuid::fromString(zero_string);
        auto bigger = Uuid::fromString(bigger_string);
        BOOST_CHECK_LT(zero, bigger);
    }

    BOOST_AUTO_TEST_CASE(test_unique) {
        Uuid u1, u2;
        BOOST_CHECK_NE(u1, u2);

        std::vector<Uuid> v(100);
        std::set<Uuid> s(v.begin(), v.end());
        BOOST_CHECK_EQUAL(v.size(), s.size());
    }

    BOOST_AUTO_TEST_CASE(test_zero) {
        auto z = Uuid::fromString(std::string(16, 0));
        BOOST_CHECK_EQUAL(z, UUID_NIL);

        std::string bytes = z.string();
        std::string expected(16, 0);
        BOOST_CHECK_EQUAL(bytes, expected);
    }

    BOOST_AUTO_TEST_CASE(test_to_hex) {
        auto s = UUID_NIL.hex();
        std::string expected("00000000-0000-0000-0000-000000000000");
        BOOST_CHECK_EQUAL(s, expected);

        auto u = Uuid::fromString({
            0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x33, 0x33,
            0x44, 0x44, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55
        });
        s = u.hex();
        expected = "11111111-2222-3333-4444-555555555555";
        BOOST_CHECK_EQUAL(s, expected);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_map) {
        std::map<Uuid, std::string> m;
        int n = 10;
        for (int i = 0; i < n; i++) {
            m[Uuid()] = "some value";
        }
        BOOST_CHECK_EQUAL(m.size(), n);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_unordered_map) {
        std::unordered_map<Uuid, std::string> m;
        int n = 10;
        for (int i = 0; i < n; i++) {
            m[Uuid()] = "some value";
        }
        BOOST_CHECK_EQUAL(m.size(), n);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_set) {
        std::set<Uuid> s;
        int n = 10;
        for (int i = 0; i < n; i++) {
            s.insert(Uuid());
        }
        BOOST_CHECK_EQUAL(s.size(), n);
    }

    BOOST_AUTO_TEST_CASE(test_add_to_unordered_set) {
        std::unordered_set<Uuid> s;
        int n = 10;
        for (int i = 0; i < n; i++) {
            s.insert(Uuid());
        }
        BOOST_CHECK_EQUAL(s.size(), n);
    }
BOOST_AUTO_TEST_SUITE_END()
