#include "mem_util.hpp"

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(test_mem_util)
    BOOST_AUTO_TEST_CASE(test_write_to_stream) {
        std::ostringstream s;

        uint32_t v = 0x11223344;
        MemUtil::Write(s, v);

        std::string expected = {0x44, 0x33, 0x22, 0x11};
        BOOST_CHECK_EQUAL(s.str(), expected);
    }
    BOOST_AUTO_TEST_CASE(test_write_to_string) {
        uint32_t v = 0x11223344;
        std::string result = MemUtil::Write(v, sizeof(v));
        std::string expected = {0x44, 0x33, 0x22, 0x11};
        BOOST_CHECK_EQUAL(result, expected);
    }
BOOST_AUTO_TEST_SUITE_END()
