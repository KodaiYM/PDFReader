#include "pdfparser.ipdffstream.hpp"

using namespace pdfparser;

ipdffstream::ipdffstream(const char* filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}

ipdffstream::ipdffstream(const std::string& filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}

ipdffstream::ipdffstream(const std::filesystem::path::value_type* filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}

ipdffstream::ipdffstream(const std::filesystem::path& filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}
