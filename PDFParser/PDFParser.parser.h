#pragma once

#include "PDFParser.xref_types.h"

#include <fstream> // Intellisense
#include <ios>
#include <memory>

/**************
  Parser Class
 **************/
namespace pdfparser {
class parser final {
public:
	const xref_types::xref_table& get_xref_table() &;
	xref_types::xref_table        get_xref_table() &&;

public:
	template <class FilenameT>
	explicit parser(const FilenameT& filename);

private:
	struct footer {
	public:
		explicit footer(std::istream& istr);

	public:
		std::streamoff xref_byte_offset;   // cross reference table byte offset
		xref_types::xref_table xref_table; // cross reference table
	};

private:
	std::ifstream           m_stream;
	std::unique_ptr<footer> m_footer;
};
public
ref class parser_tostring sealed { // C++/CLI execute parser and get xref_table by string
public:
	static System::String ^ get(System::String^ filename);
};
} // namespace pdfparser
