#include "pdfparser.space.hpp"

using namespace pdfparser;
rectangle_data::rectangle_data(const coordinates& diagonal_coordinate1,
                               const coordinates& diagonal_coordinate2) {
	const auto& x1 = diagonal_coordinate1.x;
	const auto& y1 = diagonal_coordinate1.y;
	const auto& x2 = diagonal_coordinate2.x;
	const auto& y2 = diagonal_coordinate2.y;

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

AffineMatrix::AffineMatrix(std::array<double, 6> coefficients)
    : Eigen::Matrix3d({{coefficients[0], coefficients[1], 0},
                       {coefficients[2], coefficients[3], 0},
                       {coefficients[4], coefficients[5], 1}}) {}

AffineMatrix AffineMatrix::Identity() {
	return AffineMatrix({1, 0, 0, 1, 0, 0});
}
