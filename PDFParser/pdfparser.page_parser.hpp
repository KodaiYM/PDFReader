#pragma once

#include "PDFReader.PDFPage.hpp"
#include "pdfparser.object_parser.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.object_types.hpp"

namespace pdfparser {
template <class InputStreamT>
class page_parser {
public:
	PDFReader::PDFPage ^ get_page();

public:
	page_parser(object_parser<InputStreamT>&           obj_parser,
	            const object_types::dictionary_object& page_object);

private:
	object_parser<InputStreamT>&           m_object_parser;
	const object_types::dictionary_object& m_page_object;
};
} // namespace pdfparser
