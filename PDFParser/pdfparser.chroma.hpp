#pragma once

#include <variant>

namespace pdfparser::chroma {
struct rgb_color {
	double red   = 0;
	double green = 0;
	double blue  = 0;
};
struct cmyk_color {
	double cyan    = 0;
	double magenta = 0;
	double yellow  = 0;
	double black   = 1;
};
struct grayscale_color {
	double intensity = 0;
};

using color = std::variant<rgb_color, cmyk_color, grayscale_color>;

constexpr rgb_color       rgb_black       = {0, 0, 0};
constexpr cmyk_color      cmyk_black      = {0, 0, 0, 1};
constexpr grayscale_color grayscale_black = {0};
} // namespace pdfparser::chroma
