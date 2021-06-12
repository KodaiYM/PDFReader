#include "PDFParser.xref_types.h"

#include <tuple>

/************************
  Cross Reference Types
 ************************/
namespace pdfparser { namespace xref_types {
#if !CPP20
	constexpr bool
	    object_traits::operator==(const object_traits& rhs) const noexcept {
		return std::tie(object_number, generation_number) ==
		       std::tie(rhs.object_number, rhs.generation_number);
	}

	constexpr bool
	    object_traits::operator!=(const object_traits& rhs) const noexcept {
		return !(*this == rhs);
	}

	constexpr bool
	    object_traits::operator<(const object_traits& rhs) const noexcept {
		return std::tie(object_number, generation_number) <
		       std::tie(rhs.object_number, rhs.generation_number);
	}

	constexpr bool
	    object_traits::operator>(const object_traits& rhs) const noexcept {
		return rhs < *this;
	}

	constexpr bool
	    object_traits::operator<=(const object_traits& rhs) const noexcept {
		return !(*this > rhs);
	}

	constexpr bool
	    object_traits::operator>=(const object_traits& rhs) const noexcept {
		return !(*this < rhs);
	}
#endif
}} // namespace pdfparser::xref_types
