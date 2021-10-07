#pragma once

#include "pdfparser.ipdfstream.hpp"

#include <sstream>
#include <string>

namespace pdfparser {
class ipdfstringstream: public std::istringstream, public ipdfstream {
public:
	ipdfstringstream();
	explicit ipdfstringstream(const std::string& str);
};
} // namespace pdfparser
