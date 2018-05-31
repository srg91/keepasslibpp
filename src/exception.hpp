#pragma once

#include <exception>
#include <iomanip>
#include <sstream>

namespace keepasslib {
    namespace exception {
        // TODO: move this to enums
        enum class ErrorCode {
            NewVersionRequired,
            FileCorrupted,
            InvalidKdfParameters,
        };

        class KeePassLibError: public std:: exception {
        public:
            // TODO: add namespace and etc
            explicit KeePassLibError(ErrorCode code_) : code(code_) {};

            virtual std::string what() {
                std::ostringstream s;
                s << "keepasslib::keepasslib_error: catch error with code ";
                s << "0x" << std::hex << std::setfill('0') << std::setw(2);
                s << static_cast<unsigned>(code);
                return s.str();
            }
        private:
            ErrorCode code;
        };

        // TODO: Structs?
        class NewVersionRequiredError: public KeePassLibError {
        public:
            NewVersionRequiredError()
                : KeePassLibError(ErrorCode::NewVersionRequired) {};
        };

        class FileCorruptedError: public KeePassLibError {
        public:
            FileCorruptedError()
                : KeePassLibError(ErrorCode::FileCorrupted) {};
        };

        class InvalidKdfParametersError: public KeePassLibError {
        public:
            InvalidKdfParametersError()
                : KeePassLibError(ErrorCode::InvalidKdfParameters) {};
        };
    }
}