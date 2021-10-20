#include "pdfparser.contents_parser.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.ipdfstringstream.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.page_parser.hpp"
#include "pdfparser.pdfpage_builder.hpp"

#include <ios>
#include <sstream>

using namespace pdfparser;
using namespace pdfparser::object_types;

page_parser::page_parser(ipdfstream&                       stream,
                         const onstream_dictionary_object& page_object)
    : m_stream(stream), m_page_object(page_object) {}

PDFReader::PDFPage ^ page_parser::get_page() {
	pdfpage_builder this_page_builder;

	// Get Mediabox
	onstream_array_object media_box_array =
	    m_stream.dereference(m_page_object.at("MediaBox").get());
	auto media_box = rectangle_data(
	    {number_object{m_stream.dereference(media_box_array.at(0))},
	     number_object{m_stream.dereference(media_box_array.at(1))}},
	    {number_object{m_stream.dereference(media_box_array.at(2))},
	     number_object{m_stream.dereference(media_box_array.at(3))}});

	this_page_builder.set_media_box(std::move(media_box));

	// Get Contents
	onstream_stream_object content_stream_object =
	    m_stream.dereference(m_page_object.at("Contents").get());
	ipdfstringstream content_stream(content_stream_object.get_decoded_data());
	contents_parser  this_contents_parser(content_stream);
	this_page_builder.set_contents(this_contents_parser.get_contents());

	return this_page_builder.to_pdfpage();
}
