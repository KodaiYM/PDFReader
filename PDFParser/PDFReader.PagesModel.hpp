#pragma once

#include "PDFReader.PDFPage.hpp"

namespace PDFReader {
public
ref class PagesModel sealed {
public:
	static System::Collections::Generic::List<PDFReader::PDFPage ^> ^
	    GetPages(System::String ^ filename);
};
} // namespace PDFReader
