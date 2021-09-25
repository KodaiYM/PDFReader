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
class document_parser;

class type_mismatch: public std::runtime_error {
public:
	type_mismatch() : std::runtime_error("type mismatch") {}
};

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
	void add_xref_table(
	    const xref_types::xref_table& referenced_xref_table) noexcept;

	template <class Variant, class... ObjectTypesContainingRef,
	          std::enable_if_t<is_same_template_v<std::variant, Variant>,
	                           std::nullptr_t> = nullptr>
	Variant dereference(const std::variant<ObjectTypesContainingRef...>& object);

	template <class Variant,
	          std::enable_if_t<is_same_template_v<std::variant, Variant>,
	                           std::nullptr_t> = nullptr>
	Variant dereference(const object_types::indirect_reference& reference);

	/// <summary>
	/// If object is an indirect_reference, call dereference(object) and return
	/// the result. Otherwise, returns object as is.
	/// </summary>
	/// <typeparam name="...ObjectTypes">
	/// Object type pack to be dereferenced</typeparam>
	/// <typeparam name="...ObjectTypesContainingRef">
	/// variant type pack</typeparam>
	/// <param name="object">direct object or indirect_reference</param>
	/// <returns>dereferenced object</returns>
	/// <exception cref="type_mismatch">
	/// thrown when referenced object's type is not one of the specified types.
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <
	    class... ObjectTypes, class... ObjectTypesContainingRef,
	    std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t> = nullptr>
	std::variant<ObjectTypes...>
	    dereference(const std::variant<ObjectTypesContainingRef...>& object);

	/// <summary>get the indirect object referenced by reference.</summary>
	/// <typeparam name="...ObjectTypes">
	/// Object type pack to be dereferenced</typeparam>
	/// <param name="reference">indirect reference to be referenced</param>
	/// <returns>
	/// object with the specified object number and generation number
	/// </returns>
	/// <exception cref="type_mismatch">
	/// thrown when referenced object's type is not one of the specified types.
	/// </exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <class... ObjectTypes, std::enable_if_t<sizeof...(ObjectTypes) >= 2,
	                                                 std::nullptr_t> = nullptr>
	std::variant<ObjectTypes...>
	    dereference(const object_types::indirect_reference& reference);

	/// <summary>
	/// If object is an indirect_reference, call dereference(object) and return
	/// the result. Otherwise, returns object as is.
	/// </summary>
	/// <typeparam name="ObjectType">Object type to be dereferenced</typeparam>
	/// <typeparam name="...ObjectTypesContainingRef">
	/// variant type pack</typeparam>
	/// <param name="object">direct object or indirect_reference</param>
	/// <returns>dereferenced object</returns>
	/// <exception cref="type_mismatch">
	/// thrown when referenced object's type is not ObjectType
	/// </exception>
	/// <exception cref="type_mismatch"></exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <class ObjectType, class... ObjectTypesContainingRef,
	          std::enable_if_t<!is_same_template_v<std::variant, ObjectType>,
	                           std::nullptr_t> = nullptr>
	ObjectType
	    dereference(const std::variant<ObjectTypesContainingRef...>& object);

	/// <summary>get the indirect object referenced by reference.</summary>
	/// <typeparam name="ObjectType">Object type to be dereferenced</typeparam>
	/// <param name="reference">indirect reference to be referenced</param>
	/// <returns>
	/// object with the specified object number and generation number
	/// </returns>
	/// <exception cref="type_mismatch">
	/// thrown when referenced object's type is not ObjectType
	/// </exception>
	/// <exception cref="istream_extended_error"></exception>
	/// <exception cref="std::out_of_range"></exception>
	/// <exception cref="std::overflow_error"></exception>
	/// <exception cref="object_not_found_error"></exception>
	/// <exception cref="parse_error"></exception>
	/// <exception cref="tokenize_error"></exception>
	template <class ObjectType,
	          std::enable_if_t<!is_same_template_v<std::variant, ObjectType>,
	                           std::nullptr_t> = nullptr>
	ObjectType dereference(const object_types::indirect_reference& reference);

public:
	explicit object_pool(document_parser<InputStreamT>& parser) noexcept;

private:
	document_parser<InputStreamT>& m_parser;
	xref_types::xref_table         m_xref_table;
	std::unordered_map<std::pair<xref_types::object_t, xref_types::generation_t>,
	                   object_types::any_direct_object>
	    m_object_map;

private:
	template <class Variant, class... ObjectTypesContainingRef,
	          std::size_t... Seq>
	Variant dereference_Variant_impl(
	    std::index_sequence<Seq...>,
	    const std::variant<ObjectTypesContainingRef...>&);

	template <class Variant, std::size_t... Seq>
	Variant dereference_Variant_impl(
	    std::index_sequence<Seq...>,
	    const object_types::indirect_reference& reference);

	template <class... ObjectTypes>
	std::variant<ObjectTypes...> dereference_variant_fixed(
	    const object_types::indirect_reference& reference);
};
} // namespace pdfparser

