#pragma once

#include <exception>
#include <iomanip>
#include <sstream>
#include <string>

namespace keepasslibpp::exception {

// TODO: move this to enums
enum class ErrorCode {
    NewVersionRequired,
    FileCorrupted,
    InvalidKdfParameters,
    ArgumentIsNull,
    ArgumentOutOfRange,
    NotEnoughBytes,
};

class KeePassLibError: public std:: exception {
public:
    // TODO: add namespace and etc
    explicit KeePassLibError(ErrorCode code_) : code(code_) {};

    virtual std::string what() {
        std::ostringstream s;
        s << "keepasslibpp::keepasslib_error: catch error with code ";
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

// TODO: Rename?
// TODO: Sersly? Error at top and exceptions below??
class ArgumentNullException: public KeePassLibError {
public:
    ArgumentNullException(const std::string& a)
        : KeePassLibError(ErrorCode::ArgumentIsNull)
        , argument(a) {};
private:
    const std::string argument;
};

// TODO: Rename?
class ArgumentOutOfRangeException: public KeePassLibError {
public:
    ArgumentOutOfRangeException(const std::string& a)
        : KeePassLibError(ErrorCode::ArgumentOutOfRange)
        , argument(a) {};
private:
    const std::string argument;
};

// TODO: Rename?
class NotEnoughBytesException: public KeePassLibError {
public:
    NotEnoughBytesException(const std::size_t actual,
                            const std::size_t expected)
        : KeePassLibError(ErrorCode::NotEnoughBytes)
        , actual(actual), expected(expected) {};
private:
    const std::size_t actual, expected;
};

}