#include "pdfparser.object_cache.hpp"

using namespace pdfparser;

bool object_cache::contains(
    const std::pair<xref_types::object_t, xref_types::generation_t>& object_id)
    const noexcept {
	return m_object_map.find(object_id) != m_object_map.end();
}
const object_types::any_direct_object& object_cache::get(
    const std::pair<xref_types::object_t, xref_types::generation_t>& object_id)
    const {
	return m_object_map.at(object_id);
}
void object_cache::add(
    std::pair<xref_types::object_t, xref_types::generation_t> object_id,
    object_types::any_direct_object                           object) {
	m_object_map.emplace(std::move(object_id), std::move(object));
}