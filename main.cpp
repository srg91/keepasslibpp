//#include "pw_uuid.hpp"
#include "mem_util.hpp"
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
    std::istringstream s("\x0f\x00");
    auto v = keepasslib::mem_util::Read<std::int32_t>(s);
    cout << "V: " << v;
//    ostringstream s;
//    keepasslib::mem_util::Write(s, std::string("Hello, world"));
//    cout << s.str() << endl;
//    ofstream f("d:/myvd.txt");
//    cout << std::is_trivial<std::basic_string<char>>::value << endl;
//    keepasslib::mem_util::Write(cout, std::string("Hello, world"));
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