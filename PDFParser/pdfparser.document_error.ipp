#pragma once

#include "pdfparser.document_error.hpp"

#include <sstream>
#include <string>
#include <typeinfo>

namespace pdfparser {
inline document_error::~document_error() noexcept {}

#pragma region position_indicatable_error
template <class Derived>
position_indicatable_error<Derived>::position_indicatable_error(
    std::streampos position, std::string_view message)
    : m_position(position),
      document_error(generate_message(position, message)) {
	static_assert(std::is_base_of_v<position_indicatable_error, Derived>);
}

template <class Derived>
inline position_indicatable_error<
    Derived>::~position_indicatable_error() noexcept {}

template <class Derived>
std::streampos
    position_indicatable_error<Derived>::tell_position() const noexcept {
	return m_position;
}

template <class Derived>
std::string position_indicatable_error<Derived>::generate_message(
    std::streampos position, std::string_view message) {
	std::stringstream message_stream;
	message_stream << "at " << position << " byte\n";

	std::stringstream content;
	content << message;
	content << "\n\n" << '(' << typeid(Derived).name() << ')';

	for (std::string line; std::getline(content, line);) {
		message_stream << "  " << line << '\n';
	}

	return message_stream.str();
}

#pragma endregion // region position_indicatable_error
} // namespace pdfparser
