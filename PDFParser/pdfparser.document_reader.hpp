#pragma once

#include "PDFReader.PDFPage.hpp"
#include "pdfparser.document_error.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.space.hpp"
#include "pdfparser.xref_types.hpp"

#include <ios>
#include <memory>

namespace pdfparser {
class document_reader final {
public:
	System::Collections::Generic::List<PDFReader::PDFPage ^> ^ get_pages();

public:
	/// <summary>
	/// construct document_reader with pdfstream
	/// </summary>
	/// <param name="stream">PDF file stream or string stream</param>
	explicit document_reader(ipdfstream& stream);

private:
	ipdfstream&                              m_stream;
	object_types::onstream_dictionary_object m_trailer_dictionary;
};
} // namespace pdfparser
