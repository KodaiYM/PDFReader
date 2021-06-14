#include "PDFParser.error_types.cpp"
#include "PDFParser.parser.cpp"
#include "PDFParser.xref_types.cpp"

#include <fstream>

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace pdfparser_test {
[DeploymentItem(
    R"(data\helloworld.pdf)")][TestClass] public ref class parser_test {
public:
	[TestMethod] void test_normal_helloworld() {
		using namespace pdfparser::xref_types;

		pdfparser::parser parser{"helloworld.pdf"};
		Assert::IsTrue(
		    xref_table{xref_free_entry{0, 65535, 0}, xref_inuse_entry{1, 0, 15},
		               xref_inuse_entry{2, 0, 66}, xref_inuse_entry{3, 0, 223},
		               xref_inuse_entry{4, 0, 125},
		               xref_inuse_entry{5, 0, 329}} == parser.get_xref_table());
	}
};
} // namespace pdfparser_test
