#pragma once

#include "PDFParser.xref_types.h"

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace pdfparser::object_types {
using boolean_object = bool;
using integer_object = int;
using real_object    = double;
using string_object  = std::string;
using name_object    = std::string;

class array_object;
class dictionary_object;
class stream_object;
class null_object;
struct indirect_reference;

using any_direct_object =
    std::variant<boolean_object, integer_object, real_object, string_object,
                 name_object, array_object, dictionary_object, stream_object,
                 null_object>;
using any_direct_object_or_ref =
    std::variant<boolean_object, integer_object, real_object, string_object,
                 name_object, array_object, dictionary_object, stream_object,
                 null_object, indirect_reference>;
using non_null_direct_object_or_ref =
    std::variant<boolean_object, integer_object, real_object, string_object,
                 name_object, array_object, dictionary_object, stream_object,
                 indirect_reference>;

class array_object: public std::vector<any_direct_object_or_ref> {
private:
	using base = std::vector<any_direct_object_or_ref>;

public:
	using base::base;
};
class dictionary_object
    : public std::unordered_map<name_object,
                                std::variant<non_null_direct_object_or_ref>> {
private:
	using base = std::unordered_map<name_object,
	                                std::variant<non_null_direct_object_or_ref>>;

public:
	using base::base;
};

class stream_object {
public:
	std::string get_decoded_data() const;

public:
	template <class DictionaryT = dictionary_object, class DataT = std::string>
	stream_object(DictionaryT&& stream_dictionary, DataT&& encoded_data);

private:
	dictionary_object m_stream_dictionary;
	std::string       m_encoded_data;
	std::string       m_decoded_data;
};
class null_object {};
struct indirect_reference {
	xref_types::object_t     object_number;
	xref_types::generation_t generation_number;
};

} // namespace pdfparser::object_types

// definition of template functions
namespace pdfparser::object_types {
template <class DictionaryT, class DataT>
stream_object::stream_object(DictionaryT&& stream_dictionary,
                             DataT&&       encoded_data)
    : m_stream_dictionary(std::forward(stream_dictionary)),
      m_encoded_data(std::forward(encoded_data)) {}
} // namespace pdfparser::object_types
