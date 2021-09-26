#pragma once

#include <vector>

namespace pdfparser::line_style {
enum class cap_type { butt, round, square };
struct dash_pattern_type {
	std::vector<double> pattern_array;
	double              start_phase;
};
enum class join_type { miter, round, bevel };
} // namespace pdfparser::line_style
