#include "pdfparser.contents_parser.hpp"
#include "pdfparser.contents_parser_errors.hpp"
#include "pdfparser.document_error.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.pdfcontents_builder.hpp"

#include <cassert>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace pdfparser;

contents_parser::contents_parser(ipdfstream& content_stream) noexcept
    : m_content_stream(content_stream) {}

// TODO: ÉeÉXÉgÇèëÇ≠
PDFReader::PDFContents ^ contents_parser::get_contents() {
	using namespace object_types;
	pdfcontents_builder                 builder;
	std::vector<onstream_direct_object> operands;

	const std::unordered_map<std::string_view, std::function<void(void)>>
	    operations = {
	        {"q",
	         [&]() {
		         // builder.store_graphics_state();
	         }},
	        {"Q",
	         [&]() {
		         // builder.restore_graphics_state();
	         }},
	        {"cm",
	         [&]() {
		         std::vector<double> coefficients;
		         coefficients.reserve(operands.size());
		         for (auto& operand : operands) {
			         onstream_number_object number_operand = std::move(operand);
			         coefficients.push_back(std::move(number_operand));
		         }

		         builder.concat_CTM({{coefficients.at(0), coefficients.at(1),
		                              coefficients.at(2), coefficients.at(3),
		                              coefficients.at(4), coefficients.at(5)}});
	         }},
	        {"d",
	         [&]() {
		         onstream_array_object pattern_array = std::move(operands.at(0));

		         std::vector<double> pattern_vector;
		         pattern_vector.reserve(pattern_array.size());
		         std::for_each(
		             pattern_array.begin(), pattern_array.end(),
		             [&pattern_vector](auto& element) {
			             pattern_vector.push_back(
			                 static_cast<object_types::onstream_number_object&&>(
			                     element));
		             });

		         double start_phase =
		             static_cast<onstream_real_object>(std::move(operands.at(1)));
		         builder.set_line_dash_pattern(
		             {std::move(pattern_vector), std::move(start_phase)});
	         }},
	        {"i",
	         [&]() {
		         builder.set_flatness(
		             onstream_real_object(std::move(operands.at(0))));
	         }},
	        {"j",
	         [&]() {
		         onstream_integer_object::int_type join_type_number =
		             static_cast<onstream_integer_object>(
		                 std::move(operands.at(0)));
		         builder.set_line_join_style(static_cast<line_style::join_type>(
		             std::move(join_type_number)));
	         }},
	        {"J",
	         [&]() {
		         onstream_integer_object::int_type cap_type_number =
		             static_cast<onstream_integer_object>(
		                 std::move(operands.at(0)));
		         builder.set_line_cap_style(
		             static_cast<line_style::cap_type>(std::move(cap_type_number)));
	         }},
	        {"M",
	         [&]() {
		         onstream_number_object miter_limit = std::move(operands.at(0));
		         builder.set_miter_limit(miter_limit);
	         }},
	        {"w",
	         [&]() {
		         onstream_number_object line_width = std::move(operands.at(0));
		         builder.set_line_width(line_width);
	         }},
	        {"g",
	         [&]() {
		         onstream_number_object  intensity = std::move(operands.at(0));
		         chroma::grayscale_color color{intensity};
		         builder.set_fill_color(color);
	         }},
	        {"G",
	         [&]() {
		         onstream_number_object  intensity = std::move(operands.at(0));
		         chroma::grayscale_color color{intensity};
		         builder.set_stroke_color(color);
	         }},
	        {"k",
	         [&]() {
		         onstream_number_object cyan    = std::move(operands.at(0));
		         onstream_number_object magenta = std::move(operands.at(1));
		         onstream_number_object yellow  = std::move(operands.at(2));
		         onstream_number_object black   = std::move(operands.at(3));
		         chroma::cmyk_color     color{cyan, magenta, yellow, black};
		         builder.set_fill_color(color);
	         }},
	        {"K",
	         [&]() {
		         onstream_number_object cyan    = std::move(operands.at(0));
		         onstream_number_object magenta = std::move(operands.at(1));
		         onstream_number_object yellow  = std::move(operands.at(2));
		         onstream_number_object black   = std::move(operands.at(3));
		         chroma::cmyk_color     color{cyan, magenta, yellow, black};
		         builder.set_stroke_color(color);
	         }},
	        {"rg",
	         [&]() {
		         onstream_number_object red   = std::move(operands.at(0));
		         onstream_number_object green = std::move(operands.at(1));
		         onstream_number_object blue  = std::move(operands.at(2));
		         chroma::rgb_color      color{red, green, blue};
		         builder.set_fill_color(color);
	         }},
	        {"RG",
	         [&]() {
		         onstream_number_object red   = std::move(operands.at(0));
		         onstream_number_object green = std::move(operands.at(1));
		         onstream_number_object blue  = std::move(operands.at(2));
		         chroma::rgb_color      color{red, green, blue};
		         builder.set_stroke_color(color);
	         }},
	        {"BT",
	         [&]() {
		         builder.begin_text();
	         }},
	        {"Tc",
	         [&]() {
		         onstream_number_object character_spacing =
		             std::move(operands.at(0));
		         builder.set_character_spacing(character_spacing);
	         }},
	        {"Tf",
	         [&]() {
		         throw "ñ¢é¿ëï";
	         }},
	        {"TL",
	         [&]() {
		         onstream_number_object leading = std::move(operands.at(0));
		         builder.set_leading(leading);
	         }},
	        {"Tr",
	         [&]() {
		         onstream_integer_object::int_type mode_number =
		             static_cast<onstream_integer_object>(
		                 std::move(operands.at(0)));
		         builder.set_text_rendering_mode(
		             static_cast<text_style::rendering_mode>(mode_number));
	         }},
	        {"Ts",
	         [&]() {
		         onstream_number_object text_rise = std::move(operands.at(0));
		         builder.set_text_rise(text_rise);
	         }},
	        {"Tw",
	         [&]() {
		         onstream_number_object word_spacing = std::move(operands.at(0));
		         builder.set_word_spacing(word_spacing);
	         }},
	        {"Tz",
	         [&]() {
		         onstream_number_object horizontal_spacing =
		             std::move(operands.at(0));
		         builder.set_word_spacing(horizontal_spacing);
	         }},
	        {"Td",
	         [&]() {
		         onstream_number_object next_line_offset_x =
		             std::move(operands.at(0));
		         onstream_number_object next_line_offset_y =
		             std::move(operands.at(1));

		         builder.concat_text_matrices(next_line_offset_x,
		                                      next_line_offset_y);
	         }},
	        {"TD",
	         [&]() {
		         onstream_number_object next_line_offset_x =
		             std::move(operands.at(0));
		         onstream_number_object next_line_offset_y =
		             std::move(operands.at(1));

		         builder.set_leading(-next_line_offset_y);
		         builder.concat_text_matrices(next_line_offset_x,
		                                      next_line_offset_y);
	         }},
	        {"Tm",
	         [&]() {
		         onstream_number_object next_line_x = std::move(operands.at(0));
		         onstream_number_object next_line_y = std::move(operands.at(1));

		         builder.set_text_matrices(next_line_x, next_line_y);
	         }},
	        {"T*",
	         [&]() {
		         builder.move_to_next_line();
	         }},
	        {"ET",
	         [&]() {
		         builder.end_text();
	         }},
	    };

	while (true) {
		auto before_take_object_pos = m_content_stream.tell();
		try {
			onstream_direct_object object = m_content_stream.take_object();
			operands.push_back(std::move(object));
		} catch (specified_object_not_found&) {
			m_content_stream.seek(before_take_object_pos);
			auto operator_token = m_content_stream.take_token();
			if (!operator_token.has_value()) {
				break;
			}

			if (auto operation_pair = operations.find(operator_token.value());
			    operation_pair != operations.end()) {
				operation_pair->second();
				operands.clear();
			} else {
				throw unknown_operator(operator_token.value(),
				                       operator_token.value().position());
			}
		}
	}

	return builder.build();
}
