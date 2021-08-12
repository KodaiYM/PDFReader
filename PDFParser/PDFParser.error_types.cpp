#include "PDFParser.error_types.h"

/**************
  Error Types
 **************/
namespace pdfparser { inline namespace error_types {
	parse_error::parse_error(error_code er_code)
	    : runtime_error{"parse error"}, m_error_code(er_code) {}
}} // namespace pdfparser::error_types
