#pragma once

#include <stdexcept>

namespace pdfparser {
class object_not_found_error: public std::runtime_error {
public:
	enum error_code {
		boolean_object_not_found,
		integer_object_not_found,
		real_object_not_found,
		string_object_not_found,
		name_object_not_found,
		array_object_not_found,
		dictionary_object_not_found,
		stream_object_not_found,
		null_object_not_found,
		indirect_reference_not_found,
		specified_object_not_found
	};

public:
	explicit object_not_found_error(error_code er_code);
	error_code code() const noexcept;

private:
	const error_code m_error_code;
};
} // namespace pdfparser
