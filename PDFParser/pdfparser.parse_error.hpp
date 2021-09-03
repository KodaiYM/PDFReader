#pragma once

#include <stdexcept>

namespace pdfparser {
class parse_error: public std::runtime_error {
public:
	enum error_code {
		// Cross Reference Entry keyword "n" or "f" not found
		xref_entry_keyword_invalid,

		// Literal String lack of ")"
		literal_string_lack_of_right_parenthesis,

		// Array lack of "]"
		array_lack_of_right_square_bracket,

		array_invalid_element,
		hexadecimal_string_non_hexadecimal_digit_found,

		// Hexadecimal String lack of ">"
		hexadecimal_string_lack_of_greater_than_sign,

		// Dictionary lack of ">>"
		dictionary_lack_of_double_greater_than_sign,

		// key may not be name object
		dictionary_invalid_key,

		// value may not be pdf object
		dictionary_invalid_value,

		stream_dictionary_absence_of_Length_entry,
		stream_data_is_shorter_than_Length,

		// Object number and Generation number of this indirect object
		// are inconsistent with Cross Reference Table
		indirect_object_is_inconsistent_with_xref_table,
	};

public:
	explicit parse_error(error_code er_code);
	error_code code() const noexcept;

private:
	const error_code m_error_code;
};
} // namespace pdfparser
