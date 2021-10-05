#pragma once

#include "pdfparser.ipdfstream.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace pdfparser {
class ipdffstream: public std::ifstream, public ipdfstream {
public:
	explicit ipdffstream(const char* filepath);
	explicit ipdffstream(const std::string& filepath);
	explicit ipdffstream(const std::filesystem::path::value_type* filepath);
	explicit ipdffstream(const std::filesystem::path& filepath);
};
} // namespace pdfparser
