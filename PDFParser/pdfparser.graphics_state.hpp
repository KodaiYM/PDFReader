#pragma once

#include "pdfparser.chroma.hpp"
#include "pdfparser.line_style.hpp"
#include "pdfparser.space.hpp"
#include "pdfparser.text_style.hpp"

namespace pdfparser {
struct graphics_state {
	// general parameters
	// clipping_path
	AffineMatrix         current_transformation_matrix = AffineMatrix::Identity();
	coordinates          current_point;
	chroma::color        fill_color                 = chroma::grayscale_black;
	double               flatness                   = 0;
	line_style::cap_type line_cap_style             = line_style::cap_type::butt;
	line_style::dash_pattern_type line_dash_pattern = {{}, 0};
	line_style::join_type         line_join_style = line_style::join_type::miter;
	double                        line_width      = 1;
	double                        miter_width     = 10;
	chroma::color                 stroke_color    = chroma::grayscale_black;

	// text-specific parameters
	double character_spacing  = 0;
	double horizontal_scaling = 100;
	double leading            = 0;
	// text_font
	double                     text_rise = 0;
	double                     text_size;
	text_style::rendering_mode text_rendering_mode =
	    text_style::rendering_mode::fill;
	double word_spacing = 0;
};
} // namespace pdfparser
