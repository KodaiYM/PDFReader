#pragma once

#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.tokenizer.hpp"
#include "type_traits_extended.hpp"

namespace pdfparser {
class object_stream: public tokenizer {
public:
	/// <summary>
	/// Take one of the objects specified in the VariantType
	/// (which is std::variant&lt;...&gt;)
	/// </summary>
	/// <typeparam name="VariantType">
	/// std::variant&lt;object types&gt;
	/// </typeparam>
	/// <returns>taken object</returns>
	/// <exception
	/// cref="pdfparser::object_not_found_error(specified_object_not_found)">
	/// thrown when none of the objects in VariantType are found
	/// </exception>
	/// <exception cref="onstream_type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::onstream_direct_object_or_ref take_object();

	/// <summary>
	/// take boolean object
	/// </summary>
	/// <returns>boolean object</returns>
	/// <exception
	/// cref="object_not_found_error(boolean_object_not_found)">
	/// </exception>
	object_types::onstream_boolean_object take_boolean_object();

	/// <summary>
	/// take integer object
	/// </summary>
	/// <returns>integer object</returns>
	/// <exception
	/// cref="object_not_found_error(integer_object_not_found)"></exception>
	/// <exception cref="std::out_of_range">
	/// thrown when read number does not fit into long long
	/// </exception>
	object_types::onstream_integer_object take_integer_object();

	/// <summary>
	/// take real object
	/// </summary>
	/// <returns>real object</returns>
	/// <exception
	/// cref="object_not_found_error(real_object_not_found)"></exception>
	/// <exception cref="std::out_of_range">
	/// thrown when read number does not fit double
	/// </exception>
	object_types::onstream_real_object take_real_object();

	/// <summary>
	/// take string object
	/// </summary>
	/// <returns>string object</returns>
	/// <exception
	/// cref="object_not_found_error(string_object_not_found)"></exception>
	/// <exception cref="parse_error(literal_string_lack_of_right_parenthesis)">
	/// thrown when there is no right closing parenthesis for literal string
	/// </exception>
	/// <exception
	/// cref="parse_error(hexadecimal_string_lack_of_greater_than_sign)">
	/// thrown when there is no right closing greater than sign for hexadecimal
	/// string
	/// </exception>
	/// <exception
	/// cref="parse_error(hexadecimal_string_non_hexadecimal_digit_found)">
	/// thrown when character except for hexadecimal digit is found for
	/// hexadecimal string
	/// </exception>
	object_types::onstream_string_object take_string_object();

	/// <summary>
	/// take name object
	/// </summary>
	/// <returns>name object</returns>
	/// <exception
	/// cref="object_not_found_error(name_object_not_found)"></exception>
	object_types::onstream_name_object take_name_object();

	/// <summary>
	/// take array object
	/// </summary>
	/// <returns>array object</returns>
	/// <exception
	/// cref="object_not_found_error(array_object_not_found)"></exception>
	/// <exception cref="parse_error(array_lack_of_right_square_bracket)">
	/// thrown when there is no right closing square bracket
	/// </exception>
	/// <exception cref="parse_error(array_invalid_element)">
	/// thrown when there is an invalid element
	/// </exception>
	/// <exception cref="std::out_of_range">
	/// thrown when integer does not fit into long long
	/// </exception>
	/// <exception cref="onstream_type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::onstream_array_object take_array_object();

	/// <summary>
	/// take dictionary object
	/// </summary>
	/// <returns>dictionary object</returns>
	/// <exception
	/// cref="object_not_found_error(dictionary_object_not_found)"></exception>
	/// <exception
	/// cref="parse_error(dictionary_lack_of_double_greater_than_sign)">
	/// thrown when there is no closing double greater than sign
	/// </exception>
	/// <exception cref="parse_error(dictionary_invalid_key)">
	/// thrown when dictionary key cannnot be taken as name object
	/// </exception>
	/// <exception cref="parse_error(dictionary_invalid_value)">
	/// thrown when dictionary value cannot be taken
	/// </exception>
	/// <exception cref="onstream_type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::onstream_dictionary_object take_dictionary_object();

