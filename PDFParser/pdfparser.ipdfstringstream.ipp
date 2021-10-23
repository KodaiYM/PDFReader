#pragma once

#include "pdfparser.ipdfstringstream.hpp"

namespace pdfparser {

ipdfstringstream::ipdfstringstream() : ipdfstringstream("") {}

ipdfstringstream::ipdfstringstream(const std::string& str)
    : basic_istringstream(str, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_istringstream::rdbuf()) {}

} // namespace pdfparser
