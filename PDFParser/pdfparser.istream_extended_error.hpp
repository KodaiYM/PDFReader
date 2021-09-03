#pragma once

#include <stdexcept>

namespace pdfparser {
class istream_extended_error: public std::runtime_error {
public:
	enum error_code {
		failed_to_seek,
		failed_to_seek_forward_head_of_line,
		promise_failed
	};

public:
	error_code code() const noexcept;

public:
	explicit istream_extended_error(error_code er_code);

private:
	const error_code m_error_code;
};
} // namespace pdfparser
