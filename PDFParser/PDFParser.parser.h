#pragma once

#include "PDFParser.object_types.h"
#include "PDFParser.xref_types.h"

#include <fstream> // Intellisense
#include <ios>
#include <memory>

/**************
  Parser Class
 **************/
namespace pdfparser {
template <class InputStreamT>
class parser final {
public:
	explicit parser(InputStreamT&& stream);
	parser(InputStreamT&)        = delete;
	parser(const InputStreamT&&) = delete;

private:
	stream_parser<InputStreamT>     m_stream_parser;
	object_types::dictionary_object m_trailer_dictionary;
	object_pool<InputStreamT>       m_object_pool;
};
public
ref class parser_tostring
    sealed { // C++/CLI execute parser and get xref_table by string
public:
	static System::String ^ get(System::String ^ filename);
};
} // namespace pdfparser
