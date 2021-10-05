#include "pdfparser.document_error.hpp"
#include "pdfparser.page_parser.hpp"
#include "pdfparser.page_tree_parser.hpp"

using namespace pdfparser;

page_tree_parser::page_tree_parser(
    ipdfstream& stream, const object_types::dictionary_object& root_node)
    : m_stream(stream), m_root_node(root_node) {}

System::Collections::Generic::List<PDFReader::PDFPage ^> ^
    page_tree_parser::get_pages() {
	return get_pages(m_root_node, {});
}

System::Collections::Generic::List<PDFReader::PDFPage ^> ^
    page_tree_parser::get_pages(
        const object_types::dictionary_object& page_node,
        const object_types::dictionary_object& inherited_attributes) {
	using namespace object_types;

	auto type = m_stream.dereference<name_object>(page_node.at("Type"));
	if ("Pages" == type) {
		auto new_inherited_attributes = inherited_attributes;
		for (const auto& entry : page_node) {
			const auto key = entry.first;
			if ("Resources" == key || "MediaBox" == key || "CropBox" == key ||
			    "Rotate" == key) {
				new_inherited_attributes.insert_or_assign(key, entry.second);
			}
		}

		auto pages = gcnew System::Collections::Generic::List<PDFReader::PDFPage ^>;
		const auto& kids = m_stream.dereference<array_object>(page_node.at("Kids"));
		for (const auto& kid : kids) {
			pages->AddRange(get_pages(m_stream.dereference<dictionary_object>(kid),
			                          new_inherited_attributes));
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
		throw gcnew document_error();
	}
}
