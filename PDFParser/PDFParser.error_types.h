#pragma once

#include <stdexcept>

/**************
  Error Types
 **************/
namespace pdfparser { inline namespace error_types {
	class parse_error: public std::runtime_error {
	public:
		enum error_code {
			failed_to_seek_forward_head_of_line,
			failed_to_seek,

			// %%EOF not found
			keyword_EOF_not_found,
			xref_byte_offset_not_found,
			keyword_startxref_not_found,

			// end-of-line(EOL) not found
			EOL_not_found,

			signed_integer_not_found,
			unsigned_integer_not_found,
			keyword_xref_not_found,
			space_not_found,
			xref_entry_first_10_digits_invalid,
			xref_entry_second_5_digits_invalid,

			// Cross Reference Entry keyword "n" or "f" not found
			xref_entry_keyword_invalid,

			// The last two bytes of a Cross Reference Entry shall be space
			// CR, space LF, or CR LF, but they were not.
			xref_entry_last_2_bytes_invalid,

			keyword_trailer_not_found,

			// Literal String lack of ")"
			literal_string_lack_of_right_parenthesis,

			unknown_character_detected,

			// couldn't get object by take_any_object
			object_not_found,

			// #xx: xx is not octal 2 digits
			name_invalid_hexadecimal_code,

			// Array lack of "]"
			array_lack_of_right_square_bracket,

			hexadecimal_string_non_hexadecimal_digit_found,

			// Hexadecimal String lack of ">"
			hexadecimal_string_lack_of_greater_than_sign,

			dictionary_key_is_not_name_object,

			// Dictionary lack of ">>"
			dictionary_lack_of_double_greater_than_sign,

			// Stream Object needs CRLF or LF after "stream" keyword
			stream_CRLF_or_LF_not_found,

			stream_dictionary_absence_of_Length_entry,
			stream_dictionary_Length_is_not_Integer,
			stream_data_is_shorter_than_Length,
			keyword_endstream_not_found,
			trailer_dictionary_not_found,

			// Object number and Generation number of this indirect object
			// are inconsistent with Cross Reference Table
			indirect_object_is_inconsistent_with_xref_table,

			keyword_obj_not_found,
			indirect_object_refers_indirect_reference,
			keyword_endobj_not_found,
		};

	public:
		explicit parse_error(error_code er_code);
		constexpr error_code code() const noexcept {
			return m_error_code;
		}

	private:
		const error_code m_error_code;
	};
}} // namespace pdfparser::error_types
