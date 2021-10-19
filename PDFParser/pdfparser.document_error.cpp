#include "pdfparser.document_error.hpp"

#include <cassert>
#include <cstring>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

using namespace pdfparser;

#pragma region document_error
document_error::document_error(std::string_view message)
    : m_message(std::make_unique<char[]>(message.length() + 1)) {
	// check that message.length() + 1 does not overflow
	assert(message.length() <=
	       std::numeric_limits<decltype(message.length())>::max() - 1);

	std::memcpy(m_message.get(), message.data(), message.length());
	m_message[message.length()] = '\0';
}

const char* document_error::what() const noexcept {
	return m_message.get();
}
#pragma endregion // document_error

#pragma region position_indicatable_error
position_indicatable_error::position_indicatable_error(std::streampos position,
                                                       std::string_view message)
    : m_position(position), document_error(generate_message(
                                position, message, typeid(*this).name())) {}

std::streampos position_indicatable_error::tell_position() const noexcept {
	return m_position;
}

std::string position_indicatable_error::generate_message(
    std::streampos position, std::string_view message,
    std::string_view dynamic_type_name) {
	std::stringstream message_stream;
	message_stream << "at " << position << " byte\n";

	std::stringstream content;
	content << message;
	content << "\n\n" << '(' << dynamic_type_name << ')';

	for (std::string line; std::getline(content, line);) {
		message_stream << "  " << line << '\n';
	}

	return message_stream.str();
}

#pragma endregion // position_indicatable_error
