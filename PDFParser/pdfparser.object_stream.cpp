#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"

#include <regex>
#include <string>
#include <string_view>

using namespace pdfparser;

// definition of member functions of object_stream
#pragma region                              object_stream_definitions
object_types::onstream_direct_object_or_ref object_stream::take_object() {
	using namespace object_types;

	const auto before_take_object_pos = tell();

	try {
		return take_boolean_object();
	} catch (boolean_object_not_found&) { seek(before_take_object_pos); }

	try {
		return take_real_object();
	} catch (real_object_not_found&) { seek(before_take_object_pos); }

	try {
		return take_string_object();
	} catch (string_object_not_found&) { seek(before_take_object_pos); }

	try {
		return take_name_object();
	} catch (name_object_not_found&) { seek(before_take_object_pos); }

	try {
		return take_array_object();
	} catch (array_object_not_found&) { seek(before_take_object_pos); }

	try {
		return take_null_object();
	} catch (null_object_not_found&) { seek(before_take_object_pos); }

	{
		std::optional<onstream_dictionary_object> dictionary;
		try {
			dictionary = take_dictionary_object();
		} catch (dictionary_object_not_found&) { seek(before_take_object_pos); }

		if (dictionary.has_value()) {
			auto before_take_stream_pos = tell();
			try {
				return take_stream_object(dictionary.value());
			} catch (stream_object_not_found&) {
				seek(before_take_stream_pos);
				return std::move(dictionary).value();
			}
		}
	}

	try {
		return take_indirect_reference();
	} catch (indirect_reference_not_found&) { seek(before_take_object_pos); }

	try {
		return take_integer_object();
	} catch (integer_object_not_found&) { seek(before_take_object_pos); }

	throw specified_object_not_found(before_take_object_pos);
}

object_types::onstream_boolean_object object_stream::take_boolean_object() {
	using namespace object_types;

	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);

	const auto take_pos = tell();
	if (attempt_token("true")) {
		return onstream_boolean_object(take_pos, true);
	} else if (attempt_token("false")) {
		return onstream_boolean_object(take_pos, false);
	}

	throw boolean_object_not_found(take_pos);
}

object_types::onstream_integer_object object_stream::take_integer_object() {
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	auto front_token = take_token();

	if (front_token.has_value()) {
		std::regex       integer_re("[+-]?[0-9]+");
		std::string_view front_view = front_token.value();

		if (std::regex_match(front_view.begin(), front_view.end(), integer_re)) {
			return {take_pos, std::string(front_view)};
		}
	}

	throw integer_object_not_found(take_pos);
}

object_types::onstream_real_object object_stream::take_real_object() {
	using namespace object_types;
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	auto front_token = take_token();

	if (front_token.has_value()) {
		std::regex       real_re("[+-]?([0-9]*)\\.([0-9]*)");
		std::string_view front_view = front_token.value();

		if (std::match_results<std::string_view::const_iterator> result;
		    std::regex_match(front_view.begin(), front_view.end(), result,
		                     real_re) &&
		    (result[1].length() > 0 || result[2].length() > 0)) {
			try {
				return {take_pos, std::stod(std::string(front_view))};
			} catch (std::out_of_range&) { throw real_object_overflows(take_pos); }
		}
	}

	throw real_object_not_found(take_pos);
}

