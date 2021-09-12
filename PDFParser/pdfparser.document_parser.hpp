#pragma once

#include "pdfparser.PDFPage.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.stream_parser.hpp"
#include "pdfparser.xref_types.hpp"

#include <fstream> // for IntelliSense
#include <ios>
#include <memory>

namespace pdfparser {
template <class InputStreamT>
class document_parser final {
public:
	System::Collections::Generic::List<PDFPage> ^ GetPages();

public:
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

	/// <summary>move the argument stream and construct</summary>
	/// <param name="stream">input stream inheriting from std::istream</param>
	/// <exception cref="std::ios_base::failure">
	/// thrown when stream.rdbuf() == nullptr
	/// </exception>
	/// <exception>
	/// move constructor of InputStreamT may throw an exception.
	/// </exception>
	explicit document_parser(InputStreamT&& stream);

	/// <summary>prohibit to move from const rvalue/summary>
	document_parser(const InputStreamT&&) = delete;

	/// <summary>prohibit to copy from lvalue</summary>
	document_parser(InputStreamT&) = delete;

private:
	stream_parser<InputStreamT>     m_stream_parser;
	object_types::dictionary_object m_trailer_dictionary;
	object_pool<InputStreamT>       m_object_pool;

	// to GUI (by C++/CLI)
	friend ref class parser_tostring;
};
public
ref class parser_tostring
    sealed { // C++/CLI execute document_parser and get xref_table by string
public:
	static System::String ^ get(System::String ^ filename);
};
} // namespace pdfparser

// definition of template functions
namespace pdfparser {
template <class InputStreamT>
    System::Collections::Generic::List<PDFPage> ^
    document_parser<InputStreamT>::GetPages() {
	throw gcnew  System::NotImplementedException();
	return gcnew System::Collections::Generic::List<PDFPage>();
}

template <class InputStreamT>
document_parser<InputStreamT>::document_parser(InputStreamT&& stream)
    : m_stream_parser(std::move(stream)), m_object_pool(m_stream_parser) {
	m_trailer_dictionary = m_stream_parser.take_footer(m_object_pool);
}
} // namespace pdfparser
