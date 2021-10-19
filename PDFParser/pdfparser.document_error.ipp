#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
inline document_error::~document_error() noexcept {}
inline position_indicatable_error::~position_indicatable_error() noexcept {}
} // namespace pdfparser
