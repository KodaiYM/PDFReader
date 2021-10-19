#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class xref_entry_keyword_invalid final: public position_indicatable_error {
public:
	explicit xref_entry_keyword_invalid(std::streampos position)
	    : position_indicatable_error(
	          position,
	          "クロスリファレンスエントリに無効なキーワードがあります。") {}
};
class object_number_overflow_in_xref_table final
    : public position_indicatable_error {
public:
	explicit object_number_overflow_in_xref_table(std::streampos position)
	    : position_indicatable_error(position,
	                                 "このクロスリファレンスセクションで"
	                                 "オブジェクト番号がオーバーフローします。") {
	}
};
} // namespace pdfparser
