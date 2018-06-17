#include "iterator_reference.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <string>

using namespace keepasspp;

TEST(TestIteratorReference, ForwardIteratorCopyN) {
    std::string input = "1234567890";
    auto it = std::begin(input);
    auto it_ref = ForwardIteratorReference(it);

    EXPECT_EQ(*it, *it_ref);
    EXPECT_EQ(*it_ref, '1');

    std::string empty(std::size(input), '0');
    std::string expected = "1234500000";

    std::copy_n(it_ref, 5, std::begin(empty));

    EXPECT_EQ(*it, *it_ref);
    EXPECT_EQ(*it_ref, '5');
    EXPECT_EQ(empty, expected);
}