#pragma once

#include "pdfparser.object_stream.hpp"
#include "pdfparser.parse_error.hpp"

using namespace pdfparser;

// definition of template member functions of object_stream
#pragma region stream_parser_template_definitions
template <class Variant, std::size_t... Seq>
Variant object_stream::take_object_Variant_impl(std::index_sequence<Seq...>) {
	return take_object<std::variant_alternative_t<Seq, Variant>...>();
}

template <class Variant>
Variant object_stream::take_object() {
	return take_object_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>());
}

template <class... ObjectTypes,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
std::variant<ObjectTypes...> object_stream::take_object() {
	using namespace object_types;

	constexpr bool contains_boolean =
	    (... || std::is_same_v<boolean_object, ObjectTypes>);
	constexpr bool contains_real =
	    (... || std::is_same_v<real_object, ObjectTypes>);
	constexpr bool contains_string =
	    (... || std::is_same_v<string_object, ObjectTypes>);
	constexpr bool contains_name =
	    (... || std::is_same_v<name_object, ObjectTypes>);
	constexpr bool contains_array =
	    (... || std::is_same_v<array_object, ObjectTypes>);
	constexpr bool contains_null =
	    (... || std::is_same_v<null_object, ObjectTypes>);
	constexpr bool contains_stream =
	    (... || std::is_same_v<stream_object, ObjectTypes>);
	constexpr bool contains_indirect_reference =
	    (... || std::is_same_v<indirect_reference, ObjectTypes>);
	constexpr bool contains_integer =
	    (... || std::is_same_v<integer_object, ObjectTypes>);
	constexpr bool contains_dictionary =
	    (... || std::is_same_v<dictionary_object, ObjectTypes>);

	static_assert(sizeof...(ObjectTypes) ==
	              contains_boolean + contains_real + contains_string +
	                  contains_name + contains_array + contains_null +
	                  contains_stream + contains_indirect_reference +
	                  contains_integer + contains_dictionary);

	if constexpr (contains_boolean) {
		auto before_take_object_pos = tell();
		try {
			return take_boolean_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::boolean_object_not_found == obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_real) {
		auto before_take_object_pos = tell();
		try {
			return take_real_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::real_object_not_found == obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_string) {
		auto before_take_object_pos = tell();
		try {
			return take_string_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::string_object_not_found == obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_name) {
		auto before_take_object_pos = tell();
		try {
			return take_name_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::name_object_not_found == obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_array) {
		auto before_take_object_pos = tell();
		try {
			return take_array_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::array_object_not_found == obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_null) {
		auto before_take_object_pos = tell();
		try {
			return take_null_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::null_object_not_found == obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_stream || contains_dictionary) {
		auto                             before_take_dictionary_pos = tell();
		std::optional<dictionary_object> dictionary;
		try {
			dictionary = take_dictionary_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::dictionary_object_not_found == obj_e.code()) {
				seek(before_take_dictionary_pos);
			} else {
				throw;
			}
		}

		if (dictionary.has_value()) {
			if constexpr (contains_stream) {
				auto before_take_stream_pos = tell();
				try {
					if constexpr (contains_dictionary) {
						return take_stream_object(dictionary.value());
					} else {
						return take_stream_object(std::move(dictionary).value());
					}
				} catch (const object_not_found_error& obj_e) {
					if (object_not_found_error::stream_object_not_found == obj_e.code()) {
						if constexpr (contains_dictionary) {
							seek(before_take_stream_pos);
							return std::move(dictionary).value();
						} else {
							seek(before_take_dictionary_pos);
						}
					} else {
						throw;
					}
				}
			} else {
				return std::move(dictionary).value();
			}
		}
	}
	if constexpr (contains_indirect_reference) {
		auto before_take_object_pos = tell();
		try {
			return take_indirect_reference();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::indirect_reference_not_found ==
			    obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}
	if constexpr (contains_integer) {
		auto before_take_object_pos = tell();
		try {
			return take_integer_object();
		} catch (const object_not_found_error& obj_e) {
			if (object_not_found_error::integer_object_not_found == obj_e.code()) {
				seek(before_take_object_pos);
			} else {
				throw;
			}
		}
	}

	throw object_not_found_error(
	    object_not_found_error::specified_object_not_found);
}
#pragma endregion // region stream_parser_template_definitions

// definition of template member functions from old object_cache
#pragma region stream_parser_template_definitions_on_old_object_cache
template <
    class Variant, class... ObjectTypesContainingRef,
    std::enable_if_t<is_same_template_v<std::variant, Variant>, std::nullptr_t>>
Variant object_stream::dereference(
    const std::variant<ObjectTypesContainingRef...>& object) {
	return dereference_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>(), object);
}

template <
    class Variant,
    std::enable_if_t<is_same_template_v<std::variant, Variant>, std::nullptr_t>>
Variant object_stream::dereference(
    const object_types::indirect_reference& reference) {
	return dereference_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>(), reference);
}

template <class... ObjectTypes, class... ObjectTypesContainingRef,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
std::variant<ObjectTypes...> object_stream::dereference(
    const std::variant<ObjectTypesContainingRef...>& object) {
	using namespace object_types;
	static_assert(
	    (... || std::is_same_v<indirect_reference, ObjectTypesContainingRef>));

	return std::visit(
	    [this](const auto& concrete_object) -> std::variant<ObjectTypes...> {
		    using ThisType = std::decay_t<decltype(concrete_object)>;

		    if constexpr (std::is_same_v<indirect_reference, ThisType>) {
			    return dereference_Variant_fixed<ObjectTypes...>(concrete_object);
		    } else if constexpr ((... || std::is_same_v<ObjectTypes, ThisType>)) {
			    return concrete_object;
		    } else {
			    throw type_mismatch();
		    }
	    },
	    object);
}

template <class... ObjectTypes,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
std::variant<ObjectTypes...> object_stream::dereference(
    const object_types::indirect_reference& reference) {
	return dereference_Variant_fixed<ObjectTypes...>(reference);
}

template <class ObjectType, class... ObjectTypesContainingRef,
          std::enable_if_t<!is_same_template_v<std::variant, ObjectType>,
                           std::nullptr_t>>
ObjectType object_stream::dereference(
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

template <class ObjectType,
          std::enable_if_t<!is_same_template_v<std::variant, ObjectType>,
                           std::nullptr_t>>
ObjectType object_stream::dereference(
    const object_types::indirect_reference& reference) {
	return std::get<ObjectType>(dereference_Variant_fixed<ObjectType>(reference));
}

template <class Variant, class... ObjectTypesContainingRef, std::size_t... Seq>
Variant object_stream::dereference_Variant_impl(
    std::index_sequence<Seq...>,
    const std::variant<ObjectTypesContainingRef...>& object) {
	return dereference<std::variant_alternative_t<Seq, Variant>...>(object);
}

template <class Variant, std::size_t... Seq>
Variant object_stream::dereference_Variant_impl(
    std::index_sequence<Seq...>,
    const object_types::indirect_reference& reference) {
	return dereference_Variant_fixed<std::variant_alternative_t<Seq, Variant>...>(
	    reference);
}

template <class... ObjectTypes>
std::variant<ObjectTypes...> object_stream::dereference_Variant_fixed(
    const object_types::indirect_reference& reference) {
	using namespace object_types;
	using namespace xref_types;

	const std::pair<object_t, generation_t> object_id = {
	    reference.object_number, reference.generation_number};

	if (!m_object_cache.contains(object_id)) {
		object_types::any_direct_object new_object = object_types::null;
		if (auto xref_info = m_xref_table.find(object_id.first, object_id.second);
		    xref_info != m_xref_table.end() &&
		    std::holds_alternative<xref_types::xref_inuse_entry>(*xref_info)) {
			new_object = take_indirect_object(
			    std::get<xref_types::xref_inuse_entry>(*xref_info));
		}

		m_object_cache.add(object_id, std::move(new_object));
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
	    m_object_cache.get(object_id));
}
#pragma endregion // stream_parser_template_definitions_on_old_object_cache