object_types::onstream_string_object object_stream::take_string_object() {
	using namespace object_types;
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	// Litral String
	if (attempt_token("(")) {
		onstream_string_object literal_string{take_pos, ""};
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
			throw literal_string_lack_of_right_parenthesis(take_pos);
		}

		return literal_string;
	}

	// Hexadecimal String
	if (attempt_token("<")) {
		onstream_string_object hexadecimal_string{take_pos, ""};

		unsigned char           character     = 0;
		int                     hex_digit_pos = 0;
		std::optional<pdftoken> hex_token_opt;
		using namespace std::string_view_literals;
		while ((hex_token_opt = take_token()) != ">"sv) {
			// reached end of file
			if (!hex_token_opt.has_value()) {
				throw hexadecimal_string_lack_of_greater_than_sign(take_pos);
			}

			auto             hex_token_last_pos = tell();
			std::string_view hex_view           = hex_token_opt.value();
			for (std::size_t i = 0; i < hex_view.length(); ++i) {
				++hex_digit_pos;
				assert(1 == hex_digit_pos || 2 == hex_digit_pos);

				char hex_digit = hex_view.at(i);
				if (!std::isxdigit(hex_digit)) {
					throw hexadecimal_string_non_hexadecimal_digit_found(
					    hex_token_last_pos - hex_view.length() + i);
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

	throw string_object_not_found(take_pos);
}

object_types::onstream_name_object object_stream::take_name_object() {
	using namespace object_types;
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

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
			return onstream_name_object{take_pos, name_str};
		} else {
			// empty name object
			return onstream_name_object{take_pos, ""};
		}
	}

	throw name_object_not_found(take_pos);
}

object_types::onstream_array_object object_stream::take_array_object() {
	using namespace object_types;
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	if (attempt_token("[")) {
		onstream_array_object array{take_pos, {}};

		while (!attempt_token("]")) {
			if (no_token()) {
				throw array_lack_of_right_square_bracket(take_pos);
			}

			array.push_back(take_object());
		}

		return array;
	}

	throw array_object_not_found(take_pos);
}

object_types::onstream_dictionary_object
    object_stream::take_dictionary_object() {
	using namespace object_types;
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	if (attempt_token("<<")) {
		onstream_dictionary_object dictionary{take_pos, {}};

		while (!attempt_token(">>")) {
			if (no_token()) {
				throw dictionary_lack_of_double_greater_than_sign(take_pos);
			}

			auto name  = take_name_object();
			auto value = take_object();

			// emplace to dictionary
			std::visit(
			    [&dictionary, &name](auto&& concrete_value) {
				    using T = std::decay_t<decltype(concrete_value)>;
				    if constexpr (!std::is_same_v<onstream_null_object, T>) {
					    dictionary.emplace(std::move(name), std::move(concrete_value));
				    }
			    },
			    std::move(value));
		}

		return dictionary;
	}

	throw dictionary_object_not_found(take_pos);
}

object_types::onstream_stream_object object_stream::take_stream_object() {
	using namespace object_types;
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	std::optional<onstream_dictionary_object> stream_dictionary;
	try {
		stream_dictionary = take_dictionary_object();
	} catch (dictionary_object_not_found&) {
		throw stream_object_not_found(take_pos);
	}

	return take_stream_object(std::move(stream_dictionary.value()));
}

object_types::onstream_stream_object object_stream::take_stream_object(
    object_types::onstream_dictionary_object stream_dictionary) {
	using namespace object_types;

	if (!attempt_token("stream")) {
		throw stream_object_not_found(stream_dictionary.position());
	}

	promise({"\r\n", "\n"});

	const auto length_it = stream_dictionary.find("Length");
	if (length_it == stream_dictionary.end()) {
		throw stream_dictionary_absence_of_Length_entry(
		    stream_dictionary.position());
	}

	const std::streamsize stream_length = static_cast<onstream_integer_object>(
	    dereference(length_it->second.get()));

	std::string stream_data;
	stream_data.reserve(stream_length);
	auto head_of_data = tell();
	for (std::streamsize repeat_ = 0; repeat_ < stream_length; ++repeat_) {
		if (eof()) {
			throw stream_data_is_shorter_than_Length(head_of_data, stream_length);
		} else {
			// HACK: stream_length
			// バイト読み取り中に、Filterに対する明示的なEODマーカーが出現した場合にエラーにする
			stream_data.push_back(get());
		}
	}
	// at least one EOL is required
	promise({"\r\n", "\n", "\r"});

	// additional EOLs are permitted
	ignore_if_present(whitespace_flags::EOL);

	promise_token({"endstream"});

	return onstream_stream_object{std::move(stream_dictionary),
	                              std::move(stream_data)};
}

object_types::onstream_null_object object_stream::take_null_object() {
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	if (attempt_token("null")) {
		return object_types::onstream_null_object{take_pos};
	}

	throw null_object_not_found(take_pos);
}

object_types::onstream_indirect_reference
    object_stream::take_indirect_reference() {
	using namespace object_types;
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	const auto take_pos = tell();

	xref_types::object_t     object_number;
	xref_types::generation_t generation_number;
	try {
		object_number     = take_integer_object();
		generation_number = take_integer_object();
	} catch (integer_object_not_found&) {
		throw indirect_reference_not_found(take_pos);
	}

	if (attempt_token("R")) {
		return onstream_indirect_reference{take_pos,
		                                   {object_number, generation_number}};
	}

	throw indirect_reference_not_found(take_pos);
}

object_types::onstream_direct_object object_stream::take_indirect_object(
    const xref_types::xref_inuse_entry& object_info) {
	auto before_take_indirect_object_pos = tell();
	seek(object_info.byte_offset);

	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);
	auto begin_of_indirect_object = tell();

	xref_types::object_t     first_integer  = take_integer_object();
	xref_types::generation_t second_integer = take_integer_object();
	if (first_integer != object_info.object_number ||
	    second_integer != object_info.generation_number) {
		throw indirect_object_is_inconsistent_with_xref_table(
		    begin_of_indirect_object, first_integer, second_integer, object_info);
	}

	promise_token({"obj"});

	object_types::onstream_direct_object object = take_object();

	promise_token({"endobj"});

	seek(before_take_indirect_object_pos);

	return object;
}
#pragma endregion // object_stream_definitions

// definition of member functions from old object_cache
#pragma region object_stream_definitions_on_old_object_cache
void           object_stream::add_xref_table(
    const xref_types::xref_table& referenced_xref_table) noexcept {
	m_xref_table = std::move(referenced_xref_table);
}
#pragma endregion // object_stream_definitions_on_old_object_cache
