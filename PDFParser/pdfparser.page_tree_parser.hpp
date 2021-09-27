#pragma once

#include "PDFReader.PDFPage.hpp"
#include "pdfparser.object_parser.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.object_types.hpp"

namespace pdfparser {
template <class InputStremT>
class page_tree_parser {
public:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^ get_pages();

public:
	page_tree_parser(object_parser<InputStremT>&            obj_parser,
	                 object_pool<InputStremT>&              obj_pool,
	                 const object_types::dictionary_object& root_node);

private:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^
	    get_pages(const object_types::dictionary_object& page_node,
	              const object_types::dictionary_object& inherited_attributes);

private:
	object_parser<InputStremT>&            m_object_parser;
	object_pool<InputStremT>&              m_object_pool;
	const object_types::dictionary_object& m_root_node;
};
} // namespace pdfparser

#include "pdfparser.page_tree_parser.ipp"
