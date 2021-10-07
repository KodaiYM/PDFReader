#pragma once

#include "pdfparser.object_types.hpp"

#include <algorithm>

using namespace pdfparser::object_types;

// definition of inline functions
inline bool stream_object::operator==(const stream_object& rhs) const noexcept {
	return std::tie(m_stream_dictionary, m_encoded_data, m_decoded_data) ==
	       std::tie(rhs.m_stream_dictionary, rhs.m_encoded_data,
	                rhs.m_decoded_data);
}
inline bool stream_object::operator!=(const stream_object& rhs) const noexcept {
	return !(*this == rhs);
}
constexpr bool null_object::operator==(const null_object&) const noexcept {
	return true;
}
constexpr bool null_object::operator!=(const null_object& rhs) const noexcept {
	return !(*this == rhs);
}
constexpr bool indirect_reference::operator==(
    const indirect_reference& rhs) const noexcept {
	return std::tie(object_number, generation_number) ==
	       std::tie(rhs.object_number, rhs.generation_number);
}
constexpr bool indirect_reference::operator!=(
    const indirect_reference& rhs) const noexcept {
	return !(*this == rhs);
}

// definition of template functions
template <
    class Variant,
    std::enable_if_t<is_same_template_v<std::variant, Variant>, std::nullptr_t>>
array_object::operator std::vector<Variant>() {
	return operator_vector_Variant_impl<Variant>(
	    std::make_index_sequence<std::variant_size_v<Variant>>());
}
template <class... ObjectTypes,
          std::enable_if_t<sizeof...(ObjectTypes) >= 2, std::nullptr_t>>
array_object::operator std::vector<std::variant<ObjectTypes...>>() {
	return operator_vector_Variant_fixed<ObjectTypes...>();
}
template <class ObjectType,
          std::enable_if_t<!is_same_template_v<std::variant, ObjectType>,
                           std::nullptr_t>>
array_object::operator std::vector<ObjectType>() {
	std::vector<std::variant<ObjectType>> variant_vector =
	    operator_vector_Variant_fixed<ObjectType>();

	std::vector<ObjectType> ret_vector;
	ret_vector.reserve(variant_vector.size());
	for (auto& element : variant_vector) {
		ret_vector.push_back(std::move(std::get<ObjectType>(element)));
	}

	return std::move(ret_vector);
}

template <class Variant, std::size_t... Seq>
std::vector<Variant>
    array_object::operator_vector_Variant_impl(std::index_sequence<Seq...>) {
	return operator_vector_Variant_fixed<
	    std::variant_alternative_t<Seq, Variant>...>();
}
template <class... ObjectTypes>
std::vector<std::variant<ObjectTypes...>>
    array_object::operator_vector_Variant_fixed() const& {
	std::vector<std::variant<ObjectTypes...>> ret_vector;
	ret_vector.reserve(this->size());

	for (const auto& element : *this) {
		ret_vector.push_back(std::visit(
		    [](const auto& concrete_object) -> std::variant<ObjectTypes...> {
			    if constexpr ((... || std::is_same_v<
			                              ObjectTypes,
			                              std::decay_t<decltype(concrete_object)>>)) {
				    return concrete_object;
			    } else {
				    throw type_mismatch();
			    }
		    },
		    element));
	}

	return std::move(ret_vector);
}

template <class DictionaryT, class DataT>
stream_object::stream_object(DictionaryT&& stream_dictionary,
                             DataT&&       encoded_data)
    : m_stream_dictionary(std::forward<DictionaryT>(stream_dictionary)),
      m_encoded_data(std::forward<DataT>(encoded_data)) {}
