#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

namespace keepasslib {
    namespace mem_util {
        // TODO: Think about endianness
        template <typename T>
        T Read(std::istream& stream) {
            T value;
            std::size_t value_size = sizeof(value);
            auto it = reinterpret_cast<char *>(&value);
            stream.read(it, value_size);
            return value;
        };

        template <typename T>
        T Read(std::istream& stream, std::size_t size) {
            T result;
            std::copy_n(
                std::istreambuf_iterator<typename T::value_type>(stream), size,
                std::back_inserter(result)
            );
            return result;
        }

        template <typename T>
        T Read(const std::string& s) {
            std::size_t value_size = sizeof(T);
            if (s.size() != value_size) {
                std::ostringstream errs("invalid string size: ");
                errs << "expected " << value_size << " != " << s.size();
                throw std::invalid_argument(errs.str());
            }
            // TODO: Think about endianness
            T value;
            auto it = reinterpret_cast<char *>(&value);
            std::copy(s.begin(), s.end(), it);
            return value;
        }

        // TODO: Think about endianness
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
}

