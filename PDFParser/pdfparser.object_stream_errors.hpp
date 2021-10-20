#pragma once

#include "pdfparser.document_error.hpp"

#include <string>

namespace pdfparser {
class boolean_object_not_found final
    : public position_indicatable_error<boolean_object_not_found> {
public:
	explicit boolean_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Boolean オブジェクトが必要です。") {}
};
class integer_object_not_found final
    : public position_indicatable_error<integer_object_not_found> {
public:
	explicit integer_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Integer オブジェクトが必要です。") {}
};
class real_object_not_found final
    : public position_indicatable_error<real_object_not_found> {
public:
	explicit real_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Real オブジェクトが必要です。") {}
};
class string_object_not_found final
    : public position_indicatable_error<string_object_not_found> {
public:
	explicit string_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "String オブジェクトが必要です。") {}
};
class name_object_not_found final
    : public position_indicatable_error<name_object_not_found> {
public:
	explicit name_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Name オブジェクトが必要です。") {}
};
class array_object_not_found final
    : public position_indicatable_error<array_object_not_found> {
public:
	explicit array_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Array オブジェクトが必要です。") {
	}
};
class dictionary_object_not_found final
    : public position_indicatable_error<dictionary_object_not_found> {
public:
	explicit dictionary_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Dictionary オブジェクトが必要です。") {}
};
class stream_object_not_found final
    : public position_indicatable_error<stream_object_not_found> {
public:
	explicit stream_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Stream オブジェクトが必要です。") {}
};
class null_object_not_found final
    : public position_indicatable_error<null_object_not_found> {
public:
	explicit null_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Null オブジェクトが必要です。") {}
};
class indirect_reference_not_found final
    : public position_indicatable_error<indirect_reference_not_found> {
public:
	explicit indirect_reference_not_found(std::streampos position)
	    : position_indicatable_error(position, "間接参照が必要です。") {}
};
// TODO: 実装保留中
class specified_object_not_found final
    : public position_indicatable_error<specified_object_not_found> {
public:
	explicit specified_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "message") {}
};

class real_object_overflows final
    : public position_indicatable_error<real_object_overflows> {
public:
	explicit real_object_overflows(std::streampos position)
	    : position_indicatable_error(
	          position, "実数オブジェクトの内部表現がオーバーフローしました。") {}
};

class literal_string_lack_of_right_parenthesis final
    : public position_indicatable_error<
          literal_string_lack_of_right_parenthesis> {
public:
	explicit literal_string_lack_of_right_parenthesis(std::streampos position)
	    : position_indicatable_error(position,
	                                 "文字列リテラルの終端の ) がありません。") {}
};
class array_lack_of_right_square_bracket final
    : public position_indicatable_error<array_lack_of_right_square_bracket> {
public:
	explicit array_lack_of_right_square_bracket(std::streampos position)
	    : position_indicatable_error(position, "配列の終端の ] がありません。") {}
};
class hexadecimal_string_non_hexadecimal_digit_found final
    : public position_indicatable_error<
          hexadecimal_string_non_hexadecimal_digit_found> {
public:
	explicit hexadecimal_string_non_hexadecimal_digit_found(
	    std::streampos position)
	    : position_indicatable_error(
	          position, "16進文字列リテラルに非16進数字があります。") {}
};
class hexadecimal_string_lack_of_greater_than_sign final
    : public position_indicatable_error<
          hexadecimal_string_lack_of_greater_than_sign> {
public:
	explicit hexadecimal_string_lack_of_greater_than_sign(std::streampos position)
	    : position_indicatable_error(
	          position, "16進文字列リテラルの終端の > がありません。") {}
};
class dictionary_lack_of_double_greater_than_sign final
    : public position_indicatable_error<
          dictionary_lack_of_double_greater_than_sign> {
public:
	explicit dictionary_lack_of_double_greater_than_sign(std::streampos position)
	    : position_indicatable_error(position, "辞書の終端の >> がありません。") {
	}
};
class stream_dictionary_absence_of_Length_entry final
    : public position_indicatable_error<
          stream_dictionary_absence_of_Length_entry> {
public:
	explicit stream_dictionary_absence_of_Length_entry(std::streampos position)
	    : position_indicatable_error(
	          position,
	          "ストリームオブジェクトの辞書に Length エントリがありません。") {}
};
class stream_data_is_shorter_than_Length final
    : public position_indicatable_error<stream_data_is_shorter_than_Length> {
public:
	stream_data_is_shorter_than_Length(std::streampos  position,
	                                   std::streamsize length)
	    : position_indicatable_error(
	          position, "ストリームオブジェクトのデータが Length エントリの値(" +
	                        std::to_string(length) + ")に比べて短すぎます。") {}
};
class indirect_object_is_inconsistent_with_xref_table final
    : public position_indicatable_error<
          indirect_object_is_inconsistent_with_xref_table> {
public:
	explicit indirect_object_is_inconsistent_with_xref_table(
	    std::streampos position, xref_types::object_t object_number,
	    xref_types::generation_t            generation_number,
	    const xref_types::xref_inuse_entry& object_info)
	    : position_indicatable_error(
	          position, "この間接オブジェクトのオブジェクト番号(" +
	                        std::to_string(object_number) + ")と世代番号(" +
	                        std::to_string(generation_number) +
	                        ")は、"
	                        "クロスリファレンス情報(" +
	                        std::to_string(object_info.object_number) + ", " +
	                        std::to_string(object_info.generation_number) +
	                        +") と矛盾しています。") {}
};
} // namespace pdfparser
