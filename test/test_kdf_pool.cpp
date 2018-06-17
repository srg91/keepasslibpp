#include "byte_vector.hpp"
#include "kdf_engine.hpp"
#include "kdf_parameters.hpp"
#include "kdf_pool.hpp"
#include "uuid.hpp"

#include <gtest/gtest.h>

#include <iterator>
#include <memory>

using namespace keepasspp;

class SomeKdfEngine : public KdfEngine {
public:
    explicit SomeKdfEngine(Uuid uuid)
        : uuid(uuid) {};

    const Uuid& getUuid() const override {
        return uuid;
    }

    void randomize(KdfParameters& kp) const override {};
    ByteVector transform(const ByteVector& msg,
                         const KdfParameters& kp) const override {};
private:
    Uuid uuid;
};

TEST(TestKdfPool, AddKdfEngine) {
    EXPECT_EQ(KdfPool::size(), 2);

    Uuid uuid;
    KdfPool::add(std::make_shared<SomeKdfEngine>(uuid));
    EXPECT_EQ(KdfPool::size(), 3);
}

TEST(TestKdfPool, GetKdfEngineAes) {
    auto aes_uuid = Uuid::fromByteVector({
        0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
        0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea
    });

    auto engine = KdfPool::get(aes_uuid);
    EXPECT_TRUE(engine);
    EXPECT_EQ(engine->getUuid(), aes_uuid);
}

TEST(TestKdfPool, GetKdfEngineArgon) {
    auto argon_uuid = Uuid::fromByteVector({
        0xef, 0x63, 0x6d, 0xdf, 0x8c, 0x29, 0x44, 0x4b,
        0x91, 0xf7, 0xa9, 0xa4, 0x03, 0xe3, 0x0a, 0x0c
    });

    auto engine = KdfPool::get(argon_uuid);
    EXPECT_TRUE(engine);
    EXPECT_EQ(engine->getUuid(), argon_uuid);
}

TEST(TestKdfPool, GetCustonKdfEngine) {
    auto uuid = Uuid::fromByteVector({
        0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
        0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
    });

    auto engine = KdfPool::get(uuid);
    EXPECT_FALSE(engine);

    KdfPool::add(std::make_shared<SomeKdfEngine>(uuid));
    engine = KdfPool::get(uuid);
    EXPECT_TRUE(engine);
    EXPECT_EQ(engine->getUuid(), uuid);
}

TEST(TestKdfPool, GetDefaultParameters) {
    auto aes_uuid = Uuid::fromByteVector({
        0xc9, 0xd9, 0xf3, 0x9a, 0x62, 0x8a, 0x44, 0x60,
        0xbf, 0x74, 0x0d, 0x08, 0xc1, 0x8a, 0x4f, 0xea
    });

    auto params = KdfPool::getDefaultParameters();
    EXPECT_EQ(std::size(params), 2);

    auto uuid = params.get<ByteVector>("$UUID");
    EXPECT_EQ(aes_uuid.byteVector(), uuid);

    auto rounds = params.get<std::uint64_t>("R");
    EXPECT_EQ(rounds, 6000);
}
