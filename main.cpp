#include "pw_uuid.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <variant>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

using namespace std;
using namespace keepasslib;

int main() {
    cout << static_cast<int>(static_cast<char>(0xfd)) << endl;
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