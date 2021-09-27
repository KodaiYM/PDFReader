#pragma once

#include "pdfparser.chroma.hpp"
#include "pdfparser.graphics_state_stack.hpp"
#include "pdfparser.line_style.hpp"
#include "pdfparser.space.hpp"
#include "pdfparser.text_style.hpp"

namespace pdfparser {
class pdfcontents_builder {
public:
	void concat_CTM(const AffineMatrix& concat_matrix);
	void set_line_dash_pattern(
	    line_style::dash_pattern_type line_dash_pattern) noexcept;
	void set_flatness(double flatness) noexcept;
	void set_line_join_style(line_style::join_type line_join_style) noexcept;
	void set_line_cap_style(line_style::cap_type line_cap_style) noexcept;
	void set_miter_limit(double miter_limit) noexcept;
	void set_line_width(double line_width) noexcept;
	void set_fill_color(chroma::color fill_color) noexcept;
	void set_stroke_color(chroma::color stroke_color) noexcept;
	void set_character_spacing(double character_spacing) noexcept;
	void set_leading(double leading) noexcept;
	void set_text_rendering_mode(
	    text_style::rendering_mode text_rendering_mode) noexcept;
	void set_text_rise(double text_rise) noexcept;
	void set_word_spacing(double word_spacing) noexcept;
	void set_horizontal_scaling(double horizontal_scaling) noexcept;

	void set_text_matrices(double next_line_x, double next_line_y) noexcept;
	void concat_text_matrices(double next_line_offset_x,
	                          double next_line_offset_y) noexcept;
	void move_to_next_line();
	void begin_text();
	void end_text() noexcept;

private:
	graphics_state_stack m_graphics_states;
	AffineMatrix         m_text_matrix;
	AffineMatrix         m_text_line_matrix;
};
} // namespace pdfparser
