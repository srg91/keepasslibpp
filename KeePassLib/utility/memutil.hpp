#pragma once

#include <cstddef>
#include <iostream>

// TODO: Is in memory always LE?
namespace MemUtil {
//    template <typename T> void WriteLE(std::ostream& stream, T value);

    template <typename T>
    void WriteLE(std::ostream &stream, T value) {
        std::size_t s = sizeof(value);
        for (std::size_t i = 0; i < s; i++) {
            int* c = &value + (s - i - 1);
            stream << (*reinterpret_cast<char *>(c));
        }
    }
}

