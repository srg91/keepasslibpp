#include "exception.hpp"
#include "memory_util.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iterator>
#include <sstream>
#include <string>

using namespace keepasslibpp;

TEST(TestMemoryUtil, ReadBoolFromStream) {
    std::istringstream s({0x01, 0x00});
    auto true_result = MemoryUtil::read<bool>(s);
    EXPECT_TRUE(true_result);

    auto false_result = MemoryUtil::read<bool>(s);
    EXPECT_FALSE(false_result);
}

TEST(TestMemoryUtil, ReadInt32FromStream) {
    std::istringstream s({0x78, 0x56, 0x34, 0x12, 0x2e, -0x03, 0x69, -0x4a});
    auto positive_result = MemoryUtil::read<std::int32_t>(s);
    int32_t positive_expected = 0x12345678;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = MemoryUtil::read<std::int32_t>(s);
    int32_t negative_expected = -1234567890;
    EXPECT_EQ(negative_result, negative_expected);
}

TEST(TestMemoryUtil, ReadInt64FromStream) {
    std::istringstream s({0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
                          0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12});
    auto positive_result = MemoryUtil::read<std::int64_t>(s);
    int64_t positive_expected = 0x1234567887654321;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = MemoryUtil::read<std::int64_t>(s);
    int64_t negative_expected = -1234567890987654321;
    EXPECT_EQ(negative_result, negative_expected);
}

// TODO: Rework?
TEST(TestMemoryUtil, ReadUint32FromStream) {
    std::istringstream s({0x78, 0x56, 0x34, 0x12, 0x2e, -0x03, 0x69, -0x4a});
    auto positive_result = MemoryUtil::read<std::uint32_t>(s);
    uint32_t positive_expected = 0x12345678;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = MemoryUtil::read<std::uint32_t>(s);
    auto negative_expected = static_cast<uint32_t>(-1234567890);
    EXPECT_EQ(negative_result, negative_expected);
}

// TODO: Rework?
TEST(TestMemoryUtil, ReadUint64FromStream) {
    std::istringstream s({0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
                          0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12});
    auto positive_result = MemoryUtil::read<std::uint64_t>(s);
    uint64_t positive_expected = 0x1234567887654321;
    EXPECT_EQ(positive_result, positive_expected);

    auto negative_result = MemoryUtil::read<std::uint64_t>(s);
    auto negative_expected = static_cast<uint64_t>(-1234567890987654321);
    EXPECT_EQ(negative_result, negative_expected);
}

TEST(TestMemoryUtil, ReadStringFromStream) {
    std::istringstream s("hello, world!\nanother string");
    std::string expected = "hello, world!";
    auto result = MemoryUtil::read<std::string>(s, std::size(expected));
    EXPECT_EQ(result, expected);
}

TEST(TestMemoryUtil, ReadMultipleFromStream) {
    std::istringstream s({
        0x01,
        'h', 'e', 'l', 'l', 'o',
        0x78, 0x56, 0x34, 0x12,
    });

    auto bool_result = MemoryUtil::read<bool>(s);
    auto string_result = MemoryUtil::read<std::string>(s, 5);
    auto int32_result = MemoryUtil::read<std::int32_t>(s);

    EXPECT_TRUE(bool_result);
    EXPECT_EQ(string_result, "hello");
    EXPECT_EQ(int32_result, 0x12345678);
}

TEST(TestMemoryUtil, ReadBoolFromString) {
    EXPECT_NO_THROW([](){
        std::string s("0x01");
        auto result = MemoryUtil::read<bool>(s);
        EXPECT_TRUE(result);
    });
    EXPECT_NO_THROW([](){
        std::string s("0x00");
        auto result = MemoryUtil::read<bool>(s);
        EXPECT_TRUE(!result);
    });
}

TEST(TestMemoryUtil, ReadInt32FromString) {
    auto statement = []{
        std::string s = {0x78, 0x56, 0x34, 0x12};
        auto result = MemoryUtil::read<std::int32_t>(s);
        std::int32_t expected = 0x12345678;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemoryUtil, ReadInt64FromString) {
    auto statement = []{
        std::string s = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
        auto result = MemoryUtil::read<std::int64_t>(s);
        int64_t expected = 0x1234567887654321;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemoryUtil, ReadUint32FromString) {
    auto statement = []{
        std::string s = {0x78, 0x56, 0x34, 0x12};
        auto result = MemoryUtil::read<std::uint32_t>(s);
        std::uint32_t expected = 0x12345678;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemoryUtil, ReadUint64FromString) {
    auto statement = []{
        std::string s = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
        auto result = MemoryUtil::read<std::uint64_t>(s);
        uint64_t expected = 0x1234567887654321;
        EXPECT_EQ(result, expected);
    };
    EXPECT_NO_THROW(statement());
}

TEST(TestMemoryUtil, ReadInt32FromEmptyStream) {
    auto empty_stream_statement = []{
        std::istringstream s;
        MemoryUtil::read<std::int32_t>(s);
    };
    EXPECT_THROW(empty_stream_statement(), exception::NotEnoughBytesException);

    auto short_stream_statement = []{
        std::istringstream s({0x78, 0x56});
        MemoryUtil::read<std::int32_t>(s);
    };
    EXPECT_THROW(short_stream_statement(), exception::NotEnoughBytesException);
}

TEST(TestMemoryUtil, ReadInt32WithIncorrectStringSize) {
    auto short_statement = []{
        std::string s = {0x78, 0x56, 0x34};
        MemoryUtil::read<std::int32_t>(s);
    };
    EXPECT_THROW(short_statement(), exception::NotEnoughBytesException);

    auto long_statement = []{
        std::string s = {0x78, 0x56, 0x34, 0x12, 0x11};
        MemoryUtil::read<std::int32_t>(s);
    };
    EXPECT_THROW(long_statement(), exception::NotEnoughBytesException);
}

TEST(TestMemoryUtil, WriteBoolToStream) {
    std::ostringstream s;
    MemoryUtil::write<bool>(s, false);
    MemoryUtil::write<bool>(s, true);
    std::string expected = {0x00, 0x01};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemoryUtil, WriteInt32ToStream) {
    std::ostringstream s;
    MemoryUtil::write<int32_t>(s, 0x12345678);
    std::string expected = {0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemoryUtil, WriteInt64ToStream) {
    std::ostringstream s;
    MemoryUtil::write<int64_t>(s, 0x1234567887654321);
    std::string expected = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemoryUtil, WriteUint32ToStream) {
    std::ostringstream s;
    MemoryUtil::write(s, static_cast<uint32_t>(-1234567890));
    std::string expected = {0x2e, -0x03, 0x69, -0x4a};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemoryUtil, WriteUint64ToStream) {
    std::ostringstream s;
    MemoryUtil::write(s, static_cast<uint64_t>(-1234567890987654321));
    std::string expected = {0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12};
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemoryUtil, WriteStringToStream) {
    std::ostringstream s;
    std::string expected = "Hello, World!";
    MemoryUtil::write(s, expected);
    EXPECT_EQ(s.str(), expected);
}

TEST(TestMemoryUtil, WriteBoolToString) {
    EXPECT_EQ(
        MemoryUtil::write(false),
        std::string({0x00})
    );
    EXPECT_EQ(
        MemoryUtil::write(true),
        std::string({0x01})
    );
}

TEST(TestMemoryUtil, WriteInt32ToString) {
    std::string result = MemoryUtil::write<int32_t>(0x12345678);
    std::string expected = {0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(result, expected);
}

TEST(TestMemoryUtil, WriteInt64ToString) {
    std::string result = MemoryUtil::write<int64_t>(0x1234567887654321);
    std::string expected = {0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12};
    EXPECT_EQ(result, expected);
}

TEST(TestMemoryUtil, WriteUint32ToString) {
    std::string result = MemoryUtil::write(static_cast<uint32_t>(-1234567890));
    std::string expected = {0x2e, -0x03, 0x69, -0x4a};
    EXPECT_EQ(result, expected);
}

TEST(TestMemoryUtil, WriteUint64ToString) {
    std::string result = MemoryUtil::write(static_cast<uint64_t>(-1234567890987654321));
    std::string expected = {0x4f, -0x1d, -0x6d, 0x4e, 0x0b, -0x11, -0x23, -0x12};
    EXPECT_EQ(result, expected);
}
