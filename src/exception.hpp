#pragma once

#include <exception>

namespace keepasslib {
    namespace exception {
        class NewVersionRequiredError: public std::exception {};
        class FileCorruptedError: public std::exception {};
    }
}