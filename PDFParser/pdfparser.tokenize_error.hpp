#pragma once

#include <stdexcept>

namespace pdfparser {
class tokenize_error: public std::runtime_error {
public:
	enum error_code { promise_token_failed };

public:
	/// <returns>this error code</returns>
	error_code code() const noexcept;

public:
	explicit tokenize_error(error_code er_code);

private:
	const error_code m_error_code;
};
} // namespace pdfparser
