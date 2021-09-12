#pragma once

#include "pdfparser.object_pool.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.stream_parser.hpp"
#include "pdfparser.xref_types.hpp"

#include <fstream> // for IntelliSense
#include <ios>
#include <memory>

namespace pdfparser {
template <class InputStreamT>
class parser final {
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
	explicit parser(InputStreamT&& stream);

	/// <summary>prohibit to move from const rvalue/summary>
	parser(const InputStreamT&&) = delete;

	/// <summary>prohibit to copy from lvalue</summary>
	parser(InputStreamT&) = delete;

private:
	stream_parser<InputStreamT>     m_stream_parser;
	object_types::dictionary_object m_trailer_dictionary;
	object_pool<InputStreamT>       m_object_pool;

	// to GUI (by C++/CLI)
	friend ref class parser_tostring;
};
public
ref class parser_tostring
    sealed { // C++/CLI execute parser and get xref_table by string
public:
	static System::String ^ get(System::String ^ filename);
};
} // namespace pdfparser

// definition of template functions
namespace pdfparser {
template <class InputStreamT>
parser<InputStreamT>::parser(InputStreamT&& stream)
    : m_stream_parser(std::move(stream)), m_object_pool(m_stream_parser) {
	m_trailer_dictionary = m_stream_parser.take_footer(m_object_pool);
}
} // namespace pdfparser
