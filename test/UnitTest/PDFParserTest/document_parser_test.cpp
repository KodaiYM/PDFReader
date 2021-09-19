#include "document_parser_test.hpp"
#include "pdfparser.document_parser.hpp"

#include <filesystem>
#include <fstream>

using namespace pdfparser;
using namespace pdfparser_test;

void document_parser_test::test_GetPages_helloworld() {
	using namespace xref_types;
	namespace fs = std::filesystem;

	fs::path helloworld_path =
	    R"(..\..\test\UnitTest\PDFParserTest\data\helloworld.pdf)";

	Assert::IsTrue(fs::exists(helloworld_path));

	// check if no-throw
	document_parser main_parser{std::ifstream(
	    helloworld_path, std::ios_base::in | std::ios_base::binary)};

	System::Collections::Generic::List<PDFParser::PDFPage ^> expected_pages;
	PDFParser::PDFPage ^ page = gcnew                        PDFParser::PDFPage;
	page->Width               = 596;
	page->Height              = 843;
	expected_pages.Add(page);

	CollectionAssert::AreEqual(% expected_pages, main_parser.GetPages());
}
