#pragma once

#include "PDFParser.PDFPage.hpp"

namespace PDFParser {
public
ref class PagesModel sealed {
public:
	static System::Collections::Generic::List<PDFParser::PDFPage ^> ^
	    GetPages(System::String ^ filename);
};
} // namespace PDFParser
