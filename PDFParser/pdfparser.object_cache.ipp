#pragma once

#include "pdfparser.object_cache.hpp"

// hash for object_cache::m_object_map
namespace std {
template <typename Key, typename Value>
constexpr std::size_t hash<std::pair<Key, Value>>::operator()(
    const std::pair<Key, Value>& pair) const noexcept {
	auto key_hash   = std::hash<Key>{}(pair.first);
	auto value_hash = std::hash<Value>{}(pair.second);

	// NOTE: this code is from boost::hash_combine
	std::size_t seed = 0;
	seed ^= key_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	seed ^= value_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}
} // namespace std

namespace pdfparser {
#pragma region object_cache

inline bool object_cache::contains(
    const std::pair<xref_types::object_t, xref_types::generation_t>& object_id)
    const noexcept {
	return m_object_map.find(object_id) != m_object_map.end();
}

inline const object_types::onstream_direct_object& object_cache::get(
    const std::pair<xref_types::object_t, xref_types::generation_t>& object_id)
    const {
	return m_object_map.at(object_id);
}

inline void object_cache::add(
    std::pair<xref_types::object_t, xref_types::generation_t> object_id,
    object_types::onstream_direct_object                      object) {
	m_object_map.emplace(std::move(object_id), std::move(object));
}

#pragma endregion // region object_cache
} // namespace pdfparser
