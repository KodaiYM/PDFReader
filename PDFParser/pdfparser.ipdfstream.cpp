#include "pdfparser.ipdfstream.hpp"

using namespace pdfparser;

// definition of member functions of ipdfstream
#pragma region                  stream_parser_definitions
object_types::dictionary_object ipdfstream::take_footer() {
	seek_to_end();
	seek_forward_head_of_line();
	seek_forward_head_of_line();
	seek_forward_head_of_line();

	// check keyword "startxref"
	promise_token({"startxref"});

	// get cross-reference table byte offset
	std::streamoff xref_byte_offset = take_integer_object();

	// check %%EOF
	promise({"\r\n", "\n", "\r"});
	promise({"%%EOF"});

	// get cross-reference table
	seek(xref_byte_offset);
	auto xref_table = take_xref_table();

	// get trailer dictionary
	add_xref_table(xref_table);
	return take_trailer();
}

xref_types::xref_table ipdfstream::take_xref_table() {
	using namespace xref_types;

	xref_table this_xref_table;

	promise_token({"xref"});

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

xref_types::xref_entry
    ipdfstream::take_xref_entry(xref_types::object_t object_number) {
	using namespace std::string_view_literals;

	auto first_integer  = take_integer_object();
	auto second_integer = take_integer_object();

	auto keyword_opt = take_token();
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

object_types::dictionary_object ipdfstream::take_trailer() {
	promise_token({"trailer"});
	return take_dictionary_object();
}
object_types::boolean_object ipdfstream::take_boolean_object() {
	using namespace object_types;

	if (attempt_token("true")) {
		return boolean_object(true);
	} else if (attempt_token("false")) {
		return boolean_object(false);
	}

	throw object_not_found_error(
	    object_not_found_error::boolean_object_not_found);
}

object_types::integer_object ipdfstream::take_integer_object() {
	auto front_token = take_token();

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

object_types::real_object ipdfstream::take_real_object() {
	using namespace object_types;

	auto front_token = take_token();

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

object_types::string_object ipdfstream::take_string_object() {
	using namespace object_types;

	// Litral String
	if (attempt_token("(")) {
		string_object literal_string;
		// NOTE: at least size_t seems to be enough.
		std::size_t number_of_left_parenthesis  = 1;
		std::size_t number_of_right_parenthesis = 0;
		while (!eof() &&
		       number_of_left_parenthesis != number_of_right_parenthesis) {
			// assert: noexcept
			switch (const auto ch = get()) {
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
				attempt("\n");
				break;
			case '\\':
				if (eof()) {
					break;
				}

				// assert: noexcept
				switch (const auto ch_after_backslash = get()) {
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
					attempt("\n");
					break;
				case '\n':
					// do nothing
					break;
				default:
					if ('0' <= ch_after_backslash && ch_after_backslash <= '7') {
						// octal
						uint8_t octal_character =
						    static_cast<uint8_t>(ch_after_backslash - '0');
						if (const auto second_digit = peek(); second_digit.has_value() &&
						                                      '0' <= second_digit &&
						                                      second_digit <= '7') {
							++*this;
							octal_character = static_cast<decltype(octal_character)>(
							    8 * octal_character + (second_digit.value() - '0'));
							if (const auto third_digit = peek(); third_digit.has_value() &&
							                                     '0' <= third_digit &&
							                                     third_digit <= '7') {
								++*this;
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
	if (attempt_token("<")) {
		string_object hexadecimal_string;

		unsigned char           character     = 0;
		int                     hex_digit_pos = 0;
		std::optional<pdftoken> hex_token_opt;
		using namespace std::string_view_literals;
		while ((hex_token_opt = take_token()) != ">"sv) {
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

object_types::name_object ipdfstream::take_name_object() {
	using namespace object_types;

	if (attempt_token("/")) {
		if (auto next_ch = peek();
		    next_ch.has_value() && is_regular(next_ch.value())) {
			// non empty name object
			const auto       name_token = take_token().value();
			std::string_view name_view{name_token};
			std::regex       hex_re("#([0-9abcdefABCDEF]{2})");
			std::string      name_str;
			if (std::regex_iterator it(name_view.begin(), name_view.end(), hex_re),
			    last = decltype(it){};
			    it != last) {
				name_str = it->prefix();
				for (; it != last; ++it) {
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

object_types::array_object ipdfstream::take_array_object() {
	using namespace object_types;

	if (attempt_token("[")) {
		array_object array;

		while (!attempt_token("]")) {
			if (no_token()) {
				throw parse_error(parse_error::array_lack_of_right_square_bracket);
			}

			try {
				array.push_back(take_object<any_direct_object_or_ref>());
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

object_types::dictionary_object ipdfstream::take_dictionary_object() {
	using namespace object_types;

	if (attempt_token("<<")) {
		dictionary_object dictionary;

		while (!attempt_token(">>")) {
			if (no_token()) {
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
				value = take_object<any_direct_object_or_ref>();
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

object_types::stream_object ipdfstream::take_stream_object() {
	using namespace object_types;

	dictionary_object stream_dictionary;
	try {
		stream_dictionary = take_dictionary_object();
	} catch (const object_not_found_error& obj_e) {
		if (object_not_found_error::dictionary_object_not_found == obj_e.code()) {
			throw object_not_found_error(
			    object_not_found_error::stream_object_not_found);
		} else {
			throw;
		}
	}
	return take_stream_object(std::move(stream_dictionary));
}
object_types::null_object ipdfstream::take_null_object() {
	if (attempt_token("null")) {
		return object_types::null;
	}

	throw object_not_found_error(object_not_found_error::null_object_not_found);
}

object_types::indirect_reference ipdfstream::take_indirect_reference() {
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

	if (attempt_token("R")) {
		return indirect_reference{object_number, generation_number};
	}

	throw object_not_found_error(
	    object_not_found_error::indirect_reference_not_found);
}

object_types::any_direct_object ipdfstream::take_indirect_object(
    const xref_types::xref_inuse_entry& object_info) {
	auto before_take_indirect_object_pos = tell();
	seek(object_info.byte_offset);

	xref_types::object_t     first_integer  = take_integer_object();
	xref_types::generation_t second_integer = take_integer_object();
	if (first_integer != object_info.object_number ||
	    second_integer != object_info.generation_number) {
		throw parse_error(
		    parse_error::indirect_object_is_inconsistent_with_xref_table);
	}

	promise_token({"obj"});

	auto object = take_object<object_types::any_direct_object>();

	promise_token({"endobj"});

	seek(before_take_indirect_object_pos);

	return object;
}
#pragma endregion // stream_parser_definitions

// definition of member functions from old object_pool
#pragma region stream_parser_definitions_on_old_object_pool
void           ipdfstream::add_xref_table(
    const xref_types::xref_table& referenced_xref_table) noexcept {
	m_xref_table = std::move(referenced_xref_table);
}

#pragma endregion // stream_parser_definitions_on_old_object_pool
