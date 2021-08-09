#include "PDFParser.h"
#include "parser_test.hpp"

#include <fstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

/**
 * helloworld.pdf ‚Ì“Ç‚İ‚İƒeƒXƒg
 */
void parser_test::test_normal_helloworld() {
	using namespace xref_types;

	parser main_parser{std::ifstream(
	    R"(..\..\test\UnitTest\PDFParserTest\data\helloworld.pdf)")};
	/*
	Assert::IsTrue(
	    xref_table{xref_free_entry{0, 65535, 0}, xref_inuse_entry{1, 0, 15},
	               xref_inuse_entry{2, 0, 66}, xref_inuse_entry{3, 0, 223},
	               xref_inuse_entry{4, 0, 125},
	               xref_inuse_entry{5, 0, 329}} == main_parser.get_xref_table());
	*/
}
