#pragma once

#include "pdfparser.ipdffstream.hpp"

namespace pdfparser {

inline ipdffstream::ipdffstream(const char* filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}

inline ipdffstream::ipdffstream(const std::string& filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}

inline ipdffstream::ipdffstream(
    const std::filesystem::path::value_type* filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}

inline ipdffstream::ipdffstream(const std::filesystem::path& filepath)
    : basic_ifstream(filepath, std::ios_base::in | std::ios_base::binary),
      ipdfstream(basic_ifstream::rdbuf()) {}

} // namespace pdfparser
