#define private public
#include "PDFParser.h"
#undef private

#include <fstream>

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace pdfparser_test {
[DeploymentItem(
    R"(data\helloworld.pdf)")][TestClass] public ref class test_parser {
public:
	[TestMethod] void test_normal_helloworld() {
		using namespace pdfparser::xref_types;

		pdfparser::parser parser{std::ifstream("helloworld.pdf")};
		Assert::AreEqual(440, parser.m_xref_byte_offset);
		Assert::AreEqual(xref_table{xref_free_entry{}}, parser.m_xref_table);
	}
};
} // namespace pdfparser_test
