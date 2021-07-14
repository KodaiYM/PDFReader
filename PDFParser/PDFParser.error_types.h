#pragma once

#include <stdexcept>

/**************
  Error Types
 **************/
namespace pdfparser { inline namespace error_types {
	class syntax_error: public std::runtime_error {
	public:
		enum error_code {
			// %%EOF not found
			EOF_not_found,

			// end-of-line(EOL) not found
			EOL_not_found,

			// Cross Reference Table byte offset (unsigned integer) not
			// found in line above %%EOF
			xref_byte_offset_not_found,

			keyword_startxref_not_found,
			signed_integer_not_found,
			unsigned_integer_not_found,
			keyword_xref_not_found,
			space_not_found,
			xref_entry_first_10_digits_invalid,
			xref_entry_second_5_digits_invalid,

			// Cross Reference Entry keyword 'n' or 'f' not found
			xref_entry_keyword_invalid,

			// The last two bytes of a Cross Reference Entry shall be space
			// CR, space LF, or CR LF, but they were not.
			xref_entry_last_2_bytes_invalid
		};

	public:
		explicit syntax_error(error_code er_code);
		constexpr error_code code() const noexcept {
			return m_error_code;
		}

	private:
		const error_code m_error_code;
	};
	class overflow_or_underflow_error: public std::runtime_error {
	public:
		overflow_or_underflow_error();
	};
}} // namespace pdfparser::error_types
