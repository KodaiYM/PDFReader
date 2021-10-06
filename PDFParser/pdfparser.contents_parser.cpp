#include "pdfparser.contents_parser.hpp"
#include "pdfparser.object_not_found_error.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.pdfcontents_builder.hpp"
#include "variant_cast.hpp"

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
	pdfcontents_builder            builder;
	std::vector<any_direct_object> operands;

	const std::unordered_map<std::string, std::function<void(void)>> operations =
	    {{"q",
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
			      number_object number_operand = variant_cast(std::move(operand));
			      coefficients.push_back(number_to_double(std::move(number_operand)));
		      }

		      builder.concat_CTM(
		          {{coefficients.at(0), coefficients.at(1), coefficients.at(2),
		            coefficients.at(3), coefficients.at(4), coefficients.at(5)}});
	      }},
	     {"d",
	      [&]() {
		      auto pattern_array =
		          std::move(std::get<array_object>(operands.at(0)));
		      std::vector<number_object> pattern_number_array =
		          std::move(pattern_array);

		      std::vector<double> pattern_vector;
		      pattern_vector.reserve(pattern_number_array.size());
		      for (auto& element : pattern_number_array) {
			      pattern_vector.push_back(number_to_double(std::move(element)));
		      }

		      double start_phase =
		          number_to_double(std::move(variant_cast(operands.at(1))));
		      builder.set_line_dash_pattern(
		          {std::move(pattern_vector), std::move(start_phase)});
	      }},
	     {"i",
	      [&]() {
		      builder.set_flatness(
		          number_to_double(variant_cast(std::move(operands.at(0)))));
	      }},
	     {"j",
	      [&]() {
		      integer_object::int_type join_type_number =
		          std::get<integer_object>(std::move(operands.at(0)));
		      builder.set_line_join_style(
		          static_cast<line_style::join_type>(std::move(join_type_number)));
	      }},
	     {"J",
	      [&]() {
		      integer_object::int_type cap_type_number =
		          std::get<integer_object>(std::move(operands.at(0)));
		      builder.set_line_cap_style(
		          static_cast<line_style::cap_type>(std::move(cap_type_number)));
	      }},
	     {"M",
	      [&]() {
		      number_object miter_limit = variant_cast(std::move(operands.at(0)));
		      builder.set_miter_limit(number_to_double(miter_limit));
	      }},
	     {"w",
	      [&]() {
		      number_object line_width = variant_cast(std::move(operands.at(0)));
		      builder.set_line_width(number_to_double(line_width));
	      }},
	     {"g",
	      [&]() {
		      double intensity =
		          number_to_double(variant_cast(std::move(operands.at(0))));
		      chroma::grayscale_color color{intensity};
		      builder.set_fill_color(color);
	      }},
	     {"G",
	      [&]() {
		      double intensity =
		          number_to_double(variant_cast(std::move(operands.at(0))));
		      chroma::grayscale_color color{intensity};
		      builder.set_stroke_color(color);
	      }},
	     {"k",
	      [&]() {
		      double cyan =
		          number_to_double(variant_cast(std::move(operands.at(0))));
		      double magenta =
		          number_to_double(variant_cast(std::move(operands.at(1))));
		      double yellow =
		          number_to_double(variant_cast(std::move(operands.at(2))));
		      double black =
		          number_to_double(variant_cast(std::move(operands.at(3))));
		      chroma::cmyk_color color{cyan, magenta, yellow, black};
		      builder.set_fill_color(color);
	      }},
	     {"K",
	      [&]() {
		      double cyan =
		          number_to_double(variant_cast(std::move(operands.at(0))));
		      double magenta =
		          number_to_double(variant_cast(std::move(operands.at(1))));
		      double yellow =
		          number_to_double(variant_cast(std::move(operands.at(2))));
		      double black =
		          number_to_double(variant_cast(std::move(operands.at(3))));
		      chroma::cmyk_color color{cyan, magenta, yellow, black};
		      builder.set_stroke_color(color);
	      }},
	     {"rg",
	      [&]() {
		      double red =
		          number_to_double(variant_cast(std::move(operands.at(0))));
		      double green =
		          number_to_double(variant_cast(std::move(operands.at(1))));
		      double blue =
		          number_to_double(variant_cast(std::move(operands.at(2))));
		      chroma::rgb_color color{red, green, blue};
		      builder.set_fill_color(color);
	      }},
	     {"rg", [&]() {
		      double red =
		          number_to_double(variant_cast(std::move(operands.at(0))));
		      double green =
		          number_to_double(variant_cast(std::move(operands.at(1))));
		      double blue =
		          number_to_double(variant_cast(std::move(operands.at(2))));
		      chroma::rgb_color color{red, green, blue};
		      builder.set_stroke_color(color);
	      }}};

	while (true) {
		auto before_take_object_pos = m_content_stream.tell();
		try {
			auto object = m_content_stream.take_object<any_direct_object>();
			operands.push_back(std::move(object));
		} catch (object_not_found_error& obj_e) {
			assert(object_not_found_error::specified_object_not_found ==
			       obj_e.code());

			m_content_stream.seek(before_take_object_pos);
			auto contents_operator = m_content_stream.take_token();
			if (!contents_operator.has_value()) {
				break;
			}
		}
	}

	return gcnew PDFReader::PDFContents;
}
