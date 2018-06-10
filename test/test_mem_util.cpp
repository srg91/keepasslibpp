#include "mem_util.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <string>

using namespace keepasslibpp;

TEST(TestMemUtil, ReadBoolFromStream) {
    std::istringstream s({0x01, 0x00});
    auto true_result = mem_util::Read<bool>(s);
    EXPECT_TRUE(true_result);

    auto false_result = mem_util::Read<bool>(s);
    EXPECT_FALSE(false_result);
}

TEST(TestMemUtil, ReadInt32FromStream) {
    std::istringstream s({0x78, 0x56, 0x34, 0x12, 0x2e, -0x03, 0x69, -0x4a});
    auto positive_result = mem_util::Read<std::int32_t>(s);
    int32_t positive_expected = 0x12345678;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = mem_util::Read<std::int32_t>(s);
    int32_t negative_expected = -1234567890;
    EXPECT_EQ(negative_result, negative_expected);
}

TEST(TestMemUtil, ReadInt64FromStream) {
    std::istringstream s({0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
                          0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12});
    auto positive_result = mem_util::Read<std::int64_t>(s);
    int64_t positive_expected = 0x1234567887654321;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = mem_util::Read<std::int64_t>(s);
    int64_t negative_expected = -1234567890987654321;
    EXPECT_EQ(negative_result, negative_expected);
}

// TODO: Rework?
TEST(TestMemUtil, ReadUint32FromStream) {
    std::istringstream s({0x78, 0x56, 0x34, 0x12, 0x2e, -0x03, 0x69, -0x4a});
    auto positive_result = mem_util::Read<std::uint32_t>(s);
    uint32_t positive_expected = 0x12345678;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = mem_util::Read<std::uint32_t>(s);
    auto negative_expected = static_cast<uint32_t>(-1234567890);
    EXPECT_EQ(negative_result, negative_expected);
}

// TODO: Rework?
TEST(TestMemUtil, ReadUint64FromStream) {
    std::istringstream s({0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
                          0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12});
    auto positive_result = mem_util::Read<std::uint64_t>(s);
    uint64_t positive_expected = 0x1234567887654321;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = mem_util::Read<std::uint64_t>(s);
    auto negative_expected = static_cast<uint64_t>(-1234567890987654321);
    EXPECT_EQ(negative_result, negative_expected);
}

TEST(TestMemUtil, ReadStringFromStream) {
    std::istringstream s("hello, world!\nanother string");
    std::string expected = "hello, world!";
    auto result = mem_util::Read<std::string>(s, expected.size());
    EXPECT_EQ(result, expected);
}

TEST(TestMemUtil, ReadMultipleFromStream) {
    std::istringstream s({
        0x01,
        'h', 'e', 'l', 'l', 'o',
        0x78, 0x56, 0x34, 0x12,
    });

    auto bool_result = mem_util::Read<bool>(s);
    auto string_result = mem_util::Read<std::string>(s, 5);
    auto int32_result = mem_util::Read<std::int32_t>(s);

    EXPECT_TRUE(bool_result);
    EXPECT_EQ(string_result, "hello");
    EXPECT_EQ(int32_result, 0x12345678);
}

TEST(TestMemUtil, ReadBoolFromString) {
    EXPECT_NO_THROW([](){
        std::string s("0x01");
        auto result = mem_util::Read<bool>(s);
        EXPECT_TRUE(result);
    });
    EXPECT_NO_THROW([](){
        std::string s("0x00");
        auto result = mem_util::Read<bool>(s);
        EXPECT_TRUE(!result);
    });
}

TEST(TestMemUtil, ReadInt32FromString) {
    auto statement = []{
        std::string s = {0x78, 0x56, 0x34, 0x12};
        auto result = mem_util::Read<std::int32_t>(s);
        std::int32_t expected = 0x12345678;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemUtil, ReadInt64FromString) {
    auto statement = []{
        std::string s = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
        auto result = mem_util::Read<std::int64_t>(s);
        int64_t expected = 0x1234567887654321;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemUtil, ReadUint32FromString) {
    auto statement = []{
        std::string s = {0x78, 0x56, 0x34, 0x12};
        auto result = mem_util::Read<std::uint32_t>(s);
        std::uint32_t expected = 0x12345678;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemUtil, ReadUint64FromString) {
    auto statement = []{
        std::string s = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
        auto result = mem_util::Read<std::uint64_t>(s);
        uint64_t expected = 0x1234567887654321;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemUtil, ReadInt32FromEmptyStream) {
    auto empty_stream_statement = []{
        std::istringstream s;
        mem_util::Read<std::int32_t>(s);
    };
    EXPECT_THROW(empty_stream_statement(), std::length_error);

    auto short_stream_statement = []{
        std::istringstream s({0x78, 0x56});
        mem_util::Read<std::int32_t>(s);
    };
    EXPECT_THROW(short_stream_statement(), std::length_error);
}

TEST(TestMemUtil, ReadInt32WithIncorrectStringSize) {
    auto short_statement = []{
        std::string s = {0x78, 0x56, 0x34};
        mem_util::Read<std::int32_t>(s);
    };
    EXPECT_THROW(short_statement(), std::length_error);

    auto long_statement = []{
        std::string s = {0x78, 0x56, 0x34, 0x12, 0x11};
        mem_util::Read<std::int32_t>(s);
    };
    EXPECT_THROW(long_statement(), std::length_error);
}

TEST(TestMemUtil, WriteBoolToStream) {
    std::ostringstream s;
    mem_util::Write<bool>(s, false);
    mem_util::Write<bool>(s, true);
    std::string expected = {0x00, 0x01};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemUtil, WriteInt32ToStream) {
    std::ostringstream s;
    mem_util::Write<int32_t>(s, 0x12345678);
    std::string expected = {0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemUtil, WriteInt64ToStream) {
    std::ostringstream s;
    mem_util::Write<int64_t>(s, 0x1234567887654321);
    std::string expected = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemUtil, WriteUint32ToStream) {
    std::ostringstream s;
    mem_util::Write(s, static_cast<uint32_t>(-1234567890));
    std::string expected = {0x2e, -0x03, 0x69, -0x4a};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemUtil, WriteUint64ToStream) {
    std::ostringstream s;
    mem_util::Write(s, static_cast<uint64_t>(-1234567890987654321));
    std::string expected = {0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemUtil, WriteStringToStream) {
    std::ostringstream s;
    std::string expected = "Hello, World!";
    mem_util::Write(s, expected);
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemUtil, WriteBoolToString) {
    EXPECT_EQ(
        mem_util::Write(false),
        std::string({0x00})
    );
    EXPECT_EQ(
        mem_util::Write(true),
        std::string({0x01})
    );
}

TEST(TestMemUtil, WriteInt32ToString) {
    std::string result = mem_util::Write<int32_t>(0x12345678);
    std::string expected = {0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(result, expected);
}

TEST(TestMemUtil, WriteInt64ToString) {
    std::string result = mem_util::Write<int64_t>(0x1234567887654321);
    std::string expected = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(result, expected);
}

TEST(TestMemUtil, WriteUint32ToString) {
    std::string result = mem_util::Write(static_cast<uint32_t>(-1234567890));
    std::string expected = {0x2e, -0x03, 0x69, -0x4a};
    EXPECT_EQ(result, expected);
}

TEST(TestMemUtil, WriteUint64ToString) {
    std::string result = mem_util::Write(static_cast<uint64_t>(-1234567890987654321));
    std::string expected = {0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12};
    EXPECT_EQ(result, expected);
}
