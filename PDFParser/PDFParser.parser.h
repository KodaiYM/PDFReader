#pragma once

#include "PDFParser.object_pool.h"
#include "PDFParser.object_types.h"
#include "PDFParser.stream_parser.h"
#include "PDFParser.xref_types.h"

#include <fstream> // for IntelliSense
#include <ios>
#include <memory>

/**************
  Parser Class
 **************/
namespace pdfparser {
template <class InputStreamT>
class parser final {
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

public:
	explicit parser(InputStreamT&& stream);
	parser(InputStreamT&)        = delete;
	parser(const InputStreamT&&) = delete;

private:
	void parse_footer();

private:
	stream_parser<InputStreamT>     m_stream_parser;
	object_types::dictionary_object m_trailer_dictionary;
	object_pool<InputStreamT>       m_object_pool;

	friend ref class parser_tostring;
};
public
ref class parser_tostring
    sealed { // C++/CLI execute parser and get xref_table by string
public:
	static System::String ^ get(System::String ^ filename);
};
} // namespace pdfparser

/* definition of template functions */
namespace pdfparser {
template <class InputStreamT>
parser<InputStreamT>::parser(InputStreamT&& stream)
    : m_stream_parser(std::move(stream)), m_object_pool(m_stream_parser) {
	parse_footer();
}

template <class InputStreamT>
void parser<InputStreamT>::parse_footer() {
	// TODO: Implement!
	// check %%EOF
	m_stream_parser.seek_to_end();
	m_stream_parser.seek_forward_head_of_line();
	{
		auto eof_pos = m_stream_parser.tell();
		m_stream_parser.require(require_type::keyword_EOF);
		m_stream_parser.seek(eof_pos);
	}

	// get cross-reference table byte offset
	m_stream_parser.seek_forward_head_of_line();
	std::streamoff xref_byte_offset;
	{
		auto xref_byte_offset_pos = m_stream_parser.tell();
		xref_byte_offset          = m_stream_parser.take_xref_byte_offset();
		m_stream_parser.seek(xref_byte_offset_pos);
	}

	// check keyword "startxref"
	m_stream_parser.seek_forward_head_of_line();
	m_stream_parser.require(require_type::keyword_startxref);

	// get cross-reference table
	m_stream_parser.seek(xref_byte_offset);
	auto xref_table = m_stream_parser.take_xref_table();

	// get trailer dictionary
	m_object_pool.add_xref_table(xref_table);
	m_trailer_dictionary = m_stream_parser.take_trailer(m_object_pool);
}
} // namespace pdfparser
