#include "tee_stream_buf.hpp"

#include <streambuf>

namespace keepasspp {

TeeStreamBuf::int_type TeeStreamBuf::underflow() {
    return input->sgetc();
}

TeeStreamBuf::int_type TeeStreamBuf::uflow() {
    auto c = input->sbumpc();
    // TODO: catch output stream overflow?
    if (c != traits_type::eof()) output->sputc(traits_type::to_char_type(c));
    return c;
}

std::streamsize TeeStreamBuf::xsgetn(char* s, std::streamsize count) {
    auto r_count = input->sgetn(s, count);
    if (r_count > 0) output->sputn(s, r_count);
    return r_count;
}

}
