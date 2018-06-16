#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace keepasspp::exception {

class KeePassError: public std:: exception {
public:
    KeePassError()
        : namespace_("keepasspp")
        , name("KeePassError")
        , msg("unknown error") {};

    explicit KeePassError(std::string_view message)
        : namespace_("keepasspp")
        , name("KeePassError")
        , msg(message) {};

    explicit KeePassError(std::string_view error_name, std::string_view message)
        : namespace_("keepasspp")
        , name(error_name)
        , msg(message) {};

    explicit KeePassError(std::string_view namespace_,
                          std::string_view error_name,
                          std::string_view message)
        : namespace_(namespace_)
        , name(error_name)
        , msg(message) {};

    virtual std::string what() {
        std::ostringstream s;
        s << namespace_ << "::" << name << ": " << msg;
        return s.str();
    }
protected:
    std::string namespace_;
    std::string name;
    std::string msg;
};

class UnknownError: public KeePassError {};

class NewVersionRequiredError: public KeePassError {
public:
    explicit NewVersionRequiredError(std::string_view message)
        : KeePassError("NewVersionRequiredError", message) {};
};

class FileCorruptedError: public KeePassError {
public:
    explicit FileCorruptedError(std::string_view message)
        : KeePassError("FileCorruptedError", message) {};
};

class InvalidKdfParametersError: public KeePassError {
public:
    explicit InvalidKdfParametersError(std::string_view message)
        : KeePassError("InvalidKdfParametersError", message) {};
};

class ArgumentError: public KeePassError {
public:
    explicit ArgumentError(std::string_view message,
                           std::string_view argument)
        : ArgumentError("ArgumentError", message, argument) {};

    std::string what() override {
        // TODO: another way?
        auto message = KeePassError::what();
        auto n = message.find(ArgumentError::argumentPlaceholder);
        if (n != std::string::npos) {
            message.replace(n, std::size(ArgumentError::argumentPlaceholder),
                            argument);
        }
        return message;
    }
protected:
    explicit ArgumentError(std::string_view error_name,
                           std::string_view message,
                           std::string_view argument)
        : KeePassError(error_name, message), argument(argument) {};

    const std::string argument;

    inline static const std::string argumentPlaceholder = "<argument>";
};

class ArgumentIsNullError: public ArgumentError {
public:
    explicit ArgumentIsNullError(std::string_view argument)
        : ArgumentError("ArgumentIsNullError",
                        "argument <argument> is null",
                        argument){};
};

class ArgumentIsOutOfRangeError: public ArgumentError {
public:
    explicit ArgumentIsOutOfRangeError(std::string_view argument)
        : ArgumentError("ArgumentIsOutOfRangeError",
                        "argument <argument> is out of range",
                        argument) {};
};

class NotEnoughBytesError: public KeePassError {
public:
    explicit NotEnoughBytesError(std::size_t actual, std::size_t expected)
        : KeePassError("NotEnoughBytesError",
                       "not enough bytes to read")
        , actual(actual), expected(expected) {};

    std::string what() override {
        std::ostringstream os(KeePassError::what());
        os << " (" << actual << " != " << expected << " )";
        return os.str();
    }
private:
    const std::size_t actual, expected;
};

class KdfEngineError: public KeePassError {
public:
    explicit KdfEngineError(std::string_view message)
        : KeePassError("KdfEngineError", message) {};

    KdfEngineError()
        : KdfEngineError("unknown kdf engine error") {};
};

class KdfEngineAesError : public KdfEngineError {
    
};

}