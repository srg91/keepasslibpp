#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

namespace MemUtil {
    // TODO: We expect always Little Endian
    // TODO: Add check for big-endian?
    template <typename T>
    void Write(std::ostream &stream, T value) {
        std::size_t value_size = sizeof(value);
        auto it = reinterpret_cast<const char *>(&value);

        for (std::size_t i = 0; i < value_size; i++) {
            stream << *(it + i);
        }
    }

    // TODO: Make it more simple?
    template <typename>
    void Write(std::ostream &stream, const std::string& value) { stream << value; }

    template <typename>
    void Write(std::ostream &stream, const char* value) { stream << value; }
}

