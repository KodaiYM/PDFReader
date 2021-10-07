#pragma once

#include "PDFReader.PDFContents.hpp"
#include "pdfparser.ipdfstream.hpp"

namespace pdfparser {
class contents_parser {
public:
	PDFReader::PDFContents ^ get_contents();

public:
	explicit contents_parser(ipdfstream& content_stream) noexcept;

private:
	ipdfstream& m_content_stream;
};
} // namespace pdfparser
