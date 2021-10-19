#pragma once

#include "PDFReader.PDFPage.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_types.hpp"

namespace pdfparser {
class page_tree_parser {
public:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^ get_pages();

public:
	page_tree_parser(ipdfstream&                                     stream,
	                 const object_types::onstream_dictionary_object& root_node);

private:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^
	    get_pages(const object_types::onstream_dictionary_object& page_node,
	              const std::unordered_map<
	                  object_types::onstream_name_object,
	                  object_types::onstream_non_null_direct_object_or_ref>&
	                  inherited_attributes);

private:
	ipdfstream&                                     m_stream;
	const object_types::onstream_dictionary_object& m_root_node;
};
} // namespace pdfparser
