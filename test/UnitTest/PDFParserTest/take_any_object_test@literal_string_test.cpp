#include "PDFParser.h"
#include "take_any_object_test@literal_string_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace take_any_object_test;

void literal_string_test::test_literal_string_sample();
void literal_string_test::test_literal_string_escape_sequence();
void literal_string_test::test_literal_string_invalid_escape_sequence();
void literal_string_test::test_literal_string_escaped_EOL();
void literal_string_test::test_literal_string_EOL_unification();
void literal_string_test::test_literal_string_octal_overflow();
void literal_string_test::test_literal_string_lack_of_right_parenthesis();
