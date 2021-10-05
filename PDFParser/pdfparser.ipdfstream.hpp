#pragma once

#include "pdfparser.object_not_found_error.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.parse_error.hpp"
#include "pdfparser.tokenizer.hpp"
#include "pdfparser.xref_types.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cinttypes>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <string_view>

namespace pdfparser {
// HACK: want to work out the two-way dependency with object_pool.
template <class InputStreamT>
class object_pool;

template <class InputStreamT>
class ipdfstream {
public:
	/// <summary>
	/// seek to end and parse stream footer (xref table and trailer) and
	/// get trailer dictionary
	/// </summary>
	/// <param name="object_accessor">reference to object_pool</param>
	/// <returns>trailer dictionary</returns>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::dictionary_object
	    take_footer(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// take cross reference table
	/// </summary>
	/// <exception cref="std::overflow_error">
	/// thrown when first object number + number of entries - 1 > max of object_t
	/// </exception>
	/// <exception cref="tokenize_error(promise_token_failed)"></exception>
	/// <exception cref="parse_error(xref_entry_keyword_invalid)"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <returns>cross reference table</returns>
	xref_types::xref_table take_xref_table();

	/// <summary>
	/// take cross reference entry
	/// </summary>
	/// <exception cref="parse_error(xref_entry_keyword_invalid)">
	/// thrown when the keyword in the xref entry is neither "n" nor "f"
	/// </exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	xref_types::xref_entry take_xref_entry(xref_types::object_t object_number);

	/// <summary>
	/// get trailer dictionary
	/// </summary>
	/// <param name="object_accessor">object pool</param>
	/// <returns>trailer dictionary</returns>
	/// <exception cref="tokenize_error(promise_token_failed)">
	/// thrown when keyword "trailer" is not found
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::dictionary_object
	    take_trailer(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// Take one of the objects in the VariantType
	/// (which is std::variant&lt;...&gt;)
	/// (except for array_object, dictionary_object and stream_object.
	/// These objects can be taken with take_object(object_pool&amp;).)
	/// </summary>
	/// <typeparam name="VariantType">
	/// std::variant&lt;object types&gt;
	/// except for array_object, dictionary_object and stream_object
	/// </typeparam>
	/// <returns>taken object</returns>
	/// <exception
	/// cref="pdfparser::object_not_found_error(specified_object_not_found)">
	/// thrown when none of the objects in VariantType are found
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <class Variant>
	Variant take_object();

	/// <summary>
	/// Take one of the objects specified in the VariantType
	/// (which is std::variant&lt;...&gt;)
	/// (which must contain one of array_object, dictionary_object and
	/// stream_object.)
	/// </summary>
	/// <typeparam name="VariantType">
	/// std::variant&lt;object types&gt;
	/// (which must contain one of array_object, dictionary_object and
	/// stream_object)
	/// </typeparam>
	/// <returns>taken object</returns>
	/// <exception
	/// cref="pdfparser::object_not_found_error(specified_object_not_found)">
	/// thrown when none of the objects in VariantType are found
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <class Variant>
	Variant take_object(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// Take one of the objects specified in the template argument
	/// (except for array_object, dictionary_object and stream_object.
	/// These objects can be taken with take_object(object_pool&amp;).)
	/// </summary>
	/// <typeparam name="...ObjectTypes">
	/// object types except for array_object, dictionary_object and stream_object
	/// </typeparam>
	/// <returns>taken object</returns>
	/// <exception
	/// cref="pdfparser::object_not_found_error(specified_object_not_found)">
	/// thrown when none of the objects in ObjectTypes are found
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <class... ObjectTypes, std::enable_if_t<sizeof...(ObjectTypes) >= 2,
	                                                 std::nullptr_t> = nullptr>
	std::variant<ObjectTypes...> take_object();

	/// <summary>
	/// Take one of the objects specified in the template argument
	/// (which must contain one of array_object, dictionary_object and
	/// stream_object.)
	/// </summary>
	/// <typeparam name="...ObjectTypes">
	/// object types (which must contain one of array_object, dictionary_object
	/// and stream_object)
	/// </typeparam>
	/// <returns>taken object</returns>
	/// <exception
	/// cref="pdfparser::object_not_found_error(specified_object_not_found)">
	/// thrown when none of the objects in ObjectTypes are found
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <class... ObjectTypes, std::enable_if_t<sizeof...(ObjectTypes) >= 2,
	                                                 std::nullptr_t> = nullptr>
	std::variant<ObjectTypes...>
	    take_object(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// take boolean object
	/// </summary>
	/// <returns>boolean object</returns>
	/// <exception
	/// cref="object_not_found_error(boolean_object_not_found)">
	/// </exception>
	object_types::boolean_object take_boolean_object();

	/// <summary>
	/// take integer object
	/// </summary>
	/// <returns>integer object</returns>
	/// <exception
	/// cref="object_not_found_error(integer_object_not_found)"></exception>
	/// <exception cref="std::out_of_range">
	/// thrown when read number does not fit into long long
	/// </exception>
	object_types::integer_object take_integer_object();

	/// <summary>
	/// take real object
	/// </summary>
	/// <returns>real object</returns>
	/// <exception
	/// cref="object_not_found_error(real_object_not_found)"></exception>
	/// <exception cref="std::out_of_range">
	/// thrown when read number does not fit double
	/// </exception>
	object_types::real_object take_real_object();

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
	object_types::string_object take_string_object();

	/// <summary>
	/// take name object
	/// </summary>
	/// <returns>name object</returns>
	/// <exception
	/// cref="object_not_found_error(name_object_not_found)"></exception>
	object_types::name_object take_name_object();

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
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::array_object
	    take_array_object(object_pool<InputStreamT>& object_accessor);

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
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::dictionary_object
	    take_dictionary_object(object_pool<InputStreamT>& object_accessor);

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
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::stream_object
	    take_stream_object(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// take stream object (optimized version for successive calls to
	/// take_stream_object and take_dictionary_object)
	/// </summary>
	template <class DictionaryObject>
	object_types::stream_object
	    take_stream_object(object_pool<InputStreamT>& object_accessor,
	                       DictionaryObject&&         stream_dictionary);

	/// <summary>
	/// take null object
	/// </summary>
	/// <returns>null object</returns>
	/// <exception
	/// cref="object_not_found_error(null_object_not_found)"></exception>
	object_types::null_object take_null_object();

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
	object_types::indirect_reference take_indirect_reference();

	/// <summary>
	/// take indirect object
	/// </summary>
	/// <param name="object_accessor">object pool</param>
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
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	object_types::any_direct_object
	    take_indirect_object(object_pool<InputStreamT>&          object_accessor,
	                         const xref_types::xref_inuse_entry& object_info);

public:
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

	/// <summary>
	/// move the argument stream to tokenizer and construct
	/// </summary>
	/// <param name="stream">input stream inheriting from std::istream</param>
	/// <exception>
	/// constructor of tokenizer(InputStreamT&amp;&amp;) may throw an
	/// exception.
	/// </exception>
	ipdfstream(InputStreamT&& stream);

	/// <summary>
	/// prohibit to move from const rvalue
	/// </summary>
	ipdfstream(const InputStreamT&&) = delete;

	/// <summary>
	/// prohibit to copy from lvalue
	/// </summary>
	ipdfstream(InputStreamT&) = delete;

private:
	tokenizer<InputStreamT> m_tknizer;

private:
	template <class Variant, std::size_t... Seq>
	Variant take_object_Variant_impl(std::index_sequence<Seq...>);

	template <class Variant, std::size_t... Seq>
	Variant take_object_Variant_impl(std::index_sequence<Seq...>,
	                                 object_pool<InputStreamT>& object_accessor);
};
} // namespace pdfparser

// definition of member functions of ipdfstream
namespace pdfparser {
#pragma region stream_parser_definitions
template <class InputStreamT>
ipdfstream<InputStreamT>::ipdfstream(InputStreamT&& stream)
    : m_tknizer(std::move(stream)) {}

template <class InputStreamT>
object_types::dictionary_object ipdfstream<InputStreamT>::take_footer(
    object_pool<InputStreamT>& object_accessor) {
	m_tknizer.seek_to_end();
	m_tknizer.seek_forward_head_of_line();
	m_tknizer.seek_forward_head_of_line();
	m_tknizer.seek_forward_head_of_line();

	// check keyword "startxref"
	m_tknizer.promise_token({"startxref"});

	// get cross-reference table byte offset
	std::streamoff xref_byte_offset = take_integer_object();

	// check %%EOF
	m_tknizer.promise({"\r\n", "\n", "\r"});
	m_tknizer.promise({"%%EOF"});

	// get cross-reference table
	m_tknizer.seek(xref_byte_offset);
	auto xref_table = take_xref_table();

	// get trailer dictionary
	object_accessor.add_xref_table(xref_table);
	return take_trailer(object_accessor);
}

template <class InputStreamT>
xref_types::xref_table ipdfstream<InputStreamT>::take_xref_table() {
	using namespace xref_types;

	xref_table this_xref_table;

	m_tknizer.promise_token({"xref"});

	const object_t first_object_number = take_integer_object();
	const object_t number_of_entries   = take_integer_object();

	assert(number_of_entries > 0); // HACK: error check? throw?
	// this "if statement" means...
	// first_object_number + number_of_entries - 1 >
	// std::numeric_limits<object_t>::max()
	if (number_of_entries - 1 >
	    std::numeric_limits<object_t>::max() - first_object_number) {
		throw std::overflow_error("overflow");
	}

	for (object_t entry_offset = 0; entry_offset < number_of_entries;
	     ++entry_offset) {
		const auto object_number = first_object_number + entry_offset;
		this_xref_table.insert(take_xref_entry(object_number));
	}
	return this_xref_table;
}

template <class InputStreamT>
xref_types::xref_entry ipdfstream<InputStreamT>::take_xref_entry(
    xref_types::object_t object_number) {
	using namespace std::string_view_literals;

	auto first_integer  = take_integer_object();
	auto second_integer = take_integer_object();

	auto keyword_opt = m_tknizer.take_token();
	if (keyword_opt.has_value() && "n"sv == keyword_opt.value()) {
		return xref_types::xref_inuse_entry{object_number, second_integer,
		                                    first_integer};
	} else if (keyword_opt.has_value() && "f"sv == keyword_opt.value()) {
		return xref_types::xref_free_entry{object_number, second_integer,
		                                   first_integer};
	} else {
		throw parse_error(parse_error::xref_entry_keyword_invalid);
	}
}

template <class InputStreamT>
object_types::dictionary_object ipdfstream<InputStreamT>::take_trailer(
    object_pool<InputStreamT>& object_accessor) {
	m_tknizer.promise_token({"trailer"});
	return take_dictionary_object(object_accessor);
}

template <class InputStreamT>
template <class Variant, std::size_t... Seq>
Variant ipdfstream<InputStreamT>::take_object_Variant_impl(
    std::index_sequence<Seq...>) {
	return take_object<std::variant_alternative_t<Seq, Variant>...>();
}

template <class InputStreamT>
template <class Variant, std::size_t... Seq>
Variant ipdfstream<InputStreamT>::take_object_Variant_impl(
    std::index_sequence<Seq...>, object_pool<InputStreamT>& object_accessor) {
	return take_object<std::variant_alternative_t<Seq, Variant>...>(
	    object_accessor);
}

template <class InputStreamT>
template <class Variant>
Variant ipdfstream<InputStreamT>::take_object() {
	return take_object_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>());
}

template <class InputStreamT>
template <class Variant>
Variant ipdfstream<InputStreamT>::take_object(
    object_pool<InputStreamT>& object_accessor) {
	return take_object_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>(),
	    object_accessor);
}

template <class InputStreamT>
template <class... ObjectTypes,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
std::variant<ObjectTypes...> ipdfstream<InputStreamT>::take_object() {
	using namespace object_types;

	constexpr bool contains_boolean =
	    (... || std::is_same_v<boolean_object, ObjectTypes>);
	constexpr bool contains_real =
	    (... || std::is_same_v<real_object, ObjectTypes>);
	constexpr bool contains_string =
	    (... || std::is_same_v<string_object, ObjectTypes>);
	constexpr bool contains_name =
	    (... || std::is_same_v<name_object, ObjectTypes>);
	constexpr bool contains_null =
	    (... || std::is_same_v<null_object, ObjectTypes>);
	constexpr bool contains_indirect_reference =
	    (... || std::is_same_v<indirect_reference, ObjectTypes>);
	constexpr bool contains_integer =
	    (... || std::is_same_v<integer_object, ObjectTypes>);

	static_assert(sizeof...(ObjectTypes) ==
	              contains_boolean + contains_real + contains_string +
	                  contains_name + contains_null +
	                  contains_indirect_reference + contains_integer);

	if constexpr (contains_boolean) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_boolean_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::boolean_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_real) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_real_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::real_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_string) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_string_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::string_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_name) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_name_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::name_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_null) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_null_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::null_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_indirect_reference) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_indirect_reference();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::indirect_reference_not_found ==
			    obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_integer) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_integer_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::integer_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}

	throw object_not_found_error(
	    object_not_found_error::specified_object_not_found);
}

template <class InputStreamT>
template <class... ObjectTypes,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
std::variant<ObjectTypes...> ipdfstream<InputStreamT>::take_object(
    object_pool<InputStreamT>& object_accessor) {
	using namespace object_types;

	constexpr bool contains_boolean =
	    (... || std::is_same_v<boolean_object, ObjectTypes>);
	constexpr bool contains_real =
	    (... || std::is_same_v<real_object, ObjectTypes>);
	constexpr bool contains_string =
	    (... || std::is_same_v<string_object, ObjectTypes>);
	constexpr bool contains_name =
	    (... || std::is_same_v<name_object, ObjectTypes>);
	constexpr bool contains_array =
	    (... || std::is_same_v<array_object, ObjectTypes>);
	constexpr bool contains_null =
	    (... || std::is_same_v<null_object, ObjectTypes>);
	constexpr bool contains_stream =
	    (... || std::is_same_v<stream_object, ObjectTypes>);
	constexpr bool contains_indirect_reference =
	    (... || std::is_same_v<indirect_reference, ObjectTypes>);
	constexpr bool contains_integer =
	    (... || std::is_same_v<integer_object, ObjectTypes>);
	constexpr bool contains_dictionary =
	    (... || std::is_same_v<dictionary_object, ObjectTypes>);

	static_assert(contains_array || contains_dictionary || contains_stream);
	static_assert(sizeof...(ObjectTypes) ==
	              contains_boolean + contains_real + contains_string +
	                  contains_name + contains_array + contains_null +
	                  contains_stream + contains_indirect_reference +
	                  contains_integer + contains_dictionary);

	if constexpr (contains_boolean) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_boolean_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::boolean_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_real) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_real_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::real_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_string) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_string_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::string_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_name) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_name_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::name_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_array) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_array_object(object_accessor);
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::array_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_null) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_null_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::null_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_stream || contains_dictionary) {
		auto before_take_dictionary_pos = m_tknizer.tell();
		std::optional<dictionary_object> dictionary;
		try {
			dictionary = take_dictionary_object(object_accessor);
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::dictionary_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_dictionary_pos);
			} else {
				throw;
			}
		}

		if (dictionary.has_value()) {
			if constexpr (contains_stream) {
				auto before_take_stream_pos = m_tknizer.tell();
				try {
					if constexpr (contains_dictionary) {
						return take_stream_object(object_accessor, dictionary.value());
					} else {
						return take_stream_object(object_accessor,
						                          std::move(dictionary).value());
					}
				} catch (const object_not_found_error& obj_e) {
					if (object_not_found_error::stream_object_not_found == obj_e.code()) {
						if constexpr (contains_dictionary) {
							m_tknizer.seek(before_take_stream_pos);
							return std::move(dictionary).value();
						} else {
							m_tknizer.seek(before_take_dictionary_pos);
						}
					} else {
						throw;
					}
				}
			} else {
				return std::move(dictionary).value();
			}
		}
	}
	if constexpr (contains_indirect_reference) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_indirect_reference();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::indirect_reference_not_found ==
			    obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_integer) {
		auto before_take_object_pos = m_tknizer.tell();
		try {
			return take_integer_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::integer_object_not_found == obj_e.code()) {
				m_tknizer.seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}

	throw object_not_found_error(
	    object_not_found_error::specified_object_not_found);
}

