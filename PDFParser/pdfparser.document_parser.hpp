#pragma once

#include "PDFParser.PDFPage.hpp"
#include "pdfparser.data_types.hpp"
#include "pdfparser.document_error.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.stream_parser.hpp"
#include "pdfparser.xref_types.hpp"

#include <fstream> // for IntelliSense
#include <ios>
#include <memory>

namespace pdfparser {
template <class InputStreamT>
class document_reader final {
public:
	System::Collections::Generic::List<PDFParser::PDFPage ^> ^ GetPages();

public:
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

	/// <summary>move the argument stream and construct</summary>
	/// <param name="stream">input stream inheriting from std::istream</param>
	/// <exception cref="std::ios_base::failure">
	/// thrown when stream.rdbuf() == nullptr
	/// </exception>
	/// <exception>
	/// move constructor of InputStreamT may throw an exception.
	/// </exception>
	explicit document_reader(InputStreamT&& stream);

	/// <summary>prohibit to move from const rvalue/summary>
	document_reader(const InputStreamT&&) = delete;

	/// <summary>prohibit to copy from lvalue</summary>
	document_reader(InputStreamT&) = delete;

private:
	System::Collections::Generic::List<PDFParser::PDFPage ^> ^
	    GetPages(const object_types::dictionary_object& page_node,
	             const object_types::dictionary_object& inherited_attributes);

private:
	document_parser<InputStreamT>   m_stream_parser;
	object_types::dictionary_object m_trailer_dictionary;
	object_pool<InputStreamT>       m_object_pool;

	// to GUI (by C++/CLI)
	friend ref class PagesModel;
};
} // namespace pdfparser

// definition of template functions
namespace pdfparser {
template <class InputStreamT>
document_reader<InputStreamT>::document_reader(InputStreamT&& stream)
    : m_stream_parser(std::move(stream)), m_object_pool(m_stream_parser) {
	m_trailer_dictionary = m_stream_parser.take_footer(m_object_pool);
}

template <class InputStreamT>
    System::Collections::Generic::List<PDFParser::PDFPage ^> ^
    document_reader<InputStreamT>::GetPages() {
	using namespace object_types;

	auto page_tree_root = m_object_pool.dereference<dictionary_object>(
	    m_object_pool
	        .dereference<dictionary_object>(m_trailer_dictionary.at("Root"))
	        .at("Pages"));
	return GetPages(page_tree_root, {});
}
template <class InputStreamT>
    System::Collections::Generic::List<PDFParser::PDFPage ^> ^
    document_reader<InputStreamT>::GetPages(
        const object_types::dictionary_object& page_node,
        const object_types::dictionary_object& inherited_attributes) {
	using namespace object_types;
	using namespace data_types;

	auto type = m_object_pool.dereference<name_object>(page_node.at("Type"));
	if ("Pages" == type) {
		auto new_inherited_attributes = inherited_attributes;
		for (const auto& entry : page_node) {
			const auto key = entry.first;
			if ("Resources" == key || "MediaBox" == key || "CropBox" == key ||
			    "Rotate" == key) {
				new_inherited_attributes.insert_or_assign(key, entry.second);
			}
		}

		auto pages = gcnew System::Collections::Generic::List<PDFParser::PDFPage ^>;
		const auto&        kids =
		    m_object_pool.dereference<array_object>(page_node.at("Kids"));
		for (const auto& kid : kids) {
			pages->AddRange(
			    GetPages(m_object_pool.dereference<dictionary_object>(kid),
			             new_inherited_attributes));
		}
		return pages;
	} else if ("Page" == type) {
		auto complete_page_node = page_node;
		complete_page_node.insert(inherited_attributes.begin(),
		                          inherited_attributes.end());

		auto media_box =
		    rectangle_data(m_object_pool, m_object_pool.dereference<array_object>(
		                                      complete_page_node.at("MediaBox")));

		PDFParser::PDFPage ^ this_page = gcnew PDFParser::PDFPage;
		this_page->Width =
		    media_box.greater_coordinates.x - media_box.less_coordinates.x + 1;
		this_page->Height =
		    media_box.greater_coordinates.y - media_box.less_coordinates.y + 1;

		auto      only_this_page_list =
		    gcnew System::Collections::Generic::List<PDFParser::PDFPage ^>;
		only_this_page_list->Add(this_page);

		return only_this_page_list;
	} else {
		throw gcnew document_error();
	}
}
} // namespace pdfparser
