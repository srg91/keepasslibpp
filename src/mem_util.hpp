#pragma once

#include "byte_vector.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <utility>

namespace keepasslibpp::mem_util {

template <typename T, typename U>
inline void assert_read_enough(T actual, U expected) {
    if (actual != expected) {
        std::ostringstream errs("keepasslibpp::mem_util::Read: "
                                "not enough ByteVector: ");
        errs << "expected " << expected << " != " << actual;
        // TODO: change to keepasslibpp exception
        throw std::length_error(errs.str());
    }
}

// TODO: Think about endianness
template <typename T, typename = std::enable_if_t<std::is_trivial_v<T>>>
T Read(std::istream& stream) {
    T value;
    std::size_t value_size = sizeof(value);
    auto it = reinterpret_cast<char *>(&value);
    stream.read(it, value_size);
    assert_read_enough(stream.gcount(), value_size);
    return value;
};

template <typename T>
T Read(std::istream& stream, std::size_t size) {
    T value;
    value.resize(size);
    stream.read(reinterpret_cast<char *>(value.data()), size);
    assert_read_enough(stream.gcount(), size);
    return value;
}

// TODO: Think about endianness
template <typename T, typename = std::enable_if_t<std::is_trivial_v<T>>>
T Read(const std::string& s) {
    std::size_t value_size = sizeof(T);
    assert_read_enough(s.size(), value_size);

    T value;
    auto it = reinterpret_cast<char *>(&value);
    std::copy(s.begin(), s.end(), it);
    return value;
}

// TODO: We expect always Little Endian
// TODO: Add check for big-endian?
// TODO: Add function which writing size before?
template <typename T>
void Write(std::ostream& stream, T value) {
    if constexpr (std::is_trivial_v<T>) {
        auto it = reinterpret_cast<const char *>(&value);
        std::copy_n(it, sizeof(T), std::ostreambuf_iterator<char>(stream));
    } else {
        auto value_size = std::size(value);
        std::copy(std::begin(value), std::end(value),
                  std::ostreambuf_iterator<char>(stream));
    }
}

template <typename T, typename = std::enable_if_t<std::is_trivial_v<T>>>
std::string Write(T value) {
    auto it = reinterpret_cast<const char *>(&value);
    return std::string(it, it + sizeof(value));
}

}
