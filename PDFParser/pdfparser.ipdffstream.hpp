#pragma once

#include "pdfparser.ipdfstream.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace pdfparser {
class ipdffstream: public std::ifstream, public ipdfstream {
public:
	explicit inline ipdffstream(const char* filepath);
	explicit inline ipdffstream(const std::string& filepath);
	explicit inline ipdffstream(
	    const std::filesystem::path::value_type* filepath);
	explicit inline ipdffstream(const std::filesystem::path& filepath);
};
} // namespace pdfparser

#include "pdfparser.ipdffstream.ipp"
