#pragma once

#include <ios>
#include <set>
#include <variant>

#if __cplusplus > 201703L
#define CPP20 1
#endif

#if CPP20
#include <compare>
#endif

/************************
  Cross Reference Types
 ************************/
namespace pdfparser { namespace xref_types {
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
}} // namespace pdfparser::xref_types

#undef CPP20
