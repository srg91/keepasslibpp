#include "byte_vector.hpp"
#include "exception.hpp"
#include "kdf_parameters.hpp"
#include "uuid.hpp"
#include "variant_dictionary.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace keepasspp;

struct TestKdfParameters : public ::testing::Test {
    Uuid kdf_uuid;
    std::string sample_bytes;
    KdfParameters sample_params = KdfParameters(kdf_uuid);

    void SetUp() override {
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
        sample_bytes += kdf_uuid.string();
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
    };
};

TEST_F(TestKdfParameters, SerializationToString) {
    std::string result = sample_params.SerializeExt();
    std::string expected = sample_bytes;
    EXPECT_EQ(result, expected);
}

TEST_F(TestKdfParameters, SerializationToStream) {
    std::ostringstream stream;
    sample_params.SerializeExt(stream);
    std::string expected = sample_bytes;
    EXPECT_EQ(stream.str(), expected);
}

TEST_F(TestKdfParameters, DeserializationFromString) {
    auto kp = KdfParameters::DeserializeExt(sample_bytes);
    EXPECT_EQ(kp, sample_params);
}

TEST_F(TestKdfParameters, DeserializationFromStream) {
    std::istringstream stream(sample_bytes);
    auto kp = KdfParameters::DeserializeExt(stream);
    EXPECT_EQ(kp, sample_params);
}

TEST_F(TestKdfParameters, NewKdfParameters)
{
    Uuid u;
    KdfParameters kp(u);
    EXPECT_EQ(kp.KdfUuid(), u);

    ByteVector kdf_uuid;
    EXPECT_TRUE(kp.get<ByteVector>("$UUID", kdf_uuid));
    EXPECT_EQ(kdf_uuid, u.byteVector());
}

TEST_F(TestKdfParameters, InvalidKdfParameters) {
    auto statement = []{
        VariantDictionary vd;
        auto bytes = vd.serialize();
        auto kp = KdfParameters::DeserializeExt(bytes);
    };
    EXPECT_THROW(statement(), exception::InvalidKdfParametersError);
}
