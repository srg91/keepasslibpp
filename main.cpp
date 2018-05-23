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
    PwUuid u, u2;
    cout << (u != u2);
    return 0;
}