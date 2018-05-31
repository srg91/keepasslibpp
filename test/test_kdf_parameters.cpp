#include "pw_uuid.hpp"
#include "kdf_parameters.hpp"
#include "typedefs.hpp"

#include <boost/test/unit_test.hpp>

using namespace keepasslib;

BOOST_AUTO_TEST_SUITE(test_kdf_parameters)
    BOOST_AUTO_TEST_CASE(test_new_kdf_parameters)
    {
        PwUuid u;
        KdfParameters kp(u);
        BOOST_CHECK_EQUAL(kp.KdfUuid(), u);

        types::bytes kdf_uuid;
        BOOST_CHECK(kp.Get<types::bytes>("$UUID", kdf_uuid));
        BOOST_CHECK(kdf_uuid == u.Bytes());
    }

    // TODO: Fixture
    BOOST_AUTO_TEST_CASE(test_deserialization_ext_from_string) {
        std::string s = {
            0x00, 0x01,
            0x42,
            0x05, 0x00, 0x00, 0x00,
            '$', 'U', 'U', 'I', 'D',
            0x10, 0x00, 0x00, 0x00,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x00
        };
        auto kp = KdfParameters::DeserializeExt(s);
    }
BOOST_AUTO_TEST_SUITE_END()
