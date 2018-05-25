#include "mem_util.hpp"

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <sstream>
#include <string>

using namespace keepasslib;

BOOST_AUTO_TEST_SUITE(test_mem_util)
    BOOST_AUTO_TEST_CASE(test_read_bool_from_stream) {
        std::istringstream s({0x01, 0x00});
        auto true_result = mem_util::Read<bool>(s);
        auto false_result = mem_util::Read<bool>(s);
        BOOST_CHECK_EQUAL(true_result, true);
        BOOST_CHECK_EQUAL(false_result, false);
    }

    BOOST_AUTO_TEST_CASE(test_read_int32_from_stream) {
        std::istringstream s({});
        BOOST_CHECK_EQUAL(true_result, true);
        BOOST_CHECK_EQUAL(false_result, false);
    }

    BOOST_AUTO_TEST_CASE(test_write_to_stream) {
        std::ostringstream s;

        uint32_t v = 0x11223344;
        mem_util::Write(s, v);

        std::string expected = {0x44, 0x33, 0x22, 0x11};
        BOOST_CHECK_EQUAL(s.str(), expected);
    }
    BOOST_AUTO_TEST_CASE(test_write_to_string) {
        uint32_t v = 0x11223344;
        std::string result = mem_util::Write(v, sizeof(v));
        std::string expected = {0x44, 0x33, 0x22, 0x11};
        BOOST_CHECK_EQUAL(result, expected);
    }
BOOST_AUTO_TEST_SUITE_END()
