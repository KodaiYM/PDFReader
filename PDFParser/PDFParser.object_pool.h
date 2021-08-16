#pragma once

#include "PDFParser.object_types.h"
#include "PDFParser.xref_types.h"

#include <cassert>
#include <fstream> // for IntelliSense
#include <unordered_map>
#include <utility>

// hash for object_pool::m_object_map
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
template <class InputStreamT>
class stream_parser;

template <class InputStreamT>
class object_pool {
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

public:
	/// <summary>
	/// Add a new xref table to the internally held m_xref_table.
	/// </summary>
	/// <param name="referenced_xref_table">new xref table</param>
	void add_xref_table(const xref_types::xref_table& referenced_xref_table);

	/// <summary>
	/// Call dereference with the object_number and generation_number of reference
	/// and returns the result.
	/// </summary>
	/// <param name="reference">indirect reference to be referenced</param>
	/// <returns>object with the specified object number and
	/// generation number</returns>
	object_types::any_direct_object
	    dereference(const object_types::indirect_reference& reference);

	/// <summary>
	/// If object is an indirect_reference, call dereference(object) and return
	/// the result. Otherwise, returns object as is.
	/// </summary>
	/// <param name="object">direct object or indirect_reference</param>
	/// <returns>dereferenced object</returns>
	object_types::any_direct_object
	    dereference(const object_types::any_direct_object_or_ref& object);

	/// <summary>
	/// If an object with the specified object number and generation number is
	/// cached, it will be returned.
	/// Otherwise, it refers to the internal xref table and returns null if there
	/// is no entry with the specified object number and generation number or if
	/// it is a free entry.
	/// If it is in use entry, it parses the stream, takes an object,
	/// caches the object and returns it.
	/// </summary>
	/// <param name="object_number">object number of the indirect object</param>
	/// <param name="generation_number">generation number of the indirect
	/// object</param> <returns></returns>
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

// definition of template functions
namespace pdfparser {
template <class InputStreamT>
object_pool<InputStreamT>::object_pool(
    stream_parser<InputStreamT>& parser) noexcept
    : m_parser(parser) {}

template <class InputStreamT>
void object_pool<InputStreamT>::add_xref_table(
    const xref_types::xref_table& referenced_xref_table) {
	m_xref_table = std::move(referenced_xref_table);
}

template <class InputStreamT>
object_types::any_direct_object object_pool<InputStreamT>::dereference(
    const object_types::indirect_reference& reference) {
	return get_object(reference.object_number, reference.generation_number);
}

template <class InputStreamT>
object_types::any_direct_object object_pool<InputStreamT>::dereference(
    const object_types::any_direct_object_or_ref& object) {
	using namespace object_types;

	return std::visit(
	    [this](const auto& concrete_object) -> any_direct_object {
		    using T = std::decay_t<decltype(concrete_object)>;
		    if constexpr (std::is_same_v<indirect_reference, T>) {
			    return dereference(concrete_object);
		    } else {
			    return concrete_object;
		    }
	    },
	    object);
}

template <class InputStreamT>
object_types::any_direct_object object_pool<InputStreamT>::get_object(
    xref_types::object_t     object_number,
    xref_types::generation_t generation_number) {
	if (auto object_it = m_object_map.find({object_number, generation_number});
	    object_it != m_object_map.end()) {
		return object_it->second;
	} else {
		object_types::any_direct_object new_object = object_types::null;
		if (auto xref_info = m_xref_table.find(object_number, generation_number);
		    xref_info != m_xref_table.end() &&
		    std::holds_alternative<xref_types::xref_inuse_entry>(*xref_info)) {
			const auto byte_offset =
			    std::get<xref_types::xref_inuse_entry>(*xref_info).byte_offset;

			const auto prev_pos = m_parser.tell();

			m_parser.seek(byte_offset);
			m_parser.ignore_if_present(
			    ignore_flag::any_whitespace_characters_except_EOL);
			new_object = m_parser.take_indirect_object(*this, object_number,
			                                           generation_number);

			m_parser.seek(prev_pos);
		}

		m_object_map.emplace(std::pair{object_number, generation_number},
		                     new_object);
		return new_object;
	}
}
} // namespace pdfparser
