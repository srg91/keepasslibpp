#include "KeePassLib/PwUuid.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <variant>
#include <vector>
using namespace std;

//inline string PrintToHex(const string& input) {
//    size_t size = input.size() * 2;
//    char buf[size+1];
//    for (int i=0; i<size; i++)
//        sprintf(buf+i*2, "%02x", static_cast<unsigned char>(input[i]));
//    buf[size]=0;
//    return std::string(buf);
//}

inline string StreamToHex(const string& input) {
    ostringstream s;
    s << setfill('0') << hex;
    for (const auto c : input) {
        s << setw(2) << static_cast<unsigned>(static_cast<unsigned char>(c));
    }
    return s.str();
}

union A {
    string A;
    int B;

    ~A() {}
};

int main() {
    std::variant<bool, int> x = true;
    cout << "Bool: " << get<bool>(x) << endl;
    x = 5;
    cout << "Int: " << get<int>(x) << endl;
//    auto zero1 = PwUuid::Zero;
//    auto zero2 = PwUuid::Zero;

//    A a = {"123456"};

//    cout << "Z: " << zero1 << endl;
//    for (unsigned i = 0; i < 100; i++) {
//        cout << PwUuid(true) << endl;
//    }

    //    unsigned n = 500000;
//    vector<string> cases = {
//        "",
//        "\x01",
//        "\x01\x02",
//        "\xc2\x15\x80\xaf\xce\x6a\x48\xa7\xbd\xf8\x7d\xb7\x18\xc3\x81\xdf",
////        string(1024, '5'),
//    };

//    {
//        for (const auto& case_ : cases) {
//            const auto start = chrono::steady_clock::now();
//            for (unsigned i = 0; i < n; i++) {
//                PrintToHex(case_);
//            }
//            const auto end = chrono::steady_clock::now();
//            const auto ns = chrono::duration_cast<chrono::nanoseconds>(end - start);
//            cout << "[PrintToHex] Case: " << PrintToHex(case_) << endl;
//            cout << "[PrintToHex] Time: " << (ns.count() / n) << " ns/op" << endl;
//        }
//    }
//    {
//        for (const auto& case_ : cases) {
//            const auto start = chrono::steady_clock::now();
//            for (unsigned i = 0; i < n; i++) {
//                StreamToHex(case_);
//            }
//            const auto end = chrono::steady_clock::now();
//            const auto ns = chrono::duration_cast<chrono::nanoseconds>(end - start);
//            cout << "[StreamToHex] Case: " << StreamToHex(case_) << endl;
//            cout << "[StreamToHex] Time: " << (ns.count() / n) << " ns/op" << endl;
//        }
//    }
    //    string s = "\xc2\x15\x80\xaf\xce\x6a\x48\xa7\xbd\xf8\x7d\xb7\x18\xc3\x81\xdf";
//    cout << "Size: " << s.size() << endl;
//    cout << "Hex: ";
//    cout << setfill('0');
//    for (const unsigned char c : s) {
//        cout << setw(2) << hex << c;
//    }
//    cout << endl;
//    cout << hex << static_cast<unsigned>(static_cast<unsigned char>('\xc2'));
//    PwUuid u(true);
//
//    std::cout << "Hello, " << u << ", World" << std::endl;
    return 0;
}