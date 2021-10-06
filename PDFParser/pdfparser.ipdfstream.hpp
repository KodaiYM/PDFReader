#pragma once

#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_not_found_error.hpp"
#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.parse_error.hpp"
#include "pdfparser.tokenizer.hpp"
#include "pdfparser.xref_types.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cinttypes>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <string_view>

namespace pdfparser {
class ipdfstream: public object_stream {
public:
	/// <summary>
	/// seek to end and parse stream footer (xref table and trailer) and
	/// get trailer dictionary
	/// </summary>
	/// <returns>trailer dictionary</returns>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::dictionary_object take_footer();

	/// <summary>
	/// take cross reference table
	/// </summary>
	/// <exception cref="std::overflow_error">
	/// thrown when first object number + number of entries - 1 > max of object_t
	/// </exception>
	/// <exception cref="tokenize_error(promise_token_failed)"></exception>
	/// <exception cref="parse_error(xref_entry_keyword_invalid)"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <returns>cross reference table</returns>
	xref_types::xref_table take_xref_table();

	/// <summary>
	/// take cross reference entry
	/// </summary>
	/// <exception cref="parse_error(xref_entry_keyword_invalid)">
	/// thrown when the keyword in the xref entry is neither "n" nor "f"
	/// </exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	xref_types::xref_entry take_xref_entry(xref_types::object_t object_number);

	/// <summary>
	/// get trailer dictionary
	/// </summary>
	/// <param name="">object pool</param>
	/// <returns>trailer dictionary</returns>
	/// <exception cref="tokenize_error(promise_token_failed)">
	/// thrown when keyword "trailer" is not found
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::dictionary_object take_trailer();

private:
	using base = object_stream;

public:
	/// <summary>constructor</summary>
	using base::base;
};
} // namespace pdfparser
