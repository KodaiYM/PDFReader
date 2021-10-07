#pragma once

#include "PDFReader.PDFPage.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_types.hpp"

namespace pdfparser {
class page_tree_parser {
public:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^ get_pages();

public:
	page_tree_parser(ipdfstream&                            stream,
	                 const object_types::dictionary_object& root_node);

private:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^
	    get_pages(const object_types::dictionary_object& page_node,
	              const object_types::dictionary_object& inherited_attributes);

private:
	ipdfstream&                            m_stream;
	const object_types::dictionary_object& m_root_node;
};
} // namespace pdfparser
