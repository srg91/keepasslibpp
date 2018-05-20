#include "test_memutil.hpp"
#include "../KeePassLib/utility/memutil.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

template <typename T, typename U>
void AssertEqual(T left, U right) {
    if (left != right) {
        ostringstream s;
        s << "Left != Right: " << left << " != " << right;
        throw runtime_error(s.str());
    }
};

void TestWriteLE() {
    ostringstream stream;
    MemUtil::Write(stream, 5, sizeof(5));
    string out = stream.str();
    ofstream f("D:/file.txt");
    f << out;
    f.close();

    stream = ostringstream();
    stream << '\x05' << '\x00' << '\x00' << '\x00';
    string expected(stream.str());

    cout << "Len: " << expected.size() << endl;
    cout << "Size of char*: " << sizeof(expected.c_str()) << endl;
    assert(out == expected);
}