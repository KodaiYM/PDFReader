#include "document_parser_test.hpp"
#include "pdfparser.document_parser.hpp"

#include <filesystem>
#include <fstream>

using namespace pdfparser;
using namespace pdfparser_test;

/**
 * helloworld.pdf ÇÃì«Ç›çûÇ›ÉeÉXÉg
 */
void parser_test::test_normal_helloworld() {
	using namespace xref_types;
	namespace fs = std::filesystem;

	fs::path helloworld_path =
	    R"(..\..\test\UnitTest\PDFParserTest\data\helloworld.pdf)";

	Assert::IsTrue(fs::exists(helloworld_path));

	// check if no-throw
	parser main_parser{std::ifstream(helloworld_path,
	                                 std::ios_base::in | std::ios_base::binary)};
	/*
	Assert::IsTrue(
	    xref_table{xref_free_entry{0, 65535, 0}, xref_inuse_entry{1, 0, 15},
	               xref_inuse_entry{2, 0, 66}, xref_inuse_entry{3, 0, 223},
	               xref_inuse_entry{4, 0, 125},
	               xref_inuse_entry{5, 0, 329}} == main_parser.get_xref_table());
	*/
}
