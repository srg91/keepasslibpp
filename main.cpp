#include "kdf_engine_aes.hpp"
#include "kdf_engine_argon.hpp"
#include "memory_util.hpp"
#include "uuid.hpp"
#include "kdf_parameters.hpp"
#include "variant_dictionary.hpp"

#include <gcrypt.h>
#include <openssl/sha.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <type_traits>
#include <variant>
#include <vector>
#include <memory>

using namespace std;
//using namespace keepasspp;


//ostream& operator <<(ostream& stream, const std::array<std::uint8_t, 1024 * 1024>& value) {
//    auto o = stream.flags();
//    stream << setfill('0') << hex;
//    for (std::size_t i = 0; i < value.size(); i++) {
//        stream << setw(2) << static_cast<unsigned>(static_cast<unsigned char>(value[i]));
//        auto p = i + 1;
//        if (p == 4 || p == 4 + 2 || p ==  4 + 2 + 2 || p == 4 + 2 + 2 +2)
//            stream << '-';
//    }
//    stream.setf(o);
//    return stream;
//};

ostream& operator <<(ostream& stream, keepasspp::ByteVector value) {
    auto o = stream.flags();
    stream << setfill('0') << hex;
    for (const auto& c : value) {
        stream << setw(2) << static_cast<unsigned>(static_cast<unsigned char>(c));
    }
    stream.setf(o);
    return stream;
};

// 42410
// 32480
int main() {
    keepasspp::ByteVector u(16);
    keepasspp::ByteVector key(32);

    gcry_check_version(GCRYPT_VERSION);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

//    unsigned n = 1'000'000;
//    auto start = chrono::steady_clock::now();
//    for (unsigned i = 0; i < n; i++) {
        gcry_cipher_hd_t handle;

        gcry_cipher_open(
            &handle,
            GCRY_CIPHER_AES256,
            GCRY_CIPHER_MODE_ECB,
            0
        );

        gcry_cipher_setkey(handle, std::data(key), std::size(key));

        auto error = gcry_cipher_encrypt(
            handle,
            std::data(u),
            std::size(u),
            std::data(u),
            17
        );

        if (error != 0)
            std::cerr << gcry_strsource(error) << "/" << gcry_strerror(error);
        else
            std::cout << "OK" << std::endl;

//        gcry_randomize(u.data(), 16, GCRY_WEAK_RANDOM);
//        gcry_create_nonce(std::data(u), std::size(u));
//        for (auto it = std::begin(u); it != std::end(u); ++it) {
//            gcry_create_nonce(&*it, 1);
//            gcry_randomize(&*it, 1, GCRY_STRONG_RANDOM);
//        }
//        gcry_create_nonce(std::data(u), std::size(u));
//    }

//    auto end = chrono::steady_clock::now();
//    auto ns = chrono::duration_cast<chrono::nanoseconds>(end - start);
//    cout << "count: " << (ns / n).count() << " ns/op" << endl;
//    cout << "Text: " << u <<endl;

    return 0;
}