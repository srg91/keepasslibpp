#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iostream>
using namespace std;

int main() {
    auto tag = boost::uuids::random_generator()();

    cout << "Hello: " << tag << endl;
    return 0;
}