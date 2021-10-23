#include "testtool.h"

namespace document_reader_test {
[TestClass] public ref class get_pages_test {
public:
	[TestMethod] void test_GetPages_helloworld();
};
} // namespace document_reader_test

#include "pdfparser.document_reader.hpp"
#include "pdfparser.ipdffstream.hpp"

#include <filesystem>

using namespace pdfparser;
using namespace document_reader_test;
namespace fs = std::filesystem;

void get_pages_test::test_GetPages_helloworld() {
	fs::path helloworld_path =
	    R"(..\..\test\UnitTest\PDFParserTest\data\helloworld.pdf)";

	Assert::IsTrue(fs::exists(helloworld_path));

	ipdffstream stream(helloworld_path);
	// check if no-throw
	document_reader main_parser(stream);

	System::Collections::Generic::List<PDFReader::PDFPage ^> expected_pages;
	PDFReader::PDFPage ^ page = gcnew                        PDFReader::PDFPage;
	page->Width               = 596;
	page->Height              = 843;
	expected_pages.Add(page);

	CollectionAssert::AreEqual(% expected_pages, main_parser.get_pages());
}
