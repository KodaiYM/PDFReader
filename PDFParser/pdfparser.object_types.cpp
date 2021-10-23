#include "pdfparser.object_types.hpp"

using namespace pdfparser::object_types;

namespace pdfparser::object_types {
#pragma region array_object
bool operator==(const array_object& lhs, const array_object& rhs) noexcept {
	return static_cast<const array_object::base::base&>(lhs) ==
	       static_cast<const array_object::base::base&>(rhs);
}
#pragma endregion // region array_object

#pragma region dictionary_object
bool           operator==(const dictionary_object& lhs,
                const dictionary_object& rhs) noexcept {
  return static_cast<const dictionary_object::base::base&>(lhs) ==
         static_cast<const dictionary_object::base::base&>(rhs);
}
#pragma endregion // region dictionary_object
} // namespace pdfparser::object_types
