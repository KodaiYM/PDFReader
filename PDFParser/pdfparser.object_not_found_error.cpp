#include "pdfparser.object_not_found_error.hpp"

/// <summary>
/// Definition of member functions of class object_not_found_error
/// </summary>
namespace pdfparser {
#pragma region special_functions_of_object_not_found_error
object_not_found_error::object_not_found_error(error_code er_code)
    : runtime_error{"object not found error"}, m_error_code(er_code) {}
#pragma endregion // region special_functions_of_object_not_found_error

#pragma region member_functions_of_object_not_found_error
auto object_not_found_error::code() const noexcept -> error_code {
	return m_error_code;
}
#pragma endregion // region member_functions_of_object_not_found_error
} // namespace pdfparser
