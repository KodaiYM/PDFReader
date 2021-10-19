#pragma once

#include "PDFReader.PDFPage.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_types.hpp"

namespace pdfparser {
class page_parser {
public:
	PDFReader::PDFPage ^ get_page();

public:
	page_parser(ipdfstream&                                     stream,
	            const object_types::onstream_dictionary_object& page_object);

private:
	ipdfstream&                                     m_stream;
	const object_types::onstream_dictionary_object& m_page_object;
};
} // namespace pdfparser
