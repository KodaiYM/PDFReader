#pragma once

#include "pdfparser.object_types.hpp"
#include "pdfparser.xref_types.hpp"

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
	    operator()(const std::pair<Key, Value>& pair) const noexcept;
};
} // namespace std

namespace pdfparser {
class object_cache {
public:
	inline bool
	    contains(const std::pair<xref_types::object_t, xref_types::generation_t>&
	                 object_id) const noexcept;
	inline const object_types::onstream_direct_object&
	    get(const std::pair<xref_types::object_t, xref_types::generation_t>&
	            object_id) const;
	inline void
	    add(std::pair<xref_types::object_t, xref_types::generation_t> object_id,
	        object_types::onstream_direct_object                      object);

private:
	std::unordered_map<std::pair<xref_types::object_t, xref_types::generation_t>,
	                   object_types::onstream_direct_object>
	    m_object_map;
};
} // namespace pdfparser

#include "pdfparser.object_cache.ipp"
