#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

namespace MemUtil {
    template <typename T>
    void Write(std::ostream &stream, T value, size_t value_size) {
        auto it = reinterpret_cast<const char *>(&value);

        for (std::size_t i = 0; i < value_size; i++) {
            stream << *(it + i);
        }
    }

    template <typename>
    void Write(std::ostream &stream, const std::string& value, size_t) {
        stream << value;
    }

    // TODO: We expect always Little Endian
    // TODO: Add check for big-endian?
    // TODO: Add function which writing size before?
    template <typename T>
    void Write(std::ostream &stream, T value) {
        Write<T>(stream, value, sizeof(value));
    }

    template <typename>
    void Write(std::ostream &stream, const std::string& value) {
        Write<std::string>(stream, value, value.size());
    }
}

