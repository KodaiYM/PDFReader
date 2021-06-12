#include "PDFParser.error_types.h"

/**************
  Error Types
 **************/
namespace pdfparser { namespace error_types {
	syntax_error::syntax_error(error_code er_code)
	    : runtime_error{"syntax error"}, m_error_code(er_code) {}
	constexpr auto syntax_error::code() const noexcept -> error_code {
		return m_error_code;
	}
}} // namespace pdfparser::error_types
