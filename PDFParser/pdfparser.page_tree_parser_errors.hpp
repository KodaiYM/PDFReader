#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class non_page_node_detected final
    : position_indicatable_error<non_page_node_detected> {
public:
	explicit non_page_node_detected(std::streampos position)
	    : position_indicatable_error(position,
	                                 "非ページノードが検出されました。") {}
};
} // namespace pdfparser
