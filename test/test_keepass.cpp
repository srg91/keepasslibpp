#include <gcrypt.h>
#include <gtest/gtest.h>

#include <exception>

int main(int argc, char **argv) {
    // this makes sure that important subsystems are initialized
    if (!gcry_check_version(GCRYPT_VERSION)) {
        throw std::runtime_error("ligcrypt version mismatch");
    }

    // tell libgcrypt that initialization has completed
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}