#pragma once

namespace pdfparser::data_types {
struct coordinates {
	double x;
	double y;
};
struct rectangle_data {
	coordinates less_coordinates;
	coordinates greater_coordinates;
};
} // namespace pdfparser::data_types
