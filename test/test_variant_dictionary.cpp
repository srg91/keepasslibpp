#include "exception.hpp"
#include "variant_dictionary.hpp"
#include "typedefs.hpp"

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

using namespace keepasslib;

struct VariantDictionarySerializationFixture {
    std::string sample_bytes;
    VariantDictionary sample_dict;

    VariantDictionarySerializationFixture() {
        sample_dict["true_bool"] = true;
        sample_dict["false_bool"] = false;
        sample_dict["int32"] = std::int32_t(0x12345678);
        sample_dict["int64"] = std::int64_t(0x1234567887654321);
        sample_dict["uint32"] = std::uint32_t(0x12345678);
        sample_dict["uint64"] = std::uint64_t(0x1234567887654321);
        sample_dict["string"] = std::string("hello, world");
        sample_dict["bytes"] = types::bytes(
            {'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'}
        );

        sample_bytes = {
            // vd version
            0x00, 0x01,
            // bytes type
            0x42,
            // bytes key size
            0x05, 0x00, 0x00, 0x00,
            // bytes key
            'b', 'y', 't', 'e', 's',
            // bytes value size
            0x0c, 0x00, 0x00, 0x00,
            // bytes value
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

BOOST_AUTO_TEST_SUITE(test_variant_dictionary)
    BOOST_FIXTURE_TEST_CASE(test_serialization_to_string,
                            VariantDictionarySerializationFixture) {
        std::string result = sample_dict.Serialize();
        std::string expected = sample_bytes;
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_FIXTURE_TEST_CASE(test_serialization_to_stream,
                            VariantDictionarySerializationFixture) {
        std::ostringstream stream;
        sample_dict.Serialize(stream);
        std::string expected = sample_bytes;
        BOOST_CHECK_EQUAL(stream.str(), expected);
    }

    BOOST_FIXTURE_TEST_CASE(test_deserialization_from_string,
                            VariantDictionarySerializationFixture) {
        auto vd = VariantDictionary::Deserialize(sample_bytes);
        // TODO: Implement << operator?
        BOOST_CHECK(vd == sample_dict);
    }

    BOOST_FIXTURE_TEST_CASE(test_deserialization_from_stream,
                            VariantDictionarySerializationFixture) {
        std::istringstream stream(sample_bytes);
        auto vd = VariantDictionary::Deserialize(stream);
        // TODO: Implement << operator?
        BOOST_CHECK(vd == sample_dict);
    }

    BOOST_AUTO_TEST_CASE(test_deserialization_empty_dictionary) {
        std::string s = {0x00, 0x01, 0x00};
        auto vd = VariantDictionary::Deserialize(s);
        BOOST_CHECK(vd.Empty());
        BOOST_CHECK_EQUAL(vd.Size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_deserialization_empty_file) {
        BOOST_CHECK_THROW(VariantDictionary::Deserialize(""), exception::FileCorruptedError);
    }

    BOOST_AUTO_TEST_CASE(test_deserialization_newest_file) {
        // Newest version - 0x02
        auto statement_newest_version = []{
            std::string s = {0x00, 0x02};
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_newest_version(), exception::NewVersionRequiredError);

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
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_new_unknown_type(), exception::NewVersionRequiredError);
    }

    BOOST_AUTO_TEST_CASE(test_deserialization_corrupted_type) {
        auto statement_corrupted_type = []{
            std::string s = {0x00, 0x01, -0x01};
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_corrupted_type(), exception::FileCorruptedError);
    }

    BOOST_AUTO_TEST_CASE(test_deserialization_unexpected_end_on_type) {
        std::string s = {
            0x00, 0x01,
            0x08,
            0x04, 0x00, 0x00, 0x00,
            'b', 'o', 'o', 'l',
            0x01, 0x00, 0x00, 0x00,
            0x01
            // 0x00 is expected
        };
        BOOST_CHECK_THROW(VariantDictionary::Deserialize(s), exception::FileCorruptedError);
    }

    BOOST_AUTO_TEST_CASE(test_deserialization_unexpected_end_on_reading) {
        auto statement_failed_on_type = []{
            std::string s = {0x00, 0x01};
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_failed_on_type(), exception::FileCorruptedError);

        auto statement_failed_on_key_size = []{
            std::string s = {0x00, 0x01, 0x08, 0x04};
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_failed_on_key_size(), exception::FileCorruptedError);

        auto statement_failed_on_key = []{
            std::string s = {0x00, 0x01, 0x08, 0x04, 0x00, 0x00, 0x00, 'k', 'e', 'y'};
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_failed_on_key(), exception::FileCorruptedError);

        auto statement_failed_on_value_size = []{
            std::string s = {
                0x00, 0x01,
                0x08,
                0x04, 0x00, 0x00, 0x00,
                'b', 'o', 'o', 'l',
                0x04
            };
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_failed_on_value_size(), exception::FileCorruptedError);

        auto statement_failed_on_value = []{
            std::string s = {
                0x00, 0x01,
                0x18,
                0x03, 0x00, 0x00, 0x00,
                's', 't', 'r',
                0x04, 0x00, 0x00, 0x00,
                'v', 'a', 'l'
            };
            VariantDictionary::Deserialize(s);
        };
        BOOST_CHECK_THROW(statement_failed_on_value(), exception::FileCorruptedError);
    }

    BOOST_FIXTURE_TEST_CASE(test_get_bool,
                            VariantDictionarySerializationFixture) {
        bool result;
        BOOST_CHECK(sample_dict.Get("true_bool", result));
        BOOST_CHECK(result);

        BOOST_CHECK(sample_dict.Get("false_bool", result));
        BOOST_CHECK(!result);
    }

    BOOST_FIXTURE_TEST_CASE(test_get_int32,
                            VariantDictionarySerializationFixture) {
        std::int32_t result;
        BOOST_CHECK(sample_dict.Get("int32", result));
        BOOST_CHECK_EQUAL(result, 0x12345678);
    }

    BOOST_FIXTURE_TEST_CASE(test_get_int64,
                            VariantDictionarySerializationFixture) {
        std::int64_t result;
        BOOST_CHECK(sample_dict.Get("int64", result));
        BOOST_CHECK_EQUAL(result, 0x1234567887654321);
    }

    BOOST_FIXTURE_TEST_CASE(test_get_uint32,
                            VariantDictionarySerializationFixture) {
        std::uint32_t result;
        BOOST_CHECK(sample_dict.Get("uint32", result));
        BOOST_CHECK_EQUAL(result, 0x12345678);
    }

    BOOST_FIXTURE_TEST_CASE(test_get_uint64,
                            VariantDictionarySerializationFixture) {
        std::uint64_t result;
        BOOST_CHECK(sample_dict.Get("uint64", result));
        BOOST_CHECK_EQUAL(result, 0x1234567887654321);
    }

    BOOST_FIXTURE_TEST_CASE(test_get_string,
                            VariantDictionarySerializationFixture) {
        std::string result;
        BOOST_CHECK(sample_dict.Get<std::string>("string", result));
        BOOST_CHECK_EQUAL(result, "hello, world");
    }

    BOOST_FIXTURE_TEST_CASE(test_get_bytes,
                            VariantDictionarySerializationFixture) {
        types::bytes result;
        BOOST_CHECK(sample_dict.Get("bytes", result));

        types::bytes expected = {
            'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'
        };
        BOOST_CHECK(result == expected);
    }

    BOOST_AUTO_TEST_CASE(test_get_nothing) {
        VariantDictionary vd;
        std::int32_t value;
        bool success = vd.Get<std::int32_t>("unknown_key", value);
        BOOST_CHECK(!success);
    }

    BOOST_AUTO_TEST_CASE(test_set_bool) {
        VariantDictionary vd;
        vd.Set<bool>("key", true);

        std::string result = vd.Serialize();
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
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_set_int32) {
        VariantDictionary vd;
        vd.Set<std::int32_t>("key", 0x12345678);

        std::string result = vd.Serialize();
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
        BOOST_TEST_MESSAGE(s.str());
        BOOST_TEST_MESSAGE(d.str());

        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_set_int64) {
        VariantDictionary vd;
        vd.Set<std::int64_t>("key", 0x1234567887654321);

        std::string result = vd.Serialize();
        std::string expected = {
            0x00, 0x01,
            0x0d,
            0x03, 0x00, 0x00, 0x00,
            'k', 'e', 'y',
            0x08, 0x00, 0x00, 0x00,
            0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
            0x00,
        };
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_set_uint32) {
        VariantDictionary vd;
        vd.Set<std::uint32_t>("key", 0x12345678);

        std::string result = vd.Serialize();
        std::string expected = {
            0x00, 0x01,
            0x04,
            0x03, 0x00, 0x00, 0x00,
            'k', 'e', 'y',
            0x04, 0x00, 0x00, 0x00,
            0x78, 0x56, 0x34, 0x12,
            0x00,
        };
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_set_uint64) {
        VariantDictionary vd;
        vd.Set<std::uint64_t>("key", 0x1234567887654321);

        std::string result = vd.Serialize();
        std::string expected = {
            0x00, 0x01,
            0x05,
            0x03, 0x00, 0x00, 0x00,
            'k', 'e', 'y',
            0x08, 0x00, 0x00, 0x00,
            0x21, 0x43, 0x65, -0x79, 0x78, 0x56, 0x34, 0x12,
            0x00,
        };
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_set_string) {
        VariantDictionary vd;
        vd.Set<std::string>("key", "hello");

        std::string result = vd.Serialize();
        std::string expected = {
            0x00, 0x01,
            0x18,
            0x03, 0x00, 0x00, 0x00,
            'k', 'e', 'y',
            0x05, 0x00, 0x00, 0x00,
            'h', 'e', 'l', 'l', 'o',
            0x00,
        };
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_set_bytes) {
        VariantDictionary vd;

        types::bytes bytes = {'h', 'e', 'l', 'l', 'o'};
        vd.Set("key", bytes);

        std::string result = vd.Serialize();
        std::string expected = {
            0x00, 0x01,
            0x42,
            0x03, 0x00, 0x00, 0x00,
            'k', 'e', 'y',
            0x05, 0x00, 0x00, 0x00,
            'h', 'e', 'l', 'l', 'o',
            0x00,
        };
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_AUTO_TEST_CASE(test_empty_dictionary) {
        VariantDictionary vd;
        BOOST_CHECK(vd.Empty());
        BOOST_CHECK_EQUAL(vd.Size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_non_empty_dictionary) {
        VariantDictionary vd;
        vd.Set<std::int32_t>("some int", 123456);
        vd.Set<std::string>("some string", "hello, world");
        vd.Set<bool>("some bool", true);
        BOOST_CHECK(!vd.Empty());
        BOOST_CHECK_EQUAL(vd.Size(), 3);
    }

    BOOST_AUTO_TEST_CASE(test_erase) {
        VariantDictionary vd;
        vd.Set<std::int32_t>("some int", 123456);
        BOOST_CHECK_EQUAL(vd.Size(), 1);

        vd.Erase("some int");
        BOOST_CHECK(vd.Empty());

        vd.Set<std::int32_t>("some int", 123456);
        vd.Set<std::string>("some string", "hello, world");
        BOOST_CHECK_EQUAL(vd.Size(), 2);
        vd.Erase("some string");
        BOOST_CHECK_EQUAL(vd.Size(), 1);
    }

    BOOST_AUTO_TEST_CASE(test_clear) {
        VariantDictionary vd;
        vd.Set<std::int32_t>("some int", 123456);
        vd.Set<std::string>("some string", "hello, world");
        BOOST_CHECK(!vd.Empty());

        vd.Clear();
        BOOST_CHECK(vd.Empty());
    }

    BOOST_AUTO_TEST_CASE(test_equal) {
        VariantDictionary left, right;
        left["string"] = std::string("hello, world");
        left["int32"] = std::int32_t(123456);

        right["string"] = std::string("hello, world");
        right["int32"] = std::int32_t(123456);

        BOOST_CHECK(left == right);
    }

    BOOST_AUTO_TEST_CASE(test_index) {
        VariantDictionary vd;
        vd["bool"] = true;
        BOOST_CHECK(!vd.Empty());
        BOOST_CHECK_EQUAL(vd.Size(), 1);

        bool result;
        BOOST_CHECK(vd.Get("bool", result));
        BOOST_CHECK(result);

        auto mapped_value = vd["bool"];
        bool is_same_types = std::is_same<
            decltype(mapped_value),
            VariantDictionary::mapped_type
        >::value;
        BOOST_CHECK(is_same_types);
    }
BOOST_AUTO_TEST_SUITE_END()
