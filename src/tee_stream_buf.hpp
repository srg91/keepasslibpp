#pragma once

#include <streambuf>

namespace keepasspp {

// TODO: make generic basic_tee_stream_buf?
class TeeStreamBuf : public std::streambuf {
public:
    explicit TeeStreamBuf(std::streambuf* input, std::streambuf* output)
        : input(input), output(output) {}
private:
    int_type underflow() final;
    int_type uflow() final;
    std::streamsize xsgetn(char_type* s, std::streamsize count) final;

    std::streambuf* input;
    std::streambuf* output;
};

}
