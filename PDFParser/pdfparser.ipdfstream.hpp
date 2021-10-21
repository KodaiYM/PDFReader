#pragma once

#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.xref_types.hpp"

namespace pdfparser {
class ipdfstream: public object_stream {
public:
	/// <summary>
	/// seek to end and parse stream footer (xref table and trailer) and
	/// get trailer dictionary
	/// </summary>
	/// <returns>trailer dictionary</returns>
	object_types::onstream_dictionary_object take_footer();

	/// <summary>
	/// take cross reference table
	/// </summary>
	xref_types::xref_table take_xref_table();

	/// <summary>
	/// take cross reference entry
	/// </summary>
	xref_types::xref_entry take_xref_entry(xref_types::object_t object_number);

	/// <summary>
	/// get trailer dictionary
	/// </summary>
	/// <returns>trailer dictionary</returns>
	object_types::onstream_dictionary_object take_trailer();

private:
	using base = object_stream;

public:
	/// <summary>constructor</summary>
	using base::base;
};
} // namespace pdfparser
