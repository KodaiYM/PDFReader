#pragma once

#include "PDFReader.PDFContents.hpp"
#include "pdfparser.ipdfstream.hpp"

namespace pdfparser {
template <class InputStreamT>
class contents_parser {
public:
	PDFReader::PDFContents ^ get_contents();

public:
	explicit contents_parser(object_parser<InputStreamT>& obj_parser) noexcept;

private:
	object_parser<InputStreamT>& m_object_parser;
};
} // namespace pdfparser

#include "pdfparser.contents_parser.ipp"
