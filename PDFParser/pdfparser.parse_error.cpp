#include "pdfparser.parse_error.hpp"

/// <summary>Definition of member functions of class parse_error</summary>
namespace pdfparser {
#pragma region special_functions_of_parse_error
parse_error::parse_error(error_code er_code)
    : runtime_error{"parse error"}, m_error_code(er_code) {}
#pragma endregion // region special_functions_of_parse_error

#pragma region member_functions_of_parse_error
auto parse_error::code() const noexcept -> error_code {
	return m_error_code;
}
#pragma endregion // region member_functions_of_parse_error
} // namespace pdfparser
