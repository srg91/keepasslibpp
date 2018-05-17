#include "test_memutil.hpp"
#include "../KeePassLib/utility/memutil.hpp"

#include <cassert>
#include <sstream>
#include <string>
using namespace std;

void TestWriteLE() {
    ostringstream stream;
    MemUtil::WriteLE(stream, 5);
    string out = stream.str();
    assert(out == "\x05\x00\x00\x00");
}