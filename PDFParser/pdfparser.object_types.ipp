#pragma once

#include "pdfparser.object_types.hpp"
#include "pdfparser.object_types_errors.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>

namespace pdfparser::object_types {

#pragma region boolean_object
template <typename BoolT,
          std::enable_if_t<std::is_same_v<BoolT, bool>, std::nullptr_t>>
constexpr boolean_object::boolean_object(BoolT value) noexcept
    : m_value(value) {}

constexpr boolean_object::operator bool() const noexcept {
	return m_value;
}
#pragma endregion // region boolean_object

#pragma region onstream_boolean_object
inline onstream_boolean_object::onstream_boolean_object(
    std::streampos position, boolean_object value) noexcept
    : portion_of_stream(std::move(position)), boolean_object(std::move(value)) {
}
#pragma endregion // region onstream_boolean_object

#pragma region integer_object
template <typename IntegerT,
          std::enable_if_t<std::is_integral_v<IntegerT> &&
                               !std::is_same_v<IntegerT, bool>,
                           std::nullptr_t>>
constexpr integer_object::integer_object(IntegerT value) {
	if (value >= 0) {
		if (value > std::numeric_limits<int_type>::max()) {
			throw integer_object_overflows();
		}
	} else {
		assert(value < 0);
		if (value < std::numeric_limits<int_type>::min()) {
			throw integer_object_overflows();
		}
	}
	m_value = value;
}

template <typename IntegerT,
          std::enable_if_t<std::is_integral_v<IntegerT> &&
                               !std::is_same_v<IntegerT, bool>,
                           std::nullptr_t>>
constexpr integer_object::operator IntegerT() const {
	if constexpr (std::is_signed_v<IntegerT>) {
		if (!(std::numeric_limits<IntegerT>::min() <= m_value &&
		      m_value <= std::numeric_limits<IntegerT>::max())) {
			throw integer_object_overflows();
		}
	} else {
		static_assert(std::is_unsigned_v<IntegerT>);

		const std::make_unsigned_t<int_type> m_value_u = m_value;

		if (m_value < 0 || !(std::numeric_limits<IntegerT>::min() <= m_value_u &&
		                     m_value_u <= std::numeric_limits<IntegerT>::max())) {
			throw integer_object_overflows();
		}
	}

	return static_cast<IntegerT>(m_value);
}

constexpr integer_object::operator int_type() const noexcept {
	return m_value;
}
#pragma endregion // region integer_object

#pragma region onstream_integer_object
inline onstream_integer_object::onstream_integer_object(std::streampos position,
                                                        integer_object value)
    : portion_of_stream(std::move(position)), integer_object(std::move(value)) {
}
inline onstream_integer_object::onstream_integer_object(
    std::streampos position, const std::string& str) try
    : portion_of_stream(std::move(position)),
      integer_object(std::stoll(str, nullptr, 10)) {
	static_assert(std::is_same_v<long long, int_type>);
} catch (std::out_of_range&) {
	throw onstream_integer_object_overflows(position);
}

template <typename IntegerT,
          std::enable_if_t<std::is_integral_v<IntegerT> &&
                               !std::is_same_v<IntegerT, bool>,
                           std::nullptr_t>>
onstream_integer_object::operator IntegerT() const {
	try {
		return static_cast<const integer_object&>(*this).operator IntegerT();
	} catch (integer_object_overflows&) {
		throw onstream_integer_object_overflows(position());
	}
}

constexpr bool operator==(const integer_object& lhs,
                          const integer_object& rhs) noexcept {
	return lhs.m_value == rhs.m_value;
}
#pragma endregion // region onstream_integer_object

#pragma region real_object
template <typename RealT,
          std::enable_if_t<std::is_floating_point_v<RealT>, std::nullptr_t>>
constexpr real_object::real_object(RealT value) noexcept : m_value(value) {}

constexpr real_object::operator double() const noexcept {
	return m_value;
}
#pragma endregion // region real_object

#pragma region onstream_real_object
inline onstream_real_object::onstream_real_object(std::streampos position,
                                                  real_object    value) noexcept
    : portion_of_stream(std::move(position)), real_object(std::move(value)) {}
#pragma endregion // region onstream_real_object

#pragma region string_object
inline string_object::string_object(const std::string& another)
    : std::string(another) {}
inline string_object::string_object(std::string&& another) noexcept
    : std::string(std::move(another)) {}
#pragma endregion // region string_object

#pragma region onstream_string_object
inline onstream_string_object::onstream_string_object(
    std::streampos position, string_object value) noexcept
    : portion_of_stream(std::move(position)), string_object(std::move(value)) {}
#pragma endregion // region onstream_string_object

#pragma region name_object
inline name_object::name_object(const std::string& another)
    : std::string(another) {}
inline name_object::name_object(std::string&& another)
    : std::string(std::move(another)) {}
#pragma endregion // region name_object

#pragma region onstream_name_object
inline onstream_name_object::onstream_name_object(std::streampos position,
                                                  name_object    value) noexcept
    : portion_of_stream(std::move(position)), name_object(std::move(value)) {}
#pragma endregion // region onstream_name_object

#pragma region array_object_base
template <class T>
auto array_object_base<T>::at(typename base::size_type n) ->
    typename base::reference {
	if (n >= base::size()) {
		throw array_out_of_range();
	} else {
		return base::operator[](n);
	}
}
template <class T>
auto array_object_base<T>::at(typename base::size_type n) const ->
    typename base::const_reference {
	if (n >= base::size()) {
		throw array_out_of_range();
	} else {
		return base::operator[](n);
	}
}
#pragma endregion // region array_object_base

#pragma region onstream_array_object
inline onstream_array_object::onstream_array_object(
    std::streampos position, array_object_base value) noexcept
    : portion_of_stream(std::move(position)),
      array_object_base(std::move(value)) {}

inline onstream_array_object::operator array_object() const& {
	return array_object(begin(), end());
}
inline onstream_array_object::operator array_object() && {
	return array_object(std::make_move_iterator(begin()),
	                    std::make_move_iterator(end()));
}

inline auto onstream_array_object::at(typename base::size_type n) ->
    typename base::reference {
	try {
		return array_object_base::at(n);
	} catch (array_out_of_range&) {
		throw array_onstream_out_of_range{position()};
	}
}
inline auto onstream_array_object::at(typename base::size_type n) const ->
    typename base::const_reference {
	try {
		return array_object_base::at(n);
	} catch (array_out_of_range&) {
		throw array_onstream_out_of_range{position()};
	}
}
#pragma endregion // region onstream_array_object

#pragma region dictionary_object_base
#pragma endregion // region dictionary_object_base

#pragma region onstream_dictionary_object
inline auto    onstream_dictionary_object::find(const name_object& key)
    -> iterator {
	return base::find({0, key});
}
inline auto onstream_dictionary_object::find(const name_object& key) const
    -> const_iterator {
	return base::find({0, key});
}

inline auto onstream_dictionary_object::count(const name_object& key) const
    -> size_type {
	return base::count({0, key});
}
inline bool onstream_dictionary_object::contains(const name_object& key) const {
	return find(key) != end();
}

inline auto onstream_dictionary_object::equal_range(const name_object& key)
    -> std::pair<iterator, iterator> {
	return base::equal_range({0, key});
}
inline auto
    onstream_dictionary_object::equal_range(const name_object& key) const
    -> std::pair<const_iterator, const_iterator> {
	return base::equal_range({0, key});
}

inline auto onstream_dictionary_object::at(const name_object& key)
    -> mapped_type& {
	return base::at({0, key});
}
inline auto onstream_dictionary_object::at(const name_object& key) const
    -> const mapped_type& {
	return base::at({0, key});
}

inline onstream_dictionary_object::operator dictionary_object() const& {
	return {begin(), end(), size()};
}
inline onstream_dictionary_object::operator dictionary_object() && {
	return {std::make_move_iterator(begin()), std::make_move_iterator(end()),
	        size()};
}

inline onstream_dictionary_object::onstream_dictionary_object(
    std::streampos position, dictionary_object_base value) noexcept
    : portion_of_stream(std::move(position)),
      dictionary_object_base(std::move(value)) {}
#pragma endregion // region onstream_dictionary_object

#pragma region stream_object_base
template <class Dictionary>
inline stream_object_base<Dictionary>::stream_object_base(
    Dictionary stream_dictionary, std::string encoded_data) noexcept
    : m_stream_dictionary(std::move(stream_dictionary)),
      m_encoded_data(std::move(encoded_data)) {}

template <class Dictionary>
std::string stream_object_base<Dictionary>::get_decoded_data() const {
	// HACK: Filterに応じてデコード
	return m_decoded_data;
}

template <class Dictionary>
inline bool stream_object_base<Dictionary>::operator==(
    const stream_object_base& rhs) const noexcept {
	return std::tie(m_stream_dictionary, m_encoded_data, m_decoded_data) ==
	       std::tie(rhs.m_stream_dictionary, rhs.m_encoded_data,
	                rhs.m_decoded_data);
}
template <class Dictionary>
inline bool stream_object_base<Dictionary>::operator!=(
    const stream_object_base& rhs) const noexcept {
	return !(*this == rhs);
}
#pragma endregion // region stream_object_base

#pragma region                 onstream_stream_object
inline onstream_stream_object::operator stream_object() const& {
	return {m_stream_dictionary, m_encoded_data};
}
inline onstream_stream_object::operator stream_object() && {
	return {std::move(m_stream_dictionary), std::move(m_encoded_data)};
}

inline onstream_stream_object::onstream_stream_object(
    onstream_dictionary_object stream_dictionary,
    std::string                encoded_data) noexcept
    : portion_of_stream(stream_dictionary.position()),
      stream_object_base(std::move(stream_dictionary),
                         std::move(encoded_data)) {}
#pragma endregion // region onstream_stream_object

#pragma region null_object
inline bool null_object::operator==(const null_object&) const noexcept {
	return true;
}
inline bool null_object::operator!=(const null_object& rhs) const noexcept {
	return !(*this == rhs);
}
#pragma endregion // region null_object

#pragma region onstream_null_object
inline onstream_null_object::onstream_null_object(
    std::streampos position) noexcept
    : portion_of_stream(std::move(position)) {}
#pragma endregion // region onstream_null_object

#pragma region indirect_reference
constexpr indirect_reference::indirect_reference(
    xref_types::object_t     object_number,
    xref_types::generation_t generation_number) noexcept
    : object_number(std::move(object_number)),
      generation_number(std::move(generation_number)) {}

constexpr bool indirect_reference::operator==(
    const indirect_reference& rhs) const noexcept {
	return std::tie(object_number, generation_number) ==
	       std::tie(rhs.object_number, rhs.generation_number);
}
constexpr bool indirect_reference::operator!=(
    const indirect_reference& rhs) const noexcept {
	return !(*this == rhs);
}
#pragma endregion // region indirect_reference

#pragma region onstream_indirect_reference
inline onstream_indirect_reference::onstream_indirect_reference(
    std::streampos position, indirect_reference value) noexcept
    : portion_of_stream(std::move(position)),
      indirect_reference(std::move(value)) {}
#pragma endregion // region onstream_indirect_reference

#pragma region onstream_variant_base
template <class VariantObject, class VariantBase>
inline onstream_variant_base<VariantObject, VariantBase>::operator VariantBase()
    const& {
	return std::visit(
	    [](const auto& concrete_object) -> VariantBase {
		    return concrete_object;
	    },
	    static_cast<const typename VariantObject::variant_base&>(*this));
}

template <class VariantObject, class VariantBase>
inline onstream_variant_base<VariantObject,
                             VariantBase>::operator VariantBase() && {
	return std::visit(
	    [](auto&& concrete_object) -> VariantBase {
		    return std::move(concrete_object);
	    },
	    static_cast<typename VariantObject::variant_base&&>(*this));
}
#pragma endregion // onstream_variant_base

#pragma region variant_object
template <class Visitor, class VariantObject>
constexpr decltype(auto) visit(Visitor&& visitor, VariantObject&& object) {
	using variant_type =
	    typename std::remove_reference_t<VariantObject>::variant_base;

	if constexpr (std::is_lvalue_reference_v<VariantObject>) {
		return std::visit(std::forward<Visitor>(visitor),
		                  static_cast<const variant_type&>(object));
	} else {
		return std::visit(std::forward<Visitor>(visitor),
		                  static_cast<variant_type&&>(object));
	}
}

template <typename... ObjectTypes>
template <class ObjectType,
          std::enable_if_t<std::is_constructible_v<std::variant<ObjectTypes...>,
                                                   ObjectType&&>,
                           std::nullptr_t>>
variant_object<ObjectTypes...>::variant_object(ObjectType&& another)
    : variant_base(std::forward<ObjectType>(another)) {}

template <typename... ObjectTypes>
template <typename... FromTypes>
variant_object<ObjectTypes...>::variant_object(
    const variant_object<FromTypes...>& another)
    : variant_base(visit(
          [](const auto& concrete_object) -> variant_base {
	          using type = std::decay_t<decltype(concrete_object)>;
	          if constexpr (std::is_constructible_v<variant_base, type>) {
		          return concrete_object;
	          } else {
		          if constexpr (std::is_base_of_v<portion_of_stream, type>) {
			          throw onstream_type_mismatch(concrete_object.position());
		          } else {
			          throw type_mismatch();
		          }
	          }
          },
          another)) {}

template <typename... ObjectTypes>
template <typename... FromTypes>
variant_object<ObjectTypes...>::variant_object(
    variant_object<FromTypes...>&& another) noexcept
    : variant_base(visit(
          [](auto&& concrete_object) -> variant_base {
	          using type = std::decay_t<decltype(concrete_object)>;
	          if constexpr (std::is_constructible_v<variant_base, type>) {
		          return std::move(concrete_object);
	          } else {
		          if constexpr (std::is_base_of_v<portion_of_stream, type>) {
			          throw onstream_type_mismatch(concrete_object.position());
		          } else {
			          throw type_mismatch();
		          }
	          }
          },
          std::move(another))) {}

template <typename... ObjectTypes>
inline variant_object<ObjectTypes...>::~variant_object() noexcept {}

#if defined __GNUC__ || defined __clang__
#pragma GCC diagnostic          ignored "-Wreturn-type"
#elif defined                   _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4715)
#else
#error("未対応のコンパイラです")
#endif
template <typename... ObjectTypes>
template <typename ObjectType,
          std::enable_if_t<
              std::disjunction_v<std::is_same<ObjectType, ObjectTypes>...>,
              std::nullptr_t>>
inline variant_object<ObjectTypes...>::operator const ObjectType&() const
    volatile& {
	const variant_object& self = *const_cast<const variant_object*>(this);
	if (std::holds_alternative<ObjectType>(self)) {
		return std::get<ObjectType>(self);
	} else {
		std::visit(
		    [](const auto& concrete_object) {
			    using type = std::decay_t<decltype(concrete_object)>;

			    if constexpr (std::is_base_of_v<portion_of_stream, type>) {
				    throw onstream_type_mismatch(concrete_object.position());
			    } else {
				    throw type_mismatch();
			    }
		    },
		    static_cast<const variant_base&>(self));
	}
}
#if defined __GNUC__ || defined __clang__
#pragma GCC diagnostic          warning "-Wreturn-type"
#elif defined                   _MSC_VER
#pragma warning(pop)
#else
#error("未対応のコンパイラです")
#endif

template <typename... ObjectTypes>
template <typename ObjectType,
          std::enable_if_t<
              std::disjunction_v<std::is_same<ObjectType, ObjectTypes>...>,
              std::nullptr_t>>
inline variant_object<ObjectTypes...>::operator ObjectType&() & {
	return const_cast<ObjectType&>(this->operator const ObjectType&());
}

template <typename... ObjectTypes>
template <typename ObjectType,
          std::enable_if_t<
              std::disjunction_v<std::is_same<ObjectType, ObjectTypes>...>,
              std::nullptr_t>>
inline variant_object<ObjectTypes...>::operator ObjectType() && {
	return std::move(this->operator ObjectType&());
}
#pragma endregion // region variant_object

#pragma region           number_object
constexpr number_object::operator double() const noexcept {
	return std::visit(
	    [](const auto& concrete_number) noexcept -> double {
		    using T = std::decay_t<decltype(concrete_number)>;

		    if constexpr (std::is_same_v<integer_object, T>) {
			    return static_cast<double>(
			        static_cast<integer_object::int_type>(concrete_number));
		    } else if constexpr (std::is_same_v<real_object, T>) {
			    return static_cast<double>(concrete_number);
		    } else {
			    static_assert([] {
				    return false;
			    }());
		    }
	    },
	    static_cast<const variant_base&>(*this));
}
#pragma endregion // region number_object

#pragma region                    onstream_number_object
constexpr onstream_number_object::operator double() const noexcept {
	return std::visit(
	    [](const auto& concrete_number) noexcept -> double {
		    using T = std::decay_t<decltype(concrete_number)>;

		    if constexpr (std::is_same_v<onstream_integer_object, T>) {
			    return static_cast<double>(
			        static_cast<onstream_integer_object::int_type>(concrete_number));
		    } else if constexpr (std::is_same_v<onstream_real_object, T>) {
			    return static_cast<double>(concrete_number);
		    } else {
			    static_assert([] {
				    return false;
			    }());
		    }
	    },
	    static_cast<const variant_base&>(*this));
}
#pragma endregion // region onstream_number_object

} // namespace pdfparser::object_types
