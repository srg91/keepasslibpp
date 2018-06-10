#include "crypto_util.hpp"
#include "kdf_engine_aes.hpp"
#include "kdf_engine_argon.hpp"
#include "mem_util.hpp"
#include "uuid.hpp"
#include "kdf_parameters.hpp"
#include "variant_dictionary.hpp"

//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/random_generator.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include <gcrypt.h>
#include <openssl/sha.h>

#include <algorithm>
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

using namespace std;
//using namespace keepasslibpp;


ostream& operator <<(ostream& stream, keepasslibpp::type::ByteVector value) {
    auto o = stream.flags();
    stream << setfill('0') << hex;
    for (const auto& c : value) {
        stream << setw(2) << static_cast<unsigned>(static_cast<unsigned char>(c));
    }
    stream.setf(o);
    return stream;
};

int main() {
//    keepasslibpp::type::ByteVector S = {'1', '2', '3', '4', '5', '6', '7', '8'};
//    keepasslibpp::type::ByteVector msg = {'H', 'e', 'l', 'l', 'o'};
//
//    keepasslibpp::AesKdf aes_kdf;
//    keepasslibpp::KdfParameters aes_kp(aes_kdf.GetUuid());
//    aes_kp.set<keepasslibpp::type::ByteVector>("S", S);
//    aes_kp.set<std::uint64_t>("R", 6000);
//
//    cout << "AES: " << aes_kdf.Transform(msg, aes_kp) << endl;
//
//    keepasslibpp::Argon2Kdf argon_kdf;
//    keepasslibpp::KdfParameters argon_kp(argon_kdf.GetUuid());
//    argon_kp[keepasslibpp::Argon2Kdf::ParamSalt] = S;
//    argon_kp[keepasslibpp::Argon2Kdf::ParamParallelism] = std::uint32_t(2);
//    argon_kp[keepasslibpp::Argon2Kdf::ParamMemory] = std::uint64_t(1024 * 1024);
//    argon_kp[keepasslibpp::Argon2Kdf::ParamIterations] = std::uint64_t(2);
//    argon_kp[keepasslibpp::Argon2Kdf::ParamVersion] = std::uint32_t(0x13);
//
//    cout << "Argon2: " << argon_kdf.Transform(msg, argon_kp) << endl;

//    cout << "Hello!" << endl;
//    std::istringstream s("asdf123");
//    std::vector<std::uint8_t> v;
//    v.resize(7);
//    s.read(reinterpret_cast<char *>(&v[0]), 7);
//    std::cout << v[3];

//    ostringstream s;
//    keepasslibpp::mem_util::Write(s, std::string("Hello, world"));
//    cout << s.str() << endl;
//    ofstream f("d:/myvd.txt");
//    cout << std::is_trivial<std::basic_string<char>>::value << endl;
//    keepasslibpp::mem_util::Write(cout, std::string("Hello, world"));
//
//    keepasslibpp::VariantDictionary sample_dict;
//    sample_dict["true_bool"] = true;
//    sample_dict["false_bool"] = false;
//    sample_dict["int32"] = std::int32_t(0x12345678);
//    sample_dict["int64"] = std::int64_t(0x1234567887654321);
//    sample_dict["uint32"] = std::uint32_t(0x12345678);
//    sample_dict["uint64"] = std::uint64_t(0x1234567887654321);
//    sample_dict["string"] = std::string("hello, world");
//    sample_dict["ByteVector"] = keepasslibpp::VariantDictionary::ByteVector(
//        {'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'}
//    );
//
//    f << sample_dict.serialize();
//    f.close();

//    vt v = true;
//    auto vv = visitor_t();
//    v.apply_visitor(vv);
//    string s = "1234567";
//    std::copy(
//        s.begin(),
//        s.end(),
//        std::ostreambuf_iterator<string::value_type>(cout)
//    );

//    ifstream f("e:\\srg91\\downloads\\Kick-Ass.2010.DUB.no.censors.XviD.AC3.BDRip._by.ZNG505_.avi", ios::in | ios::binary);
//
//    f.seekg(0, ios::end);
//    auto size = f.tellg();
//    f.seekg(0, ios::beg);
//    auto start = chrono::steady_clock::now();
//    auto s = keepasslibpp::mem_util::Read<std::string>(f, static_cast<std::size_t>(size));
//    stringstream s;
//    s << f.rdbuf();
//    auto end = chrono::steady_clock::now();
//    auto time = chrono::duration_cast<chrono::milliseconds>(end - start);
//    cout << "size: " << s.size() << endl;
//    cout << "time: " << time.count() << " ms" << endl;
//    istringstream source("hello, world");
//    string dest;

//    keepasslibpp::type::ByteVector data = {'H', 'e', 'l', 'l', 'o'};
//    keepasslibpp::type::ByteVector dest(32, 0);


//    gcry_md_hd_t handle;
//    gcry_md_open(&handle, GCRY_MD_SHA256, 0);

//    gcry_buffer_t data_buffer = {
//        data.size(),
//        0,
//        data.size(),
//        &data[0]
//    };

//    unsigned n = 1'000'000;
//    auto start = chrono::steady_clock::now();
//    for (unsigned i = 0; i < n; i++) {

//        gcry_md_write(handle, &data[0], data.size());
//        std::memcpy(&dest[0], gcry_md_read(handle, GCRY_MD_SHA256), 32);
//        gcry_md_reset(handle);
//        gcry_md_hash_buffers
//    }

//    gcry_md_close(handle);
//    auto end = chrono::steady_clock::now();
//    auto ns = chrono::duration_cast<chrono::nanoseconds>(end - start);
//    cout << "count: " << (ns / n).count() << " ns/op" << endl;
//    cout << "Text: " << dest <<endl;
    return 0;
}