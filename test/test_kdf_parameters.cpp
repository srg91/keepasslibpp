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
BOOST_AUTO_TEST_SUITE_END()
