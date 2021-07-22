#include "PDFParser.error_types.h"

/**************
  Error Types
 **************/
namespace pdfparser { inline namespace error_types {
	syntax_error::syntax_error(error_code er_code)
	    : runtime_error{"syntax error"}, m_error_code(er_code) {}
}} // namespace pdfparser::error_types
