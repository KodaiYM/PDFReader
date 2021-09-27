#pragma once

#include "pdfparser.contents_parser.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.pdfcontents_builder.hpp"

#include <vector>

using namespace pdfparser;

template <class InputStreamT>
contents_parser<InputStreamT>::contents_parser(
    object_parser<InputStreamT>& obj_parser) noexcept
    : m_object_parser(obj_parser) {}

template <class InputStreamT>
    PDFReader::PDFContents ^ contents_parser<InputStreamT>::get_contents() {
	using namespace object_types;
	pdfcontents_builder            contents_builder;
	std::vector<any_direct_object> contents_operands;

	return gcnew PDFReader::PDFContents;
}