// definition of template functions
namespace pdfparser {
template <class InputStreamT>
object_pool<InputStreamT>::object_pool(
    document_parser<InputStreamT>& parser) noexcept
    : m_parser(parser) {}

template <class InputStreamT>
void object_pool<InputStreamT>::add_xref_table(
    const xref_types::xref_table& referenced_xref_table) noexcept {
	m_xref_table = std::move(referenced_xref_table);
}

template <class InputStreamT>
template <
    class Variant, class... ObjectTypesContainingRef,
    std::enable_if_t<is_same_template_v<std::variant, Variant>, std::nullptr_t>>
Variant object_pool<InputStreamT>::dereference(
    const std::variant<ObjectTypesContainingRef...>& object) {
	return dereference_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>(), object);
}

template <class InputStreamT>
template <
    class Variant,
    std::enable_if_t<is_same_template_v<std::variant, Variant>, std::nullptr_t>>
Variant object_pool<InputStreamT>::dereference(
    const object_types::indirect_reference& reference) {
	return dereference_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>(), reference);
}

template <class InputStreamT>
template <class... ObjectTypes, class... ObjectTypesContainingRef,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
std::variant<ObjectTypes...> object_pool<InputStreamT>::dereference(
    const std::variant<ObjectTypesContainingRef...>& object) {
	using namespace object_types;
	static_assert(
	    (... || std::is_same_v<indirect_reference, ObjectTypesContainingRef>));

	return std::visit(
	    [this](const auto& concrete_object) -> std::variant<ObjectTypes...> {
		    using ThisType = std::decay_t<decltype(concrete_object)>;

		    if constexpr (std::is_same_v<indirect_reference, ThisType>) {
			    return dereference_variant_fixed<ObjectTypes...>(concrete_object);
		    } else if constexpr ((... || std::is_same_v<ObjectTypes, ThisType>)) {
			    return concrete_object;
		    } else {
			    throw type_mismatch();
		    }
	    },
	    object);
}

template <class InputStreamT>
template <class... ObjectTypes,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
std::variant<ObjectTypes...> object_pool<InputStreamT>::dereference(
    const object_types::indirect_reference& reference) {
	return dereference_variant_fixed<ObjectTypes...>(reference);
}

template <class InputStreamT>
template <class ObjectType, class... ObjectTypesContainingRef,
          std::enable_if_t<!is_same_template_v<std::variant, ObjectType>,
                           std::nullptr_t>>
ObjectType object_pool<InputStreamT>::dereference(
    const std::variant<ObjectTypesContainingRef...>& object) {
	using namespace object_types;
	static_assert(
	    (... || std::is_same_v<indirect_reference, ObjectTypesContainingRef>));

	if (std::holds_alternative<ObjectType>(object)) {
		return std::get<ObjectType>(object);
	} else if (std::holds_alternative<indirect_reference>(object)) {
		return dereference<ObjectType>(std::get<indirect_reference>(object));
	} else {
		throw type_mismatch();
	}
}

template <class InputStreamT>
template <class ObjectType,
          std::enable_if_t<!is_same_template_v<std::variant, ObjectType>,
                           std::nullptr_t>>
ObjectType object_pool<InputStreamT>::dereference(
    const object_types::indirect_reference& reference) {
	return std::get<ObjectType>(dereference_variant_fixed<ObjectType>(reference));
}

template <class InputStreamT>
template <class Variant, class... ObjectTypesContainingRef, std::size_t... Seq>
Variant object_pool<InputStreamT>::dereference_Variant_impl(
    std::index_sequence<Seq...>,
    const std::variant<ObjectTypesContainingRef...>& object) {
	return dereference<std::variant_alternative_t<Seq, Variant>...>(object);
}

template <class InputStreamT>
template <class Variant, std::size_t... Seq>
Variant object_pool<InputStreamT>::dereference_Variant_impl(
    std::index_sequence<Seq...>,
    const object_types::indirect_reference& reference) {
	return dereference<std::variant_alternative_t<Seq, Variant>...>(reference);
}

template <class InputStreamT>
template <class... ObjectTypes>
std::variant<ObjectTypes...>
    object_pool<InputStreamT>::dereference_variant_fixed(
        const object_types::indirect_reference& reference) {
	decltype(m_object_map)::mapped_type target_object;

	if (auto object_it = m_object_map.find(
	        {reference.object_number, reference.generation_number});
	    object_it != m_object_map.end()) {
		target_object = object_it->second;
	} else {
		decltype(m_object_map)::mapped_type new_object = object_types::null;
		if (auto xref_info = m_xref_table.find(reference.object_number,
		                                       reference.generation_number);
		    xref_info != m_xref_table.end() &&
		    std::holds_alternative<xref_types::xref_inuse_entry>(*xref_info)) {
			new_object = m_parser.take_indirect_object(
			    *this, std::get<xref_types::xref_inuse_entry>(*xref_info));
		}

		m_object_map.emplace(
		    std::pair{reference.object_number, reference.generation_number},
		    new_object);
		target_object = std::move(new_object);
	}

	return std::visit(
	    [](auto concrete_object) -> std::variant<ObjectTypes...> {
		    if constexpr ((... || std::is_same_v<ObjectTypes,
		                                         decltype(concrete_object)>)) {
			    return std::move(concrete_object);
		    } else {
			    throw type_mismatch();
		    }
	    },
	    std::move(target_object));
}
} // namespace pdfparser
