#include "PDFParser.PagesModel.hpp"
#include "pdfparser.document_parser.hpp"

#include <msclr/marshal_cppstd.h>

namespace PDFParser {
System::Collections::Generic::List<PDFParser::PDFPage ^> ^
    PagesModel::GetPages(System::String ^ filename) {
	pdfparser::document_reader doc_parser{
	    std::ifstream(msclr::interop::marshal_as<std::string>(filename),
	                  std::ios_base::in | std::ios_base::binary)};

	return doc_parser.GetPages();
}
} // namespace PDFParser
