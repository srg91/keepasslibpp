#pragma once

#include "tee_stream_buf.hpp"

#include <iostream>

namespace keepasspp {

class TeeStream : public std::istream {
public:
    explicit TeeStream(std::istream& input, std::ostream& output)
            : buf(input.rdbuf(), output.rdbuf()), std::istream(&buf) {};
private:
    TeeStreamBuf buf;
};

}

