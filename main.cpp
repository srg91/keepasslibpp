#include "pw_uuid.hpp"

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>
#include <vector>
using namespace std;

int main() {
    try {
        PwUuid u(string(5, 0));
        cout << u << endl;
    } catch (const invalid_argument& e) {
        cerr << "Failed: " << e.what() << endl;
        return 1;
    }
    return 0;
}