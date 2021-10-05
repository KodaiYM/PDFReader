#pragma once

#include "PDFReader.PDFPage.hpp"
#include "pdfparser.document_error.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.space.hpp"
#include "pdfparser.xref_types.hpp"

#include <fstream> // for IntelliSense
#include <ios>
#include <memory>

namespace pdfparser {
template <class InputStreamT>
class document_reader final {
public:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^ get_pages();

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
	explicit document_reader(InputStreamT&& stream);

	/// <summary>prohibit to move from const rvalue/summary>
	document_reader(const InputStreamT&&) = delete;

	/// <summary>prohibit to copy from lvalue</summary>
	document_reader(InputStreamT&) = delete;

private:
	ipdfstream<InputStreamT>        m_object_parser;
	object_types::dictionary_object m_trailer_dictionary;
	object_pool<InputStreamT>       m_object_pool;
};
} // namespace pdfparser

#include "pdfparser.document_reader.ipp"
