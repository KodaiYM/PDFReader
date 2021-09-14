#pragma once

#include "pdfparser.object_pool.hpp"
#include "pdfparser.object_types.hpp"

namespace pdfparser::data_types {
struct coordinates {
	double x;
	double y;
};
struct rectangle_data {
	coordinates less_coordinates;
	coordinates greater_coordinates;

	rectangle_data() = default;
	template <typename InputStreamT>
	rectangle_data(object_pool<InputStreamT>&        object_accessor,
	               const object_types::array_object& diagonal_coordinates);
};
} // namespace pdfparser::data_types

// definition of template functions
namespace pdfparser::data_types {
template <typename InputStreamT>
rectangle_data::rectangle_data(
    object_pool<InputStreamT>&        object_accessor,
    const object_types::array_object& diagonal_coordinates) {
	using namespace object_types;
	double x1 = number_to_double(
	    object_accessor.dereference<number_object>(diagonal_coordinates.at(0)));
	double y1 = number_to_double(
	    object_accessor.dereference<number_object>(diagonal_coordinates.at(1)));
	double x2 = number_to_double(
	    object_accessor.dereference<number_object>(diagonal_coordinates.at(2)));
	double y2 = number_to_double(
	    object_accessor.dereference<number_object>(diagonal_coordinates.at(3)));

	if (x1 < x2) {
		less_coordinates.x    = x1;
		greater_coordinates.x = x2;
	} else {
		less_coordinates.x    = x2;
		greater_coordinates.x = x1;
	}

	if (y1 < y2) {
		less_coordinates.y    = y1;
		greater_coordinates.y = y2;
	} else {
		less_coordinates.y    = y2;
		greater_coordinates.y = y1;
	}
}
} // namespace pdfparser::data_types
