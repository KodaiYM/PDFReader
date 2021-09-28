#pragma once

#include "pdfparser.document_reader.hpp"
#include "pdfparser.page_tree_parser.hpp"

using namespace pdfparser;
template <class InputStreamT>
document_reader<InputStreamT>::document_reader(InputStreamT&& stream)
    : m_object_parser(std::move(stream)), m_object_pool(m_object_parser) {
	m_trailer_dictionary = m_object_parser.take_footer(m_object_pool);
}

template <class InputStreamT>
    System::Collections::Generic::List<PDFReader::PDFPage ^> ^
    document_reader<InputStreamT>::get_pages() {
	using namespace object_types;

	auto page_tree_root = m_object_pool.dereference<dictionary_object>(
	    m_object_pool
	        .dereference<dictionary_object>(m_trailer_dictionary.at("Root"))
	        .at("Pages"));
	page_tree_parser this_page_tree_parser(m_object_pool, page_tree_root);
	return this_page_tree_parser.get_pages();
}
