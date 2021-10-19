#include "pdfparser.document_error.hpp"
#include "pdfparser.page_parser.hpp"
#include "pdfparser.page_tree_parser.hpp"
#include "pdfparser.page_tree_parser_errors.hpp"

using namespace pdfparser;

page_tree_parser::page_tree_parser(
    ipdfstream&                                     stream,
    const object_types::onstream_dictionary_object& root_node)
    : m_stream(stream), m_root_node(root_node) {}

System::Collections::Generic::List<PDFReader::PDFPage ^> ^
    page_tree_parser::get_pages() {
	return get_pages(m_root_node, {});
}

System::Collections::Generic::List<PDFReader::PDFPage ^> ^
    page_tree_parser::get_pages(
        const object_types::onstream_dictionary_object& page_node,
        const std::unordered_map<
            object_types::onstream_name_object,
            object_types::onstream_non_null_direct_object_or_ref>&
            inherited_attributes) {
	using namespace object_types;

	onstream_name_object type = m_stream.dereference(page_node.at("Type").get());
	if ("Pages" == type) {
		auto new_inherited_attributes = inherited_attributes;
		for (const auto& entry : page_node) {
			const auto key = entry.first;
			if ("Resources" == key || "MediaBox" == key || "CropBox" == key ||
			    "Rotate" == key) {
				new_inherited_attributes.insert_or_assign(key, entry.second.get());
			}
		}

		auto pages = gcnew System::Collections::Generic::List<PDFReader::PDFPage ^>;
		const onstream_array_object& kids =
		    m_stream.dereference(page_node.at("Kids").get());
		for (const auto& kid : kids) {
			pages->AddRange(
			    get_pages(m_stream.dereference(kid), new_inherited_attributes));
		}
		return pages;
	} else if ("Page" == type) {
		auto complete_page_node = page_node;
		complete_page_node.insert(inherited_attributes.begin(),
		                          inherited_attributes.end());

		page_parser this_page_parser(m_stream, complete_page_node);
		auto        only_this_page_list =
		    gcnew   System::Collections::Generic::List<PDFReader::PDFPage ^>;
		only_this_page_list->Add(this_page_parser.get_page());

		return only_this_page_list;
	} else {
		throw non_page_node_detected(type.position());
	}
}
