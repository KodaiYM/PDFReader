#pragma once

#include "PDFParser.xref_types.h"

#include <fstream> // Intellisense
#include <ios>

/**************
  Parser Class
 **************/
namespace pdfparser {
template <class InputStreamT>
class parser final {
public:
	const xref_types::xref_table& get_xref_table() const& noexcept;
	xref_types::xref_table        get_xref_table() &&;

public:
	explicit parser(InputStreamT&& istr);

private:
	struct footer {
	public:
		explicit footer(InputStreamT& istr);

	public:
		std::streamoff xref_byte_offset;   // cross reference table byte offset
		xref_types::xref_table xref_table; // cross reference table
	};

private:
	footer       m_footer;
	InputStreamT m_stream; // input stream
};
} // namespace pdfparser
