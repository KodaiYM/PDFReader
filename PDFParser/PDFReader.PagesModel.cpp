#include "PDFReader.PagesModel.hpp"
#include "PDFReader.PagesModel_errors.hpp"
#include "pdfparser.document_reader.hpp"
#include "pdfparser.ipdffstream.hpp"

#define NOMINMAX
#include <msclr/marshal_cppstd.h>

namespace PDFReader {
System::Collections::Generic::List<PDFReader::PDFPage ^> ^
    PagesModel::GetPages(System::String ^ filename) try {
	pdfparser::ipdfstream&& stream =
	    pdfparser::ipdffstream(msclr::interop::marshal_as<std::string>(filename));
	pdfparser::document_reader doc_parser(stream);

	return doc_parser.get_pages();
} catch (std::exception& e) {
	throw gcnew PDFParserError(gcnew System::String(e.what()));
}
} // namespace PDFReader
