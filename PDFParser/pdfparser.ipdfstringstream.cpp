#include "pdfparser.ipdfstringstream.hpp"

using namespace pdfparser;

pdfparser::ipdfstringstream::ipdfstringstream() : ipdfstringstream("") {}

pdfparser::ipdfstringstream::ipdfstringstream(const std::string& str)
    : basic_istringstream(str, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_istringstream::rdbuf()) {}
