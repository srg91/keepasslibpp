#pragma once

#include <exception>
#include <iomanip>
#include <sstream>
#include <string>

namespace keepasspp::exception {

// TODO: move this to enums
enum class ErrorCode {
    NewVersionRequired,
    FileCorrupted,
    InvalidKdfParameters,
    ArgumentIsNull,
    ArgumentOutOfRange,
    NotEnoughBytes,
};

class KeePassError: public std:: exception {
public:
    // TODO: add namespace and etc
    explicit KeePassError(ErrorCode code_) : code(code_) {};

    virtual std::string what() {
        std::ostringstream s;
        s << "keepasspp::keepasslib_error: catch error with code ";
        s << "0x" << std::hex << std::setfill('0') << std::setw(2);
        s << static_cast<unsigned>(code);
        return s.str();
    }
private:
    ErrorCode code;
};

// TODO: Structs?
class NewVersionRequiredError: public KeePassError {
public:
    NewVersionRequiredError()
        : KeePassError(ErrorCode::NewVersionRequired) {};
};

class FileCorruptedError: public KeePassError {
public:
    FileCorruptedError()
        : KeePassError(ErrorCode::FileCorrupted) {};
};

class InvalidKdfParametersError: public KeePassError {
public:
    InvalidKdfParametersError()
        : KeePassError(ErrorCode::InvalidKdfParameters) {};
};

// TODO: Rename?
// TODO: Sersly? Error at top and exceptions below??
class ArgumentNullException: public KeePassError {
public:
    ArgumentNullException(const std::string& a)
        : KeePassError(ErrorCode::ArgumentIsNull)
        , argument(a) {};
private:
    const std::string argument;
};

// TODO: Rename?
class ArgumentOutOfRangeException: public KeePassError {
public:
    ArgumentOutOfRangeException(const std::string& a)
        : KeePassError(ErrorCode::ArgumentOutOfRange)
        , argument(a) {};
private:
    const std::string argument;
};

// TODO: Rename?
class NotEnoughBytesException: public KeePassError {
public:
    NotEnoughBytesException(const std::size_t actual,
                            const std::size_t expected)
        : KeePassError(ErrorCode::NotEnoughBytes)
        , actual(actual), expected(expected) {};
private:
    const std::size_t actual, expected;
};

}