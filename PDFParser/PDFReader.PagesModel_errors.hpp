#pragma once

namespace PDFReader {
public
ref class PDFParserError: public System::Exception {
public:
	PDFParserError(System::String ^ message) : Exception(message) {}
};
} // namespace PDFReader
