#include "PDFParser.error_types.h"
#include "PDFParser.parser.h"

#include <msclr/marshal_cppstd.h>
#include <sstream>

static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

namespace pdfparser {
#pragma region cpp_cli_parser_tostring
static std::ostream& operator<<(std::ostream&                       os,
                                const object_types::boolean_object& boolean) {
	os << (boolean ? "true" : "false");
	return os;
}
static std::ostream& operator<<(std::ostream&                      os,
                                const object_types::string_object& string) {
	os << '(' << static_cast<std::string>(string) << ')';
	return os;
}
static std::ostream& operator<<(std::ostream&                    os,
                                const object_types::name_object& name) {
	os << '/' << static_cast<std::string>(name);
	return os;
}

template <typename Value>
static std::ostream& operator<<(std::ostream&             os,
                                const std::vector<Value>& array) {
	os << "[";
	for (auto it = array.begin(), end = array.end(); it != end; ++it) {
		if (it != array.begin()) {
			os << ',';
		}
		os << *it;
	}
	os << "]";
	return os;
}
template <typename... Types>
static std::ostream& operator<<(std::ostream&                 os,
                                const std::variant<Types...>& object) {
	std::visit([&](const auto& value) { os << value; }, object);
	return os;
}
template <typename Key, typename Value>
static std::ostream& operator<<(std::ostream&                         os,
                                const std::unordered_map<Key, Value>& map) {
	os << "{\n";
	for (const auto& key_value : map) {
		os << '\t' << key_value.first << ": " << key_value.second << ",\n";
	}
	os << "}";
	return os;
}
static std::ostream& operator<<(std::ostream& os,
                                const object_types::stream_object&) {
	os << "$stream$";
	return os;
}
static std::ostream& operator<<(std::ostream& os,
                                const object_types::null_object&) {
	os << "null";
	return os;
}
static std::ostream&
    operator<<(std::ostream&                           os,
               const object_types::indirect_reference& reference) {
	os << reference.object_number << ' ' << reference.generation_number << " R";
	return os;
}
System::String ^ parser_tostring::get(System::String ^ filename) {
	parser PDFParser{
	    std::ifstream(msclr::interop::marshal_as<std::string>(filename),
	                  std::ios_base::in | std::ios_base::binary)};

	auto              trailer_dictionary = PDFParser.m_trailer_dictionary;
	std::stringstream ss;
	ss << trailer_dictionary;
	return msclr::interop::marshal_as<System::String ^>(ss.str());
}
#pragma endregion // region cpp_cli_parser_tostring
} // namespace pdfparser
