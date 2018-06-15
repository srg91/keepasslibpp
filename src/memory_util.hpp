#pragma once

#include "byte_vector.hpp"
#include "exception.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace keepasspp {

class MemoryUtil {
public:
    template <
        typename T,
        typename = std::enable_if_t<std::is_trivial_v<T>>,
        typename = std::enable_if_t<__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__>
    >
    static T read(std::istream& stream);

    template <typename T>
    static T read(std::istream& stream, std::size_t count);

    template <typename T>
    static T read(const std::string& s);

    template <typename T>
    static void write(std::ostream& stream, T value);

    template <typename T>
    static std::string write(T value);
private:
    // TODO: rename to something like assertEnoughBytes
    template <typename T, typename U>
    inline static void assertEnoughBytes(T actual, U expected) {
        if (actual != expected)
            throw exception::NotEnoughBytesException(actual, expected);
    };
};

template <typename T, typename, typename>
T MemoryUtil::read(std::istream& stream) {
    T value;
    std::size_t value_size = sizeof(T);
    stream.read(reinterpret_cast<char*>(&value), value_size);
    MemoryUtil::assertEnoughBytes(stream.gcount(), value_size);
    return value;
}

template <typename T>
T MemoryUtil::read(std::istream& stream, std::size_t count) {
    T value;
    value.resize(count);
    stream.read(reinterpret_cast<char*>(std::data(value)), std::size(value));
    MemoryUtil::assertEnoughBytes(stream.gcount(), std::size(value));
    return value;
}

template <typename T>
T MemoryUtil::read(const std::string& s) {
    std::size_t value_size = sizeof(T);
    MemoryUtil::assertEnoughBytes(std::size(s), value_size);

    T value;
    auto it = reinterpret_cast<char*>(&value);
    std::copy(std::begin(s), std::end(s), it);
    return value;
}

template <typename T>
void MemoryUtil::write(std::ostream& stream, T value) {
    if constexpr (std::is_trivial_v<T>) {
        auto it = reinterpret_cast<const char *>(&value);
        std::copy_n(it, sizeof(T), std::ostreambuf_iterator<char>(stream));
    } else {
        auto value_size = std::size(value);
        std::copy(std::begin(value), std::end(value),
                  std::ostreambuf_iterator<char>(stream));
    }
}

template <typename T>
std::string MemoryUtil::write(T value) {
    auto it = reinterpret_cast<const char*>(&value);
    return std::string(it, it + sizeof(value));
}

}
