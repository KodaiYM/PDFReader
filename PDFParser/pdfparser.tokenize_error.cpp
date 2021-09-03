#include "pdfparser.tokenize_error.hpp"

/// <summary>Definition of member functions of class tokenize_error</summary>
namespace pdfparser {
#pragma region special_functions_of_tokenize_error
tokenize_error::tokenize_error(error_code er_code)
    : std::runtime_error("tokenize error"), m_error_code(er_code) {}
#pragma endregion // region special_functions_of_tokenize_error

#pragma region member_functions_of_tokenize_error
auto tokenize_error::code() const noexcept -> error_code {
	return m_error_code;
}
#pragma endregion // region member_functions_of_tokenize_error
} // namespace pdfparser