	/// <summary>
	/// take stream object
	/// </summary>
	/// <returns>stream object</returns>
	/// <exception
	/// cref="object_not_found_error(stream_object_not_found)">
	/// thrown when stream dictionary is not found or keyword "stream" is not
	/// found
	/// </exception>
	/// <exception cref="istream_extended_error(promise_failed)">
	/// thrown when "\r\n" or "\n" after keyword "stream" is not found
	/// or "\r\n", "\n", or "\r" before keyword "endstream" is not found
	/// </exception>
	/// <exception
	/// cref="parse_error(stream_dictionary_absence_of_Length_entry)">
	/// thrown when stream dictionary does not contain Length entry
	/// </exception>
	/// <exception cref="parse_error(stream_data_is_shorter_than_Length)">
	/// thrown when actual stream data is shorter than Length
	/// </exception>
	/// <exception cref="tokenize_error(promise_token_failed)">
	/// thrown when keyword "endstream" is not found
	/// </exception>
	/// <exception cref="onstream_type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::onstream_stream_object take_stream_object();

	/// <summary>
	/// take stream object (optimized version for successive calls to
	/// take_stream_object and take_dictionary_object)
	/// </summary>
	object_types::onstream_stream_object take_stream_object(
	    object_types::onstream_dictionary_object stream_dictionary);

	/// <summary>
	/// take null object
	/// </summary>
	/// <returns>null object</returns>
	/// <exception
	/// cref="object_not_found_error(null_object_not_found)"></exception>
	object_types::onstream_null_object take_null_object();

	/// <summary>
	/// take indirect reference
	/// </summary>
	/// <returns>indirect reference</returns>
	/// <exception
	/// cref="object_not_found_error(indirect_reference_not_found)"></exception>
	/// <exception cref="std::out_of_range">
	/// thrown when object number or generation number does not fit into long long
	/// </exception>
	/// <exception cref="std::overflow_error">
	/// thrown if overflow occurs when casting from long long to object_t or
	/// generation_t
	/// </exception>
	object_types::onstream_indirect_reference take_indirect_reference();

	/// <summary>
	/// take indirect object
	/// </summary>
	/// <param name="">object pool</param>
	/// <param name="object_number">object number to be taken</param>
	/// <param name="generation_number">generation number to be taken</param>
	/// <returns>the direct object to which this indirect object refers</returns>
	/// <exception cref="istream_extended_error(failed_to_seek)">
	/// thrown when failed to seek to object_info.byte_offset
	/// </exception>
	/// <exception
	/// cref="object_not_found_error(integer_object_not_found)"></exception>
	/// <exception cref="std::out_of_range">
	/// thrown when object number or generation number does not fit into long long
	/// </exception>
	/// <exception cref="std::overflow_error">
	/// thrown if overflow occurs when casting from long long to object_t or
	/// generation_t
	/// </exception>
	/// <exception
	/// cref="parse_error(indirect_object_is_inconsistent_with_xref_table)">
	/// thrown when the object number and generation number in object_info do not
	/// match those in the stream
	/// </exception>
	/// <exception cref="tokenize_error(promise_token_failed)">
	/// thrown when failed to take token "obj" or "endobj"
	/// </exception>
	/// <exception cref="onstream_type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::onstream_direct_object
	    take_indirect_object(const xref_types::xref_inuse_entry& object_info);

public:
	/// <summary>
	/// Add a new xref table to the internally held m_xref_table.
	/// </summary>
	/// <param name="referenced_xref_table">new xref table</param>
	void add_xref_table(
	    const xref_types::xref_table& referenced_xref_table) noexcept;

	template <class... ObjectTypesContainingRef>
	object_types::onstream_direct_object dereference(
	    const object_types::variant_object<ObjectTypesContainingRef...>& object);

	inline object_types::onstream_direct_object
	    dereference(const object_types::onstream_indirect_reference& reference);

private:
	using base = tokenizer;

public:
	/// <summary>constructor</summary>
	using base::base;

private:
	object_cache           m_object_cache;
	xref_types::xref_table m_xref_table;

private:
	template <class Variant, std::size_t... Seq>
	Variant take_object_Variant_impl(std::index_sequence<Seq...>);

private:
	template <class Variant, class... ObjectTypesContainingRef,
	          std::size_t... Seq>
	Variant dereference_Variant_impl(
	    std::index_sequence<Seq...>,
	    const std::variant<ObjectTypesContainingRef...>&);

	template <class Variant, std::size_t... Seq>
	Variant dereference_Variant_impl(
	    std::index_sequence<Seq...>,
	    const object_types::onstream_indirect_reference& reference);

	template <class... ObjectTypes>
	std::variant<ObjectTypes...> dereference_Variant_fixed(
	    const object_types::onstream_indirect_reference& reference);
};
} // namespace pdfparser

#include "pdfparser.object_stream.ipp"
