#pragma once

#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"

namespace pdfparser {

#pragma region stream_parser_template_definitions_on_old_object_cache
template <class... ObjectTypesContainingRef>
object_types::onstream_direct_object object_stream::dereference(
    const object_types::variant_object<ObjectTypesContainingRef...>& object) {
	using namespace object_types;
	static_assert(
	    (... ||
	     std::is_same_v<onstream_indirect_reference, ObjectTypesContainingRef>));

	if (std::holds_alternative<onstream_indirect_reference>(object)) {
		return dereference(static_cast<onstream_indirect_reference>(object));
	} else {
		return object;
	}
}

inline object_types::onstream_direct_object object_stream::dereference(
    const object_types::onstream_indirect_reference& reference) {
	using namespace object_types;
	using namespace xref_types;

	const std::pair<object_t, generation_t> object_id = {
	    reference.object_number, reference.generation_number};

	if (!m_object_cache.contains(object_id)) {
		// TODO: 0 で決め打ちしてしまっているのを直す
		// object_cache は、直接オブジェクトの他に、単なる null_object
		// も入れれるようにしてもいいかも？ それか、null_object
		// じゃなくて、std::nullopt みたいに、無い　のを表すようにするとか
		object_types::onstream_direct_object new_object =
		    object_types::onstream_null_object{0};
		if (auto xref_info = m_xref_table.find(object_id.first, object_id.second);
		    xref_info != m_xref_table.end() &&
		    std::holds_alternative<xref_types::xref_inuse_entry>(*xref_info)) {
			new_object = take_indirect_object(
			    std::get<xref_types::xref_inuse_entry>(*xref_info));
		}

		m_object_cache.add(object_id, std::move(new_object));
	}

	return m_object_cache.get(object_id);
}
#pragma endregion // stream_parser_template_definitions_on_old_object_cache

} // namespace pdfparser
