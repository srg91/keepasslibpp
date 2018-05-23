#include "pw_uuid.hpp"

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(test_pw_uuid)
    BOOST_AUTO_TEST_CASE(test_equal_operator) {
        PwUuid u1, u2;
        PwUuid u3(u1);

        BOOST_CHECK_EQUAL(u1, u3);
        BOOST_CHECK_NE(u1, u2);
    }

    BOOST_AUTO_TEST_CASE(test_length) {
        std::string no_throw(16, 0);
        BOOST_CHECK_NO_THROW(PwUuid(no_throw));

        std::string expect_throw(5, 0);
        BOOST_CHECK_THROW(PwUuid(expect_throw), std::invalid_argument);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid) {
        PwUuid u1;
        auto bytes = u1.Bytes();
        PwUuid u2(bytes);
        BOOST_CHECK_EQUAL(u1, u2);
        BOOST_CHECK_EQUAL(bytes.size(), PwUuid::UuidSize);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_string) {
        std::string expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        PwUuid u(expected);
        auto bytes = u.Bytes();
        BOOST_CHECK_EQUAL(bytes, expected);
    }

    BOOST_AUTO_TEST_CASE(test_new_uuid_by_iter) {
        std::vector<uint8_t> v = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        };

        PwUuid u(v.begin(), v.end());

        auto bytes = u.Bytes();
        std::string expected(v.begin(), v.end());
        BOOST_CHECK_EQUAL(bytes, expected);
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
        BOOST_CHECK_EQUAL(PwUuid::Zero, z);

        std::string bytes = z.Bytes();
        std::string expected(16, 0);
        BOOST_CHECK_EQUAL(bytes, expected);
    }
BOOST_AUTO_TEST_SUITE_END()
