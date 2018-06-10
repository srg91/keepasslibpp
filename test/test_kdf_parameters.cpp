#include "exception.hpp"
#include "kdf_parameters.hpp"
#include "uuid.hpp"
#include "typedefs.hpp"
#include "variant_dictionary.hpp"

#include <boost/test/unit_test.hpp>

#include <sstream>

using namespace keepasslibpp;

struct KdfParametersSerializationFixture {
    Uuid kdf_uuid;
    std::string sample_bytes;
    KdfParameters sample_params;

    KdfParametersSerializationFixture() : sample_params(kdf_uuid) {
        sample_params["int32"] = std::int32_t(0x12345678);

        sample_bytes = {
            // vd version
            0x00, 0x01,
            // ByteVector type
            0x42,
            // $UUID key size
            0x05, 0x00, 0x00, 0x00,
            // default key for uuid
            '$', 'U', 'U', 'I', 'D',
            // uuid size
            0x10, 0x00, 0x00, 0x00,
        };
        sample_bytes += kdf_uuid.ByteString();
        sample_bytes += {
            // another values
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
            // end
            0x00
        };
    }
};

BOOST_AUTO_TEST_SUITE(test_kdf_parameters)
    BOOST_FIXTURE_TEST_CASE(test_serializtion_to_string,
                            KdfParametersSerializationFixture) {
        std::string result = sample_params.SerializeExt();
        std::string expected = sample_bytes;
        BOOST_CHECK_EQUAL(result, expected);
    }

    BOOST_FIXTURE_TEST_CASE(test_serializtion_to_stream,
                            KdfParametersSerializationFixture) {
        std::ostringstream stream;
        sample_params.SerializeExt(stream);
        std::string expected = sample_bytes;
        BOOST_CHECK_EQUAL(stream.str(), expected);
    }

    BOOST_FIXTURE_TEST_CASE(test_deserialization_from_string,
                            KdfParametersSerializationFixture) {
        auto kp = KdfParameters::DeserializeExt(sample_bytes);
        BOOST_CHECK(kp == sample_params);
    }

    BOOST_FIXTURE_TEST_CASE(test_deserialization_from_stream,
                            KdfParametersSerializationFixture) {
        std::istringstream stream(sample_bytes);
        auto kp = KdfParameters::DeserializeExt(stream);
        BOOST_CHECK(kp == sample_params);
    }

    BOOST_AUTO_TEST_CASE(test_new_kdf_parameters)
    {
        Uuid u;
        KdfParameters kp(u);
        BOOST_CHECK_EQUAL(kp.KdfUuid(), u);

        type::ByteVector kdf_uuid;
        BOOST_CHECK(kp.get<type::ByteVector>("$UUID", kdf_uuid));
        BOOST_CHECK(kdf_uuid == u.Bytes());
    }

    BOOST_AUTO_TEST_CASE(test_invalid_kdf_parameters) {
        auto statement = []{
            VariantDictionary vd;
            auto bytes = vd.serialize();
            auto kp = KdfParameters::DeserializeExt(bytes);
        };
        BOOST_CHECK_THROW(statement(), exception::InvalidKdfParametersError);
    }
BOOST_AUTO_TEST_SUITE_END()
