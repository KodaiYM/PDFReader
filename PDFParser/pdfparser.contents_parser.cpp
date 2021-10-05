#include "pdfparser.contents_parser.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.pdfcontents_builder.hpp"

#include <vector>

using namespace pdfparser;

contents_parser::contents_parser(ipdfstream& content_stream) noexcept
    : m_content_stream(content_stream) {}

PDFReader::PDFContents ^ contents_parser::get_contents() {
	using namespace object_types;
	pdfcontents_builder            contents_builder;
	std::vector<any_direct_object> contents_operands;

	return gcnew PDFReader::PDFContents;
}
