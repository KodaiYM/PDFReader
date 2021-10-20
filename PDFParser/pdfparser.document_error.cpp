#include "pdfparser.document_error.hpp"

#include <cassert>
#include <cstring>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>

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