template <class InputStreamT>
object_types::boolean_object ipdfstream<InputStreamT>::take_boolean_object() {
	using namespace object_types;

	if (m_tknizer.attempt_token("true")) {
		return boolean_object(true);
	} else if (m_tknizer.attempt_token("false")) {
		return boolean_object(false);
	}

	throw object_not_found_error(
	    object_not_found_error::boolean_object_not_found);
}
template <class InputStreamT>
object_types::integer_object ipdfstream<InputStreamT>::take_integer_object() {
	auto front_token = m_tknizer.take_token();

	if (front_token.has_value()) {
		std::regex       integer_re("[+-]?[0-9]+");
		std::string_view front_view = front_token.value();

		if (std::regex_match(front_view.begin(), front_view.end(), integer_re)) {
			// may throw std::out_of_range
			static_assert(
			    std::is_same_v<long long, object_types::integer_object::int_type>);
			return std::stoll(std::string(front_view), nullptr, 10);
		}
	}

	throw object_not_found_error(
	    object_not_found_error::integer_object_not_found);
}
template <class InputStreamT>
object_types::real_object ipdfstream<InputStreamT>::take_real_object() {
	using namespace object_types;

	auto front_token = m_tknizer.take_token();

	if (front_token.has_value()) {
		std::regex       real_re("[+-]?([0-9]*)\\.([0-9]*)");
		std::string_view front_view = front_token.value();

		if (std::match_results<std::string_view::const_iterator> result;
		    std::regex_match(front_view.begin(), front_view.end(), result,
		                     real_re) &&
		    (result[1].length() > 0 || result[2].length() > 0)) {
			// may throw std::out_of_range
			return std::stod(std::string(front_view));
		}
	}

	throw object_not_found_error(object_not_found_error::real_object_not_found);
}
template <class InputStreamT>
object_types::string_object ipdfstream<InputStreamT>::take_string_object() {
	using namespace object_types;

	// Litral String
	if (m_tknizer.attempt_token("(")) {
		string_object literal_string;
		// NOTE: at least size_t seems to be enough.
		std::size_t number_of_left_parenthesis  = 1;
		std::size_t number_of_right_parenthesis = 0;
		while (!m_tknizer.eof() &&
		       number_of_left_parenthesis != number_of_right_parenthesis) {
			// assert: noexcept
			switch (const auto ch = m_tknizer.get()) {
			case '(':
				++number_of_left_parenthesis;
				literal_string.push_back('(');
				break;
			case ')':
				++number_of_right_parenthesis;
				if (number_of_left_parenthesis != number_of_right_parenthesis) {
					literal_string.push_back(')');
				}
				break;
			case '\r':
				literal_string.push_back('\n');
				m_tknizer.attempt("\n");
				break;
			case '\\':
				if (m_tknizer.eof()) {
					break;
				}

				// assert: noexcept
				switch (const auto ch_after_backslash = m_tknizer.get()) {
				case 'n':
					literal_string.push_back('\n');
					break;
				case 'r':
					literal_string.push_back('\r');
					break;
				case 't':
					literal_string.push_back('\t');
					break;
				case 'b':
					literal_string.push_back('\b');
					break;
				case 'f':
					literal_string.push_back('\f');
					break;
				case '(':
				case ')':
				case '\\':
					literal_string.push_back(ch_after_backslash);
					break;
				case '\r': // backslash + EOL -> delete
					m_tknizer.attempt("\n");
					break;
				case '\n':
					// do nothing
					break;
				default:
					if ('0' <= ch_after_backslash && ch_after_backslash <= '7') {
						// octal
						uint8_t octal_character =
						    static_cast<uint8_t>(ch_after_backslash - '0');
						if (const auto second_digit = m_tknizer.peek();
						    second_digit.has_value() && '0' <= second_digit &&
						    second_digit <= '7') {
							++m_tknizer;
							octal_character = static_cast<decltype(octal_character)>(
							    8 * octal_character + (second_digit.value() - '0'));
							if (const auto third_digit = m_tknizer.peek();
							    third_digit.has_value() && '0' <= third_digit &&
							    third_digit <= '7') {
								++m_tknizer;
								octal_character = static_cast<decltype(octal_character)>(
								    8 * octal_character + (third_digit.value() - '0'));
							}
						}
						literal_string.push_back(
						    static_cast<decltype(ch_after_backslash)>(octal_character));
					} else {
						literal_string.push_back(ch_after_backslash);
					}
					break;
				}
				break;
			default:
				literal_string.push_back(static_cast<char>(ch));
				break;
			}
		}

		if (number_of_left_parenthesis != number_of_right_parenthesis) {
			throw parse_error(parse_error::literal_string_lack_of_right_parenthesis);
		}

		return literal_string;
	}

	// Hexadecimal String
	if (m_tknizer.attempt_token("<")) {
		string_object hexadecimal_string;

		unsigned char           character     = 0;
		int                     hex_digit_pos = 0;
		std::optional<pdftoken> hex_token_opt;
		using namespace std::string_view_literals;
		while ((hex_token_opt = m_tknizer.take_token()) != ">"sv) {
			// reached end of file
			if (!hex_token_opt.has_value()) {
				throw parse_error(
				    parse_error::hexadecimal_string_lack_of_greater_than_sign);
			}

			std::string_view hex_view = hex_token_opt.value();
			while (!hex_view.empty()) {
				++hex_digit_pos;
				assert(1 == hex_digit_pos || 2 == hex_digit_pos);

				char hex_digit = hex_view.front();
				hex_view.remove_prefix(1);
				if (!std::isxdigit(hex_digit)) {
					throw parse_error(
					    parse_error::hexadecimal_string_non_hexadecimal_digit_found);
				}

				character |= std::stoi(std::string({hex_digit}), nullptr, 16)
				             << (4 * (2 - hex_digit_pos));

				if (2 == hex_digit_pos) {
					hexadecimal_string.push_back(character);
					character     = 0;
					hex_digit_pos = 0;
				}
			}
		}
		if (1 == hex_digit_pos) {
			hexadecimal_string.push_back(character);
		}

		return hexadecimal_string;
	}

	throw object_not_found_error(object_not_found_error::string_object_not_found);
}
template <class InputStreamT>
object_types::name_object ipdfstream<InputStreamT>::take_name_object() {
	using namespace object_types;

	if (m_tknizer.attempt_token("/")) {
		if (auto next_ch = m_tknizer.peek();
		    next_ch.has_value() && is_regular(next_ch.value())) {
			// non empty name object
			const auto       name_token = m_tknizer.take_token().value();
			std::string_view name_view{name_token};
			std::regex       hex_re("#([0-9abcdefABCDEF]{2})");
			std::string      name_str;
			if (std::regex_iterator it(name_view.begin(), name_view.end(), hex_re),
			    end = decltype(it){};
			    it != end) {
				name_str = it->prefix();
				for (; it != end; ++it) {
					name_str.push_back(decltype(name_str)::traits_type::to_char_type(
					    std::stoi(it->str(1), nullptr, 16)));
					name_str += it->suffix();
				}
			} else {
				name_str = name_token;
			}
			return name_object{name_str};
		} else {
			// empty name object
			return name_object{};
		}
	}

	throw object_not_found_error(object_not_found_error::name_object_not_found);
}
template <class InputStreamT>
object_types::array_object ipdfstream<InputStreamT>::take_array_object(
    object_pool<InputStreamT>& object_accessor) {
	using namespace object_types;

	if (m_tknizer.attempt_token("[")) {
		array_object array;

		while (!m_tknizer.attempt_token("]")) {
			if (m_tknizer.no_token()) {
				throw parse_error(parse_error::array_lack_of_right_square_bracket);
			}

			try {
				array.push_back(take_object<any_direct_object_or_ref>(object_accessor));
			} catch (const object_not_found_error& obj_e) {
				if (object_not_found_error::specified_object_not_found ==
				    obj_e.code()) {
					throw parse_error(parse_error::array_invalid_element);
				} else {
					throw;
				}
			}
		}

		return array;
	}

	throw object_not_found_error(object_not_found_error::array_object_not_found);
}
template <class InputStreamT>
object_types::dictionary_object
    ipdfstream<InputStreamT>::take_dictionary_object(
        object_pool<InputStreamT>& object_accessor) {
	using namespace object_types;

	if (m_tknizer.attempt_token("<<")) {
		dictionary_object dictionary;

		while (!m_tknizer.attempt_token(">>")) {
			if (m_tknizer.no_token()) {
				throw parse_error(
				    parse_error::dictionary_lack_of_double_greater_than_sign);
			}

			name_object name;
			try {
				name = take_name_object();
			} catch (const object_not_found_error& obj_e) {
				if (object_not_found_error::name_object_not_found == obj_e.code()) {
					throw parse_error(parse_error::dictionary_invalid_key);
				} else {
					throw;
				}
			}

			any_direct_object_or_ref value;
			try {
				value = take_object<any_direct_object_or_ref>(object_accessor);
			} catch (const object_not_found_error& obj_e) {
				if (object_not_found_error::specified_object_not_found ==
				    obj_e.code()) {
					throw parse_error(parse_error::dictionary_invalid_value);
				} else {
					throw;
				}
			}

			// emplace to dictionary
			std::visit(
			    [&dictionary, &name](auto&& concrete_value) {
				    using T = std::decay_t<decltype(concrete_value)>;
				    if constexpr (!std::is_same_v<null_object, T>) {
					    dictionary.emplace(std::move(name), std::move(concrete_value));
				    }
			    },
			    std::move(value));
		}

		return dictionary;
	}

	throw object_not_found_error(
	    object_not_found_error::dictionary_object_not_found);
}
template <class InputStreamT>
object_types::stream_object ipdfstream<InputStreamT>::take_stream_object(
    object_pool<InputStreamT>& object_accessor) {
	using namespace object_types;

	dictionary_object stream_dictionary;
	try {
		stream_dictionary = take_dictionary_object(object_accessor);
	} catch (const object_not_found_error& obj_e) {
		if (object_not_found_error::dictionary_object_not_found == obj_e.code()) {
			throw object_not_found_error(
			    object_not_found_error::stream_object_not_found);
		} else {
			throw;
		}
	}
	return take_stream_object(object_accessor, std::move(stream_dictionary));
}
template <class InputStreamT>
template <class DictionaryObject>
object_types::stream_object ipdfstream<InputStreamT>::take_stream_object(
    object_pool<InputStreamT>& object_accessor,
    DictionaryObject&&         stream_dictionary) {
	using namespace object_types;

	static_assert(
	    std::is_same_v<dictionary_object, std::decay_t<DictionaryObject>>);

	if (!m_tknizer.attempt_token("stream")) {
		throw object_not_found_error(
		    object_not_found_error::stream_object_not_found);
	}

	m_tknizer.promise({"\r\n", "\n"});

	const auto length_it = stream_dictionary.find("Length");
	if (length_it == stream_dictionary.end()) {
		throw parse_error(parse_error::stream_dictionary_absence_of_Length_entry);
	}

	const std::size_t stream_length =
	    object_accessor.dereference<integer_object>(length_it->second);

	std::string stream_data;
	stream_data.reserve(stream_length);
	for (std::size_t repeat_ = 0; repeat_ < stream_length; ++repeat_) {
		try {
			// HACK: stream_length
			// バイト読み取り中に、Filterに対する明示的なEODマーカーが出現した場合にエラーにする
			stream_data.push_back(m_tknizer.get());
		} catch (istream_extended_error&) {
			throw parse_error(parse_error::stream_data_is_shorter_than_Length);
		}
	}
	// at least one EOL is required
	m_tknizer.promise({"\r\n", "\n", "\r"});

	// additional EOLs are permitted
	m_tknizer.ignore_if_present(whitespace_flags::EOL);

	m_tknizer.promise_token({"endstream"});

	return stream_object{std::forward<DictionaryObject>(stream_dictionary),
	                     std::move(stream_data)};
}
template <class InputStreamT>
object_types::null_object ipdfstream<InputStreamT>::take_null_object() {
	if (m_tknizer.attempt_token("null")) {
		return object_types::null;
	}

	throw object_not_found_error(object_not_found_error::null_object_not_found);
}
template <class InputStreamT>
object_types::indirect_reference
    ipdfstream<InputStreamT>::take_indirect_reference() {
	using namespace object_types;

	xref_types::object_t     object_number;
	xref_types::generation_t generation_number;
	try {
		object_number     = take_integer_object();
		generation_number = take_integer_object();
	} catch (const object_not_found_error& obj_e) {
		if (object_not_found_error::integer_object_not_found == obj_e.code()) {
			throw object_not_found_error(
			    object_not_found_error::indirect_reference_not_found);
		} else {
			throw;
		}
	}

	if (m_tknizer.attempt_token("R")) {
		return indirect_reference{object_number, generation_number};
	}

	throw object_not_found_error(
	    object_not_found_error::indirect_reference_not_found);
}

template <class InputStreamT>
object_types::any_direct_object ipdfstream<InputStreamT>::take_indirect_object(
    object_pool<InputStreamT>&          object_accessor,
    const xref_types::xref_inuse_entry& object_info) {
	auto before_take_indirect_object_pos = m_tknizer.tell();
	m_tknizer.seek(object_info.byte_offset);

	xref_types::object_t     first_integer  = take_integer_object();
	xref_types::generation_t second_integer = take_integer_object();
	if (first_integer != object_info.object_number ||
	    second_integer != object_info.generation_number) {
		throw parse_error(
		    parse_error::indirect_object_is_inconsistent_with_xref_table);
	}

	m_tknizer.promise_token({"obj"});

	auto object = take_object<object_types::any_direct_object>(object_accessor);

	m_tknizer.promise_token({"endobj"});

	m_tknizer.seek(before_take_indirect_object_pos);

	return object;
}
#pragma endregion // region stream_parser_definitions
} // namespace pdfparser
