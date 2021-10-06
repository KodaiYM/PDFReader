#pragma once

#include "pdfparser.object_types.hpp"
#include "pdfparser.xref_types.hpp"
#include "type_traits_extended.hpp"

#include <cassert>
#include <fstream> // for IntelliSense
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <variant>

// hash for object_cache::m_object_map
namespace std {
template <typename Key, typename Value>
struct hash<std::pair<Key, Value>> {
	constexpr std::size_t
	    operator()(const std::pair<Key, Value>& pair) const noexcept {
		auto key_hash   = std::hash<Key>{}(pair.first);
		auto value_hash = std::hash<Value>{}(pair.second);

		// NOTE: this code is from boost::hash_combine
		std::size_t seed = 0;
		seed ^= key_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= value_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};
} // namespace std

namespace pdfparser {
class object_cache {
public:
	bool contains(const std::pair<xref_types::object_t, xref_types::generation_t>&
	                  object_id) const noexcept;
	const object_types::any_direct_object&
	     get(const std::pair<xref_types::object_t, xref_types::generation_t>&
	             object_id) const;
	void add(std::pair<xref_types::object_t, xref_types::generation_t> object_id,
	         object_types::any_direct_object                           object);

private:
	std::unordered_map<std::pair<xref_types::object_t, xref_types::generation_t>,
	                   object_types::any_direct_object>
	    m_object_map;
};
} // namespace pdfparser
