#pragma once

#include "pdfparser.ipdfstream.hpp"

#include <sstream>
#include <string>

namespace pdfparser {
class ipdfstringstream: public std::istringstream, public ipdfstream {
public:
	inline ipdfstringstream();
	explicit inline ipdfstringstream(const std::string& str);
};
} // namespace pdfparser

#include "pdfparser.ipdfstringstream.ipp"
