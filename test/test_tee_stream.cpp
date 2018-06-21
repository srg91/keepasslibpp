#include "tee_stream.hpp"
#include "tee_stream_buf.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

using namespace keepasspp;

// TODO: add test when you can read nothing
// TODO: add tests when you can't write nothing (or just part)

// TODO: Simplify? Deduplicate?
TEST(TeeStream, ReadIntegers) {
    std::istringstream is("1 2 3");
    std::ostringstream os;

    TeeStream ts(is, os);

    int i = 0;
    EXPECT_TRUE(ts >> i);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(os.str(), std::string("1"));

    EXPECT_TRUE(ts >> i);
    EXPECT_EQ(i, 2);
    EXPECT_EQ(os.str(), std::string("1 2"));

    EXPECT_TRUE(ts >> i);
    EXPECT_EQ(i, 3);

    std::string expected = "1 2 3";
    EXPECT_EQ(os.str(), expected);
}