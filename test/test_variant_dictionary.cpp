#include "variant_dictionary.hpp"

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <unordered_map>

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

        sample_bytes = {
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
        };
    }
};

BOOST_AUTO_TEST_SUITE(test_variant_dictionary)
    BOOST_AUTO_TEST_CASE(test_empty_dictionary)
    {
        VariantDictionary vd;
        BOOST_CHECK(vd.Empty());
        BOOST_CHECK_EQUAL(vd.Size(), 0);
    }

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
BOOST_AUTO_TEST_SUITE_END()
