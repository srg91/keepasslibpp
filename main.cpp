//#include "pw_uuid.hpp"
#include "variant_dictionary.hpp"

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

//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/random_generator.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

using namespace std;
//using namespace keepasslib;

using vt = boost::variant<int, bool>;

using my_int = int;

struct visitor_t : public boost::static_visitor<>
{
    void operator()(int i) const {
        cout << "found int i: " << i << endl;
    }
//    void operator()(std::int32_t j) const {
//        cout << "found my_int j: " << j << endl;
//    }
};

int main() {
    std::string v = {
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
    istringstream s(v);
    auto vd = keepasslib::VariantDictionary::Deserialize(s);
    cout << vd.Size() << endl;
    bool nt;
    if (vd.Get<bool>("true_bool", nt)) cout << "true bool: " << nt << endl;
//    ofstream f("d:/myvd.txt");
//
//    keepasslib::VariantDictionary sample_dict;
//    sample_dict["true_bool"] = true;
//    sample_dict["false_bool"] = false;
//    sample_dict["int32"] = std::int32_t(0x12345678);
//    sample_dict["int64"] = std::int64_t(0x1234567887654321);
//    sample_dict["uint32"] = std::uint32_t(0x12345678);
//    sample_dict["uint64"] = std::uint64_t(0x1234567887654321);
//    sample_dict["string"] = std::string("hello, world");
//    sample_dict["bytes"] = keepasslib::VariantDictionary::bytes(
//        {'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'}
//    );
//
//    f << sample_dict.Serialize();
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

//    istringstream source("hello, world");
//    string dest;
//    unsigned n = 1;
//    auto start = chrono::steady_clock::now();
//    for (unsigned i = 0; i < n; i++) {
//        dest.resize(12);
//        source.read(&dest[0], 12);
//
////        source.clear();
////        dest.clear();
////        copy_n(istreambuf_iterator<char>(source), 12, back_inserter(dest));
//    }
//    auto end = chrono::steady_clock::now();
//    auto ns = chrono::duration_cast<chrono::nanoseconds>(end - start);
//    cout << "Count: " << (ns / n).count() << " ns/op" << endl;
//    cout << "Text: " << dest <<endl;
    return 0;
}