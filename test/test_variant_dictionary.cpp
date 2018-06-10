#include "exception.hpp"
#include "variant_dictionary.hpp"
#include "typedefs.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

using namespace keepasslibpp;

class TestVariantDictionary : public ::testing::Test {
protected:
    std::string sample_bytes;
    VariantDictionary sample_dict;

    void SetUp() override {
        sample_dict["true_bool"] = true;
        sample_dict["false_bool"] = false;
        sample_dict["int32"] = std::int32_t(0x12345678);
        sample_dict["int64"] = std::int64_t(0x1234567887654321);
        sample_dict["uint32"] = std::uint32_t(0x12345678);
        sample_dict["uint64"] = std::uint64_t(0x1234567887654321);
        sample_dict["string"] = std::string("hello, world");
        sample_dict["bytes"] = type::ByteVector(
            {'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'}
        );

        sample_bytes = {
            // vd version
            0x00, 0x01,
            // ByteVector type
            0x42,
            // ByteVector key size
            0x05, 0x00, 0x00, 0x00,
            // ByteVector key
            'b', 'y', 't', 'e', 's',
            // ByteVector value size
            0x0c, 0x00, 0x00, 0x00,
            // ByteVector value
            'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd',
            // bool type
            0x08,
            // false bool key size
            0x0a, 0x00, 0x00, 0x00,
            // false bool key
            'f', 'a', 'l', 's', 'e', '_', 'b', 'o', 'o', 'l',
            // false value size
            0x01, 0x00, 0x00, 0x00,
            // false value
            0x00,
            // int32 type
            0x0c,
            // int32 key size
            0x05, 0x00, 0x00, 0x00,
            // int32 key
            'i', 'n', 't', '3', '2',
            // int32 value size
            0x04, 0x00, 0x00, 0x00,
            // int32 value
            0x78, 0x56, 0x34, 0x12,
            // int64 type
            0x0d,
            // int64 key size
            0x05, 0x00, 0x00, 0x00,
            // int64 key
            'i', 'n', 't', '6', '4',
            // int64 value size
            0x08, 0x00, 0x00, 0x00,
            // int64 value
            0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
            // string type
            0x18,
            // string key size
            0x06, 0x00, 0x00, 0x00,
            // string key
            's', 't', 'r', 'i', 'n', 'g',
            // string value size
            0x0c, 0x00, 0x00, 0x00,
            // string value
            'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd',
            // bool type
            0x08,
            // true bool key size
            0x09, 0x00, 0x00, 0x00,
            // true bool key
            't', 'r', 'u', 'e', '_', 'b', 'o', 'o', 'l',
            // true value size
            0x01, 0x00, 0x00, 0x00,
            // true value
            0x01,
            // uint32 type
            0x04,
            // uint32 key size
            0x06, 0x00, 0x00, 0x00,
            // uint32 key
            'u', 'i', 'n', 't', '3', '2',
            // uint32 value size
            0x04, 0x00, 0x00, 0x00,
            // uint32 value
            0x78, 0x56, 0x34, 0x12,
            // uint64 type
            0x05,
            // uint64 key size
            0x06, 0x00, 0x00, 0x00,
            // uint64 key
            'u', 'i', 'n', 't', '6', '4',
            // uint64 value size
            0x08, 0x00, 0x00, 0x00,
            // uint64 value
            0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
            // none type (=end)
            0x00,
        };
    }
};

TEST_F(TestVariantDictionary, SerializationToString) {
    std::string result = sample_dict.serialize();
    std::string expected = sample_bytes;
    EXPECT_EQ(result, expected);
}

TEST_F(TestVariantDictionary, SerializationToStream) {
    std::ostringstream stream;
    sample_dict.serialize(stream);
    std::string expected = sample_bytes;
    EXPECT_EQ(stream.str(), expected);
}

TEST_F(TestVariantDictionary, DeserializationFromString) {
    auto vd = VariantDictionary::deserialize(sample_bytes);
    // TODO: Implement << operator?
    EXPECT_TRUE(vd == sample_dict);
}

TEST_F(TestVariantDictionary, DeserializationFromStream) {
    std::istringstream stream(sample_bytes);
    auto vd = VariantDictionary::deserialize(stream);
    // TODO: Implement << operator?
    EXPECT_TRUE(vd == sample_dict);
}

TEST(TestVariantDictionary, DeserializationEmptyDictionary) {
    std::string s = {0x00, 0x01, 0x00};
    auto vd = VariantDictionary::deserialize(s);
    EXPECT_TRUE(vd.empty());
    EXPECT_EQ(vd.size(), 0);
}

TEST(TestVariantDictionary, DeserializationEmptyFile) {
    EXPECT_THROW(VariantDictionary::deserialize(""),
                 exception::FileCorruptedError);
}

TEST(TestVariantDictionary, DeserializationNewestFile) {
    // Newest version - 0x02
    auto statement_newest_version = []{
        std::string s = {0x00, 0x02};
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_newest_version(),
                 exception::NewVersionRequiredError);

    // Unknown type - 0x03 with key "key" and value 0x01
    auto statement_new_unknown_type = []{
        std::string s = {
            0x00, 0x01,
            0x03,
            0x03, 0x00, 0x00, 0x00,
            'k', 'e', 'y',
            0x01, 0x00, 0x00, 0x00,
            0x01,
            0x00
        };
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_new_unknown_type(),
                 exception::NewVersionRequiredError);
}

TEST(TestVariantDictionary, DeserializationCorruptedType) {
    auto statement_corrupted_type = []{
        std::string s = {0x00, 0x01, -0x01};
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_corrupted_type(), exception::FileCorruptedError);
}

TEST(TestVariantDictionary, DeserializationUnexpectedEndOnType) {
    std::string s = {
        0x00, 0x01,
        0x08,
        0x04, 0x00, 0x00, 0x00,
        'b', 'o', 'o', 'l',
        0x01, 0x00, 0x00, 0x00,
        0x01
        // 0x00 is expected
    };
    EXPECT_THROW(VariantDictionary::deserialize(s),
                 exception::FileCorruptedError);
}

TEST(TestVariantDictionary, DeserializationEndOnReading) {
    auto statement_failed_on_type = []{
        std::string s = {0x00, 0x01};
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_failed_on_type(), exception::FileCorruptedError);

    auto statement_failed_on_key_size = []{
        std::string s = {0x00, 0x01, 0x08, 0x04};
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_failed_on_key_size(), exception::FileCorruptedError);

    auto statement_failed_on_key = []{
        std::string s = {0x00, 0x01, 0x08, 0x04, 0x00, 0x00, 0x00,
                         'k', 'e', 'y'};
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_failed_on_key(), exception::FileCorruptedError);

    auto statement_failed_on_value_size = []{
        std::string s = {
            0x00, 0x01,
            0x08,
            0x04, 0x00, 0x00, 0x00,
            'b', 'o', 'o', 'l',
            0x04
        };
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_failed_on_value_size(),
                 exception::FileCorruptedError);

    auto statement_failed_on_value = []{
        std::string s = {
            0x00, 0x01,
            0x18,
            0x03, 0x00, 0x00, 0x00,
            's', 't', 'r',
            0x04, 0x00, 0x00, 0x00,
            'v', 'a', 'l'
        };
        VariantDictionary::deserialize(s);
    };
    EXPECT_THROW(statement_failed_on_value(), exception::FileCorruptedError);
}

TEST_F(TestVariantDictionary, GetBool) {
    bool result;
    EXPECT_TRUE(sample_dict.get("true_bool", result));
    EXPECT_TRUE(result);

    EXPECT_TRUE(sample_dict.get("false_bool", result));
    EXPECT_TRUE(!result);
}

TEST_F(TestVariantDictionary, GetInt32) {
    std::int32_t result;
    EXPECT_TRUE(sample_dict.get("int32", result));
    EXPECT_EQ(result, 0x12345678);
}

TEST_F(TestVariantDictionary, GetInt64) {
    std::int64_t result;
    EXPECT_TRUE(sample_dict.get("int64", result));
    EXPECT_EQ(result, 0x1234567887654321);
}

TEST_F(TestVariantDictionary, GetUint32) {
    std::uint32_t result;
    EXPECT_TRUE(sample_dict.get("uint32", result));
    EXPECT_EQ(result, 0x12345678);
}

TEST_F(TestVariantDictionary, GetUint64) {
    std::uint64_t result;
    EXPECT_TRUE(sample_dict.get("uint64", result));
    EXPECT_EQ(result, 0x1234567887654321);
}

TEST_F(TestVariantDictionary, GetString) {
    std::string result;
    EXPECT_TRUE(sample_dict.get<std::string>("string", result));
    EXPECT_EQ(result, "hello, world");
}

TEST_F(TestVariantDictionary, GetByteVector) {
    type::ByteVector result;
    EXPECT_TRUE(sample_dict.get("bytes", result));

    type::ByteVector expected = {
        'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'
    };
    EXPECT_TRUE(result == expected);
}

TEST(TestVariantDictionary, GetNothing) {
    VariantDictionary vd;
    std::int32_t value;
    bool success = vd.get<std::int32_t>("unknown_key", value);
    EXPECT_TRUE(!success);
}

TEST(TestVariantDictionary, SetBool) {
    VariantDictionary vd;
vd.set<bool>("key", true);

    std::string result = vd.serialize();
    // TODO: Cut version and end None?
    std::string expected = {
        0x00, 0x01,
        0x08,
        0x03, 0x00, 0x00, 0x00,
        'k', 'e', 'y',
        0x01, 0x00, 0x00, 0x00,
        0x01,
        0x00,
    };
    EXPECT_EQ(result, expected);
}

TEST(TestVariantDictionary, SetInt32) {
    VariantDictionary vd;
vd.set<std::int32_t>("key", 0x12345678);

    std::string result = vd.serialize();
    std::string expected = {
        0x00, 0x01,
        0x0c,
        0x03, 0x00, 0x00, 0x00,
        'k', 'e', 'y',
        0x04, 0x00, 0x00, 0x00,
        0x78, 0x56, 0x34, 0x12,
        0x00,
    };
    std::ostringstream s;
    for (const auto& c : result) {
        s << std::hex << std::setfill('0') << std::setw(2)
          << static_cast<unsigned>(static_cast<unsigned char>(c));
    }
    std::ostringstream d;
    for (const auto& c : expected) {
        d << std::hex << std::setfill('0') << std::setw(2)
          << static_cast<unsigned>(static_cast<unsigned char>(c));
    }
    EXPECT_EQ(result, expected);
}

TEST(TestVariantDictionary, SetInt64) {
    VariantDictionary vd;
vd.set<std::int64_t>("key", 0x1234567887654321);

    std::string result = vd.serialize();
    std::string expected = {
        0x00, 0x01,
        0x0d,
        0x03, 0x00, 0x00, 0x00,
        'k', 'e', 'y',
        0x08, 0x00, 0x00, 0x00,
        0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
        0x00,
    };
    EXPECT_EQ(result, expected);
}

TEST(TestVariantDictionary, SetUint32) {
    VariantDictionary vd;
vd.set<std::uint32_t>("key", 0x12345678);

    std::string result = vd.serialize();
    std::string expected = {
        0x00, 0x01,
        0x04,
        0x03, 0x00, 0x00, 0x00,
        'k', 'e', 'y',
        0x04, 0x00, 0x00, 0x00,
        0x78, 0x56, 0x34, 0x12,
        0x00,
    };
    EXPECT_EQ(result, expected);
}

TEST(TestVariantDictionary, SetUint64) {
    VariantDictionary vd;
vd.set<std::uint64_t>("key", 0x1234567887654321);

    std::string result = vd.serialize();
    std::string expected = {
        0x00, 0x01,
        0x05,
        0x03, 0x00, 0x00, 0x00,
        'k', 'e', 'y',
        0x08, 0x00, 0x00, 0x00,
        0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
        0x00,
    };
    EXPECT_EQ(result, expected);
}

TEST(TestVariantDictionary, SetString) {
    VariantDictionary vd;
vd.set<std::string>("key", "hello");

    std::string result = vd.serialize();
    std::string expected = {
        0x00, 0x01,
        0x18,
        0x03, 0x00, 0x00, 0x00,
        'k', 'e', 'y',
        0x05, 0x00, 0x00, 0x00,
        'h', 'e', 'l', 'l', 'o',
        0x00,
    };
    EXPECT_EQ(result, expected);
}

TEST(TestVariantDictionary, SetByteVector) {
    VariantDictionary vd;

    type::ByteVector bytes = {'h', 'e', 'l', 'l', 'o'};
    vd.set("key", bytes);

    std::string result = vd.serialize();
    std::string expected = {
        0x00, 0x01,
        0x42,
        0x03, 0x00, 0x00, 0x00,
        'k', 'e', 'y',
        0x05, 0x00, 0x00, 0x00,
        'h', 'e', 'l', 'l', 'o',
        0x00,
    };
    EXPECT_EQ(result, expected);
}

TEST(TestVariantDictionary, EmptyDictionary) {
    VariantDictionary vd;
    EXPECT_TRUE(vd.empty());
    EXPECT_EQ(vd.size(), 0);
}

TEST(TestVariantDictionary, NonEmptyDictionary) {
    VariantDictionary vd;
    vd.set<std::int32_t>("some int", 123456);
    vd.set<std::string>("some string", "hello, world");
    vd.set<bool>("some bool", true);
    EXPECT_TRUE(!vd.empty());
    EXPECT_EQ(vd.size(), 3);
}

TEST(TestVariantDictionary, Erase) {
    VariantDictionary vd;
    vd.set<std::int32_t>("some int", 123456);
    EXPECT_EQ(vd.size(), 1);

    vd.erase("some int");
    EXPECT_TRUE(vd.empty());

    vd.set<std::int32_t>("some int", 123456);
    vd.set<std::string>("some string", "hello, world");
    EXPECT_EQ(vd.size(), 2);
    vd.erase("some string");
    EXPECT_EQ(vd.size(), 1);
}

TEST(TestVariantDictionary, Clear) {
    VariantDictionary vd;
    vd.set<std::int32_t>("some int", 123456);
    vd.set<std::string>("some string", "hello, world");
    EXPECT_TRUE(!vd.empty());

    vd.clear();
    EXPECT_TRUE(vd.empty());
}

TEST(TestVariantDictionary, Equal) {
    VariantDictionary left, right;
    left["string"] = std::string("hello, world");
    left["int32"] = std::int32_t(123456);

    right["string"] = std::string("hello, world");
    right["int32"] = std::int32_t(123456);

    EXPECT_TRUE(left == right);
}

TEST(TestVariantDictionary, Index) {
    VariantDictionary vd;
    vd["bool"] = true;
    EXPECT_TRUE(!vd.empty());
    EXPECT_EQ(vd.size(), 1);

    bool result;
    EXPECT_TRUE(vd.get("bool", result));
    EXPECT_TRUE(result);

    auto mapped_value = vd["bool"];
    bool is_same_types = std::is_same<
        decltype(mapped_value),
        VariantDictionary::mapped_type
    >::value;
    EXPECT_TRUE(is_same_types);
}

TEST(TestVariantDictionary, Count) {
    VariantDictionary vd;
    EXPECT_EQ(vd.count("bool"), 0);

    vd["bool"] = true;
    EXPECT_EQ(vd.count("bool"), 1);
    EXPECT_EQ(vd.count("another"), 0);
}
