#pragma once

#include "PDFParser.object_types.h"
#include "PDFParser.xref_types.h"

#include <unordered_map>
#include <utility>

namespace pdfparser {
template <class InputStreamT>
class stream_parser;

template <class InputStreamT>
class object_pool {
public:
	void add_xref_table(xref_types::xref_table referenced_xref_table);
	object_types::any_direct_object
	    dereference(object_types::indirect_reference reference);
	object_types::any_direct_object
	    dereference(object_types::any_direct_object_or_ref object);
	object_types::any_direct_object
	    get_object(xref_types::object_t     object_number,
	               xref_types::generation_t generation_number);

public:
	explicit object_pool(stream_parser<InputStreamT>& parser) noexcept;

private:
	stream_parser<InputStreamT>& m_parser;
	xref_types::xref_table       m_xref_table;
	std::unordered_map<std::pair<xref_types::object_t, xref_types::generation_t>,
	                   object_types::any_direct_object>
	    m_object_map;
};
} // namespace pdfparser
