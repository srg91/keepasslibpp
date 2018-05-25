#include "pw_uuid.hpp"

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

using namespace std;
using namespace keepasslib;

PwUuid U;

PwUuid&& foo() {
    return std::move(U);
}

int main() {
    PwUuid u;

    unsigned n = 1'000'000;
    auto start = chrono::steady_clock::now();
    for (unsigned i = 0; i < n; i++) {
        u = PwUuid();
    }
    auto end = chrono::steady_clock::now();
    auto ns = chrono::duration_cast<chrono::nanoseconds>(end - start);
    cout << "Count: " << (ns / n).count() << " ns/op" << endl;
    cout << "UUID: " << u << endl;
    return 0;
}