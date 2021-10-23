#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.ipdfstream_errors.hpp"

using namespace pdfparser;

object_types::onstream_dictionary_object ipdfstream::take_footer() {
	seek_to_end();
	seek_forward_head_of_line();
	seek_forward_head_of_line();
	seek_forward_head_of_line();

	// check keyword "startxref"
	promise_token({"startxref"});

	// get cross-reference table byte offset
	std::streamoff xref_byte_offset = take_integer_object();

	// check %%EOF
	promise({"\r\n", "\n", "\r"});
	promise({"%%EOF"});

	// get cross-reference table
	seek(xref_byte_offset);
	auto xref_table = take_xref_table();

	// get trailer dictionary
	add_xref_table(xref_table);
	return take_trailer();
}

xref_types::xref_table ipdfstream::take_xref_table() {
	using namespace xref_types;

	xref_table this_xref_table;

	promise_token({"xref"});

	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto     first_object_number_pos = tell();
	const object_t first_object_number     = take_integer_object();
	const object_t number_of_entries       = take_integer_object();

	assert(number_of_entries > 0); // HACK: error check? throw?
	// this "if statement" means...
	// first_object_number + number_of_entries - 1 >
	// std::numeric_limits<object_t>::max()
	if (number_of_entries - 1 >
	    std::numeric_limits<object_t>::max() - first_object_number) {
		throw object_number_overflow_in_xref_table(first_object_number_pos);
	}

	for (object_t entry_offset = 0; entry_offset < number_of_entries;
	     ++entry_offset) {
		const auto object_number = first_object_number + entry_offset;
		this_xref_table.insert(take_xref_entry(object_number));
	}
	return this_xref_table;
}

xref_types::xref_entry
    ipdfstream::take_xref_entry(xref_types::object_t object_number) {
	using namespace std::string_view_literals;

	auto first_integer  = take_integer_object();
	auto second_integer = take_integer_object();

	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto keyword_token_pos = tell();

	auto keyword_opt = take_token();
	if ("n"sv == keyword_opt) {
		return xref_types::xref_inuse_entry{object_number, second_integer,
		                                    first_integer};
	} else if ("f"sv == keyword_opt) {
		return xref_types::xref_free_entry{object_number, second_integer,
		                                   first_integer};
	} else {
		throw xref_entry_keyword_invalid(keyword_token_pos);
	}
}

object_types::onstream_dictionary_object ipdfstream::take_trailer() {
	promise_token({"trailer"});
	return take_dictionary_object();
}
