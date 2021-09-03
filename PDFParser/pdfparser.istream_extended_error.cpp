#include "pdfparser.istream_extended_error.hpp"

/// <summary>
/// Definition of member functions of class istream_extended_error
/// </summary>
namespace pdfparser {
#pragma region special_functions_of_istream_extended_error
istream_extended_error::istream_extended_error(error_code er_code)
    : std::runtime_error("istream error"), m_error_code(er_code) {}
#pragma endregion // region special_functions_of_istream_extended_error

#pragma region member_functions_of_istream_extended_error
auto istream_extended_error::code() const noexcept -> error_code {
	return m_error_code;
}
#pragma endregion // region member_functions_of_istream_extended_error
} // namespace pdfparser
