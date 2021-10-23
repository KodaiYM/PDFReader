#include "pdfparser.document_reader.hpp"
#include "pdfparser.page_tree_parser.hpp"

using namespace pdfparser;
document_reader::document_reader(ipdfstream& stream)
    : m_stream(stream), m_trailer_dictionary(m_stream.take_footer()) {}

System::Collections::Generic::List<PDFReader::PDFPage ^> ^
    document_reader::get_pages() {
	using namespace object_types;

	onstream_dictionary_object page_tree_root = m_stream.dereference(
	    static_cast<onstream_dictionary_object>(
	        m_stream.dereference(m_trailer_dictionary.at("Root").get()))
	        .at("Pages")
	        .get());
	page_tree_parser this_page_tree_parser(m_stream, page_tree_root);
	return this_page_tree_parser.get_pages();
}
