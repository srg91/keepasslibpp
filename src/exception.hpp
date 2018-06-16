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

    explicit KeePassError(std::string&& message)
        : namespace_("keepasspp")
        , name("KeePassError")
        , msg(std::move(message)) {};

    explicit KeePassError(std::string&& error_name, std::string&& message)
        : namespace_("keepasspp")
        , name(std::move(error_name))
        , msg(std::move(message)) {};

    explicit KeePassError(std::string&& namespace_,
                          std::string&& error_name,
                          std::string&& message)
        : namespace_(std::move(namespace_))
        , name(std::move(error_name))
        , msg(std::move(message)) {};

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
    explicit NewVersionRequiredError(std::string&& message)
        : KeePassError("NewVersionRequiredError",
                       std::forward<std::string>(message)) {};
};

class FileCorruptedError: public KeePassError {
public:
    explicit FileCorruptedError(std::string&& message)
        : KeePassError("FileCorruptedError",
                       std::forward<std::string>(message)) {};
};

class InvalidKdfParametersError: public KeePassError {
public:
    explicit InvalidKdfParametersError(std::string&& message)
        : KeePassError("InvalidKdfParametersError",
                       std::forward<std::string>(message)) {};
};

class ArgumentError: public KeePassError {
public:
    explicit ArgumentError(std::string&& message,
                           std::string&& argument)
        : ArgumentError("ArgumentError",
                        std::forward<std::string>(message),
                        std::forward<std::string>(argument)) {};

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
    // TODO: remove code-duplication?
    explicit ArgumentError(std::string&& error_name,
                           std::string&& message,
                           std::string_view argument)
        : KeePassError(std::forward<std::string>(error_name),
                       std::forward<std::string>(message))
        , argument(argument) {};

    explicit ArgumentError(std::string&& error_name,
                           std::string&& message,
                           std::string&& argument)
        : KeePassError(std::forward<std::string>(error_name),
                       std::forward<std::string>(message))
        , argument(std::move(argument)) {};

    const std::string argument;

    inline static const std::string argumentPlaceholder = "<argument>";
};

class ArgumentIsNullError: public ArgumentError {
public:
    explicit ArgumentIsNullError(const std::string& argument)
        : ArgumentError("ArgumentIsNullError",
                        "argument <argument> is null",
                        argument) {}
    explicit ArgumentIsNullError(std::string&& argument)
        : ArgumentError("ArgumentIsNullError",
                        "argument <argument> is null",
                        std::forward<std::string>(argument)){};
};

class ArgumentIsOutOfRangeError: public ArgumentError {
public:
    explicit ArgumentIsOutOfRangeError(std::string&& argument)
        : ArgumentError("ArgumentIsOutOfRangeError",
                        "argument <argument> is out of range",
                        std::forward<std::string>(argument)) {};
};

class NotEnoughBytesError: public KeePassError {
public:
    NotEnoughBytesError(std::size_t actual,
                        std::size_t expected)
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
    KdfEngineError() : KeePassError("KdfEngineError",
                                    "unknown kdf engine error") {};
};

}