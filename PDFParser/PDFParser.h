#pragma once

#include <istream>
#include <set>
#include <stdexcept>
#include <utility>
#include <variant>

#if __cplusplus > 201703L
#define CPP20 1
#endif

#if CPP20
#include <compare>
#endif

namespace pdfparser {

/************************
  Cross Reference Types
 ************************/
namespace xref_types {
	using object_t     = uint32_t; // object number type
	using generation_t = uint16_t; // generation number type

	// represents an object identified by a pair of
	// (object_number, generation_number)
	struct object_traits {
		object_t     object_number;
		generation_t generation_number;

	protected:
		constexpr object_traits(object_t     object_number,
		                        generation_t generation_number) noexcept
		    : object_number(object_number), generation_number(generation_number) {}

	public:
#if CPP20
		auto operator<=>(const object_traits&) const = default;
#else
		constexpr bool operator==(const object_traits& rhs) const noexcept;
		constexpr bool operator!=(const object_traits& rhs) const noexcept;
		constexpr bool operator<(const object_traits& rhs) const noexcept;
		constexpr bool operator>(const object_traits& rhs) const noexcept;
		constexpr bool operator<=(const object_traits& rhs) const noexcept;
		constexpr bool operator>=(const object_traits& rhs) const noexcept;
#endif
	};
	struct xref_inuse_entry final: object_traits {
		std::streamoff byte_offset;

	public:
		constexpr xref_inuse_entry(object_t       object_number,
		                           generation_t   generation_number,
		                           std::streamoff byte_offset) noexcept
		    : object_traits(object_number, generation_number),
		      byte_offset(byte_offset) {}
	};
	struct xref_free_entry final: object_traits {
		object_t next_free_object_number;

	public:
		constexpr xref_free_entry(object_t     object_number,
		                          generation_t generation_number,
		                          object_t     next_free_object_number) noexcept
		    : object_traits(object_number, generation_number),
		      next_free_object_number(next_free_object_number) {}
	};
	using xref_entry = std::variant<xref_inuse_entry, xref_free_entry>;
	using xref_table = std::set<xref_entry>;
} // namespace xref_types

/**************
  Error Types
 **************/
inline namespace error_types {
	class syntax_error: public std::runtime_error {
	public:
		enum class error_code {
			// %%EOF not found
			EOF_not_found,

			// %%EOF was there, but the next byte is not EOF or EOL
			EOF_invalid,

			// end-of-line(EOL) not found
			EOL_not_found,

			// Cross Reference Table byte offset (unsigned integer) not
			// found in line above %%EOF
			xref_byte_offset_not_found,

			keyword_startxref_not_found,
			signed_integer_not_ound,
			unsigned_interger_not_found,
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
		constexpr syntax_error(error_code er_code) noexcept;
		constexpr error_code code() const noexcept;

	private:
		const error_code m_error_code;
	};
	using overflow_or_underflow_error =
	    std::variant<std::overflow_error, std::underflow_error>;
} // namespace error_types

/**************
  Parser Class
 **************/
class parser final {
public:
	parser(std::istream&& istr);

private:
	void take_footer();

private:
	std::istream   m_stream;             // input stream
	std::streamoff m_xref_byte_offset;   // cross reference table byte offset
	xref_types::xref_table m_xref_table; // cross reference table
};
} // namespace pdfparser

#undef CPP20
