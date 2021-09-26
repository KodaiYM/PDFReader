#pragma once

#pragma warning(push)
#pragma warning(disable : 4819; disable : 4127)
#include "Eigen/Core"
#pragma warning(pop)

#include <array>

namespace pdfparser {
struct coordinates {
	double x = 0.0;
	double y = 0.0;
};
struct rectangle_data {
	coordinates less_coordinates;
	coordinates greater_coordinates;

	rectangle_data() = default;
	rectangle_data(const coordinates& diagonal_coordinate1,
	               const coordinates& diagonal_coordinate2);
};
struct AffineMatrix: Eigen::Matrix3d {
public:
	static AffineMatrix Identity();

public:
	AffineMatrix(std::array<double, 6> coefficients = {1, 0, 0, 1, 0, 0});
};
} // namespace pdfparser
