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
	m_trailer_dictionary = m_stream_parser.take_footer(m_object_pool);
}
} // namespace pdfparser
