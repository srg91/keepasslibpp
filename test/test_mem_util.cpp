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
        BOOST_CHECK_EQUAL(true_result, true);

        auto false_result = mem_util::Read<bool>(s);
        BOOST_CHECK_EQUAL(false_result, false);
    }

    BOOST_AUTO_TEST_CASE(test_read_int32_from_stream) {
        std::istringstream s({0x78, 0x56, 0x34, 0x12, 0x2e, -0x03, 0x69, -0x4a});
        auto positive_result = mem_util::Read<std::int32_t>(s);
        int32_t positive_expected = 0x12345678;
        BOOST_CHECK_EQUAL(positive_result, positive_expected);

        auto negative_result = mem_util::Read<std::int32_t>(s);
        int32_t negative_expected = -1234567890;
        BOOST_CHECK_EQUAL(negative_result, negative_expected);
    }

    BOOST_AUTO_TEST_CASE(test_read_int64_from_stream) {
        std::istringstream s({0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
                              0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12});
        auto positive_result = mem_util::Read<std::int64_t>(s);
        int64_t positive_expected = 0x1234567887654321;
        BOOST_CHECK_EQUAL(positive_result, positive_expected);

        auto negative_result = mem_util::Read<std::int64_t>(s);
        int64_t negative_expected = -1234567890987654321;
        BOOST_CHECK_EQUAL(negative_result, negative_expected);
    }

    // TODO: Rework?
    BOOST_AUTO_TEST_CASE(test_read_uint32_from_stream) {
        std::istringstream s({0x78, 0x56, 0x34, 0x12, 0x2e, -0x03, 0x69, -0x4a});
        auto positive_result = mem_util::Read<std::uint32_t>(s);
        uint32_t positive_expected = 0x12345678;
        BOOST_CHECK_EQUAL(positive_result, positive_expected);

        auto negative_result = mem_util::Read<std::uint32_t>(s);
        uint32_t negative_expected = 0xb669fd2e;
        BOOST_CHECK_EQUAL(negative_result, negative_expected);
    }

    // TODO: Rework?
    BOOST_AUTO_TEST_CASE(test_read_uint64_from_stream) {
        std::istringstream s({0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
                              0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12});
        auto positive_result = mem_util::Read<std::uint64_t>(s);
        uint64_t positive_expected = 0x1234567887654321;
        BOOST_CHECK_EQUAL(positive_result, positive_expected);

        auto negative_result = mem_util::Read<std::uint64_t>(s);
        auto negative_expected = static_cast<uint64_t>(-1234567890987654321);
        BOOST_CHECK_EQUAL(negative_result, negative_expected);
    }

    BOOST_AUTO_TEST_CASE(test_read_string_from_stream) {
        std::istringstream s("hello, world!\nanother string");
        std::string expected = "hello, world!";
        auto result = mem_util::Read<std::string>(s, expected.size());
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_read_bool_from_string) {
        BOOST_CHECK_NO_THROW([](){
            std::string s("0x01");
            auto result = mem_util::Read<bool>(s);
            BOOST_CHECK(result);
        });
        BOOST_CHECK_NO_THROW([](){
            std::string s("0x00");
            auto result = mem_util::Read<bool>(s);
            BOOST_CHECK(!result);
        });
    }

    BOOST_AUTO_TEST_CASE(test_read_int32_from_string) {
        auto statement = []{
            std::string s = {0x78, 0x56, 0x34, 0x12};
            auto result = mem_util::Read<std::int32_t>(s);
            std::int32_t expected = 0x12345678;
            BOOST_CHECK_EQUAL(result, expected);
        };
        BOOST_CHECK_NO_THROW(statement());
    }

    BOOST_AUTO_TEST_CASE(test_read_int64_from_string) {
        auto statement = []{
            std::string s = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
            auto result = mem_util::Read<std::int64_t>(s);
            int64_t expected = 0x1234567887654321;
            BOOST_CHECK_EQUAL(result, expected);
        };
        BOOST_CHECK_NO_THROW(statement());
    }

    BOOST_AUTO_TEST_CASE(test_read_uint32_from_string) {
        auto statement = []{
            std::string s = {0x78, 0x56, 0x34, 0x12};
            auto result = mem_util::Read<std::uint32_t>(s);
            std::uint32_t expected = 0x12345678;
            BOOST_CHECK_EQUAL(result, expected);
        };
        BOOST_CHECK_NO_THROW(statement());
    }

    BOOST_AUTO_TEST_CASE(test_read_uint64_from_string) {
        auto statement = []{
            std::string s = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
            auto result = mem_util::Read<std::uint64_t>(s);
            uint64_t expected = 0x1234567887654321;
            BOOST_CHECK_EQUAL(result, expected);
        };
        BOOST_CHECK_NO_THROW(statement());
    }

    BOOST_AUTO_TEST_CASE(test_read_int32_with_incorrect_size) {
        auto short_statement = []{
            std::string s = {0x78, 0x56, 0x34};
            mem_util::Read<std::int32_t>(s);
        };
        BOOST_CHECK_THROW(short_statement(), std::invalid_argument);

        auto long_statement = []{
            std::string s = {0x78, 0x56, 0x34, 0x12, 0x11};
            mem_util::Read<std::int32_t>(s);
        };
        BOOST_CHECK_THROW(long_statement(), std::invalid_argument);
    }

    BOOST_AUTO_TEST_CASE(test_write_to_stream) {
        std::ostringstream s;

        std::uint32_t v = 0x11223344;
        mem_util::Write(s, v);

        std::string expected = {0x44, 0x33, 0x22, 0x11};
        BOOST_CHECK_EQUAL(s.str(), expected);
    }
    BOOST_AUTO_TEST_CASE(test_write_to_string) {
        std::uint32_t v = 0x11223344;
        std::string result = mem_util::Write(v, sizeof(v));
        std::string expected = {0x44, 0x33, 0x22, 0x11};
        BOOST_CHECK_EQUAL(result, expected);
    }
BOOST_AUTO_TEST_SUITE_END()
