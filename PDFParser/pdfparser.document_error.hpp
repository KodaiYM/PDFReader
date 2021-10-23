#pragma once

#include <exception>
#include <ios>
#include <memory>
#include <string>
#include <string_view>

namespace pdfparser {
class document_error: public std::exception {
public:
	[[nodiscard]] const char* what() const noexcept final;

protected:
	explicit document_error(std::string_view message);
	inline virtual ~document_error() noexcept = 0;

private:
	std::shared_ptr<char[]> m_message;
};

// this is only to get RTTI
template <class Derived>
class position_indicatable_error: public document_error {
public:
	[[nodiscard]] std::streampos tell_position() const noexcept;

private:
	position_indicatable_error(std::streampos position, std::string_view message);
	inline virtual ~position_indicatable_error() noexcept = 0;

private:
	std::streampos m_position;

private:
	static std::string generate_message(std::streampos   position,
	                                    std::string_view message);

	friend Derived;
};
} // namespace pdfparser

#include "pdfparser.document_error.ipp"
