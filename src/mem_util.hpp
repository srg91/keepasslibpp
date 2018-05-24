#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

namespace MemUtil {
    template <typename T>
    T Read(std::istream& stream) {
        T value;
        std::size_t value_size = sizeof(value);
        auto it = reinterpret_cast<char *>(&value);
        stream.read(it, value_size);
        return value;
    };

    // TODO: string + size
    // TODO: bytearray + size
//    template <typename>
//    std::string Read(std::istream& stream) {
//        std::string value;
//    }
//    std::string ReadString(std::istream& stream, std::size_t size) {
//        // TODO: correct this?
//        char value[size];
//        stream.read(value, size);
//        return std::string(value);
//    }

    template <typename T>
    void Write(std::ostream& stream, T value, std::size_t value_size) {
        auto it = reinterpret_cast<const char *>(&value);

        for (std::size_t i = 0; i < value_size; i++) {
            stream << *(it + i);
        }
    }

    template <typename>
    void Write(std::ostream& stream, const std::string& value, std::size_t) {
        stream << value;
    }

    // TODO: We expect always Little Endian
    // TODO: Add check for big-endian?
    // TODO: Add function which writing size before?
    template <typename T>
    void Write(std::ostream& stream, T value) {
        Write<T>(stream, value, sizeof(value));
    }

    template <typename>
    void Write(std::ostream& stream, const std::string& value) {
        Write<std::string>(stream, value, value.size());
    }

    std::string Write(uint32_t value, std::size_t value_size) {
        auto it = reinterpret_cast<const char *>(&value);
        return std::string(it, it + value_size);
    }
}

