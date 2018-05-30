#pragma once

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

namespace keepasslib {
    namespace mem_util {
        using bytes = std::vector<std::uint8_t>;

        template <typename T, typename U>
        inline void assert_read_enough(T actual, U expected) {
            if (actual != expected) {
                std::ostringstream errs("keepasslib::mem_util::Read: "
                                        "not enough bytes: ");
                errs << "expected " << expected << " != " << actual;
                throw std::length_error(errs.str());
            }
        }

        // TODO: Think about endianness
        template <typename T, typename = std::enable_if_t<std::is_trivial<T>::value>>
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
            stream.read(reinterpret_cast<char *>(&value[0]), size);
            assert_read_enough(stream.gcount(), size);
            return value;
        }

        // TODO: Think about endianness
        template <typename T, typename = std::enable_if_t<std::is_trivial<T>::value>>
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
        template <typename T, typename = std::enable_if_t<std::is_trivial<T>::value>>
        void Write(std::ostream& stream, T value) {
            auto value_size = sizeof(T);
            auto it = reinterpret_cast<const char *>(&value);
            std::copy(it, it + value_size, std::ostream_iterator<char>(stream));
        }

        inline void Write(std::ostream& stream, const std::string& value) {
            stream << value;
        }

        inline void Write(std::ostream& stream, const bytes& value) {
            std::copy(value.begin(), value.end(), std::ostreambuf_iterator<char>(stream));
        }

        template <typename T, typename = std::enable_if_t<std::is_trivial<T>::value>>
        std::string Write(T value, std::size_t value_size) {
            auto it = reinterpret_cast<const char *>(&value);
            return std::string(it, it + value_size);
        }

        template <
            typename T,
            typename = std::enable_if_t<std::is_trivial<T>::value>
        >
        std::string Write(T value) {
            return Write<T>(value, sizeof(value));
        }
    };
}
