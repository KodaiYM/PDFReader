#pragma once

#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_types.hpp"
#include "pdfparser.tokenizer.hpp"

namespace pdfparser {
class object_stream: public tokenizer {
public:
	/// <summary>
	/// Take any direct object or indirect reference
	/// </summary>
	/// <returns>taken object</returns>
	object_types::onstream_direct_object_or_ref take_object();

	object_types::onstream_boolean_object    take_boolean_object();
	object_types::onstream_integer_object    take_integer_object();
	object_types::onstream_real_object       take_real_object();
	object_types::onstream_string_object     take_string_object();
	object_types::onstream_name_object       take_name_object();
	object_types::onstream_array_object      take_array_object();
	object_types::onstream_dictionary_object take_dictionary_object();
	object_types::onstream_stream_object     take_stream_object();

	/// <summary>
	/// take stream object (optimized version for successive calls to
	/// take_stream_object and take_dictionary_object)
	/// </summary>
	object_types::onstream_stream_object take_stream_object(
	    object_types::onstream_dictionary_object stream_dictionary);

	object_types::onstream_null_object        take_null_object();
	object_types::onstream_indirect_reference take_indirect_reference();

	/// <summary>
	/// take indirect object
	/// </summary>
	/// <param name="object_info">xref in-use information to be taken</param>
	/// <returns>the direct object to which this indirect object refers</returns>
	object_types::onstream_direct_object
	    take_indirect_object(const xref_types::xref_inuse_entry& object_info);

public:
	/// <summary>
	/// Add a new xref table to the internally held m_xref_table.
	/// </summary>
	/// <param name="referenced_xref_table">new xref table</param>
	void add_xref_table(const xref_types::xref_table& referenced_xref_table);

	template <class... ObjectTypesContainingRef>
	object_types::onstream_direct_object dereference(
	    const object_types::variant_object<ObjectTypesContainingRef...>& object);

	inline object_types::onstream_direct_object
	    dereference(const object_types::onstream_indirect_reference& reference);

private:
	using base = tokenizer;

public:
	/// <summary>constructor</summary>
	using base::base;

private:
	object_cache           m_object_cache;
	xref_types::xref_table m_xref_table;
};
} // namespace pdfparser

#include "pdfparser.object_stream.ipp"
