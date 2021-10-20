#pragma once

#include "incomplete_type_wrapper.hpp"
#include "pdfparser.portion_of_stream.hpp"
#include "pdfparser.xref_types.hpp"
#include "type_traits_extended.hpp"

#include <cassert>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace pdfparser::object_types {
class boolean_object {
public:
	constexpr operator bool() const noexcept;

public:
	template <typename BoolT = bool, std::enable_if_t<std::is_same_v<BoolT, bool>,
	                                                  std::nullptr_t> = nullptr>
	constexpr boolean_object(BoolT value = false) noexcept;

private:
	bool m_value;
};
class onstream_boolean_object: public portion_of_stream, public boolean_object {
public:
	inline onstream_boolean_object(std::streampos position,
	                               boolean_object value) noexcept;
};

class integer_object {
public:
	using int_type = long long;

	template <typename IntegerT,
	          std::enable_if_t<std::is_integral_v<IntegerT> &&
	                               !std::is_same_v<IntegerT, bool>,
	                           std::nullptr_t> = nullptr>
	constexpr operator IntegerT() const;
	constexpr operator int_type() const noexcept;

public:
	template <typename IntegerT                = int_type,
	          std::enable_if_t<std::is_integral_v<IntegerT> &&
	                               !std::is_same_v<IntegerT, bool>,
	                           std::nullptr_t> = nullptr>
	constexpr integer_object(IntegerT value = 0);

private:
	int_type m_value;

	friend constexpr bool operator==(const integer_object& lhs,
	                                 const integer_object& rhs) noexcept;
};
constexpr bool operator==(const integer_object& lhs,
                          const integer_object& rhs) noexcept;
class onstream_integer_object: public portion_of_stream, public integer_object {
public:
	// hide base conversion operator
	template <typename IntegerT,
	          std::enable_if_t<std::is_integral_v<IntegerT> &&
	                               !std::is_same_v<IntegerT, bool>,
	                           std::nullptr_t> = nullptr>
	operator IntegerT() const;

public:
	inline onstream_integer_object(std::streampos position, integer_object value);
	inline onstream_integer_object(std::streampos     position,
	                               const std::string& str);
};

class real_object {
public:
	constexpr operator double() const noexcept;

public:
	template <typename RealT = double,
	          std::enable_if_t<std::is_floating_point_v<RealT>, std::nullptr_t> =
	              nullptr>
	constexpr real_object(RealT value = 0.0) noexcept;

private:
	double m_value;
};
class onstream_real_object: public portion_of_stream, public real_object {
public:
	inline onstream_real_object(std::streampos position,
	                            real_object    value) noexcept;
};

class string_object: public std::string {
public:
	using std::string::string;

	inline string_object(const std::string& another);
	inline string_object(std::string&& another) noexcept;
};
class onstream_string_object: public portion_of_stream, public string_object {
public:
	inline onstream_string_object(std::streampos position,
	                              string_object  value) noexcept;
};

class name_object: public std::string {
public:
	using std::string::string;

	inline name_object(const std::string& another);
	inline name_object(std::string&& another);
};
class onstream_name_object: public portion_of_stream, public name_object {
public:
	inline onstream_name_object(std::streampos position,
	                            name_object    value) noexcept;
};
} // namespace pdfparser::object_types

// name_object can calculate hash
namespace std {
template <>
struct hash<pdfparser::object_types::name_object>: public hash<std::string> {
private:
	using base = hash<std::string>;

public:
	using base::base;
};
template <>
struct hash<pdfparser::object_types::onstream_name_object>
    : public hash<pdfparser::object_types::name_object> {
private:
	using base = hash<pdfparser::object_types::name_object>;

public:
	using base::base;
};
} // namespace std

namespace pdfparser::object_types {
template <class T>
class array_object_base: public std::vector<T> {
public:
	using base = std::vector<T>;
	using base::base;
};
struct array_object: array_object_base<struct direct_object_or_ref> {
	using base = array_object_base;
	using base::base;
};
bool operator==(const array_object& lhs, const array_object& rhs) noexcept;
class onstream_array_object
    : public portion_of_stream,
      public array_object_base<struct onstream_direct_object_or_ref> {
public:
	inline operator array_object() const&;
	inline operator array_object() &&;

public:
	inline onstream_array_object(std::streampos    position,
	                             array_object_base value) noexcept;
};

template <class Key, class Value>
class dictionary_object_base: public std::unordered_map<Key, Value> {
public:
	using base = std::unordered_map<Key, Value>;
	using base::base;
};
struct dictionary_object
    : dictionary_object_base<
          name_object,
          incomplete_type_wrapper<struct non_null_direct_object_or_ref>> {
	using base = dictionary_object_base;
	using base::base;
};
bool operator==(const dictionary_object& lhs,
                const dictionary_object& rhs) noexcept;
class onstream_dictionary_object
    : public portion_of_stream,
      public dictionary_object_base<
          onstream_name_object,
          incomplete_type_wrapper<
              struct onstream_non_null_direct_object_or_ref>> {
public:
	using base = dictionary_object_base;

public:
	inline iterator       find(const name_object& key);
	inline const_iterator find(const name_object& key) const;

	inline size_type count(const name_object& key) const;
	inline bool      contains(const name_object& key) const;

	inline std::pair<iterator, iterator> equal_range(const name_object& key);
	inline std::pair<const_iterator, const_iterator>
	    equal_range(const name_object& key) const;

	inline mapped_type&       at(const name_object& key);
	inline const mapped_type& at(const name_object& key) const;

public:
	inline operator dictionary_object() const&;
	inline operator dictionary_object() &&;

public:
	inline onstream_dictionary_object(std::streampos         position,
	                                  dictionary_object_base value) noexcept;
};

template <class Dictionary>
class stream_object_base {
public:
	std::string get_decoded_data() const;
	inline bool operator==(const stream_object_base& rhs) const noexcept;
	inline bool operator!=(const stream_object_base& rhs) const noexcept;

public:
	inline stream_object_base(Dictionary  stream_dictionary,
	                          std::string encoded_data) noexcept;

protected:
	Dictionary  m_stream_dictionary;
	std::string m_encoded_data;
	std::string m_decoded_data;
};
struct stream_object: stream_object_base<dictionary_object> {
	using base = stream_object_base;
	using base::base;
};
class onstream_stream_object
    : public portion_of_stream,
      public stream_object_base<onstream_dictionary_object> {
public:
	inline operator stream_object() const&;
	inline operator stream_object() &&;

public:
	inline onstream_stream_object(onstream_dictionary_object stream_dictionary,
	                              std::string encoded_data) noexcept;
};

struct null_object {
	inline bool operator==(const null_object& rhs) const noexcept;
	inline bool operator!=(const null_object& rhs) const noexcept;
};
struct onstream_null_object: portion_of_stream, null_object {
	explicit inline onstream_null_object(std::streampos position) noexcept;
};

struct indirect_reference {
	xref_types::object_t     object_number;
	xref_types::generation_t generation_number;

	constexpr bool operator==(const indirect_reference& rhs) const noexcept;
	constexpr bool operator!=(const indirect_reference& rhs) const noexcept;

	constexpr indirect_reference(
	    xref_types::object_t     object_number,
	    xref_types::generation_t generation_number) noexcept;
};
struct onstream_indirect_reference: portion_of_stream, indirect_reference {
	inline onstream_indirect_reference(std::streampos     position,
	                                   indirect_reference value) noexcept;
};

// NOTE: Can't be virtual.
// due to bug of Visual Studio around P0136R1
template <class VariantObject, class VariantBase>
struct onstream_variant_base: /* virtual */ VariantObject {
	operator VariantBase() const&;
	operator VariantBase() &&;

	using VariantObject::VariantObject;
};

template <class Visitor, class VariantObject>
constexpr decltype(auto) visit(Visitor&& visitor, VariantObject&& object);
template <typename... ObjectTypes>
class variant_object: public std::variant<ObjectTypes...> {
public:
	// NOTE:
	// volatile を使う理由は、rvalue を const ObjectType& で受け取っていた場合に、
	// この関数が呼ばれてしまうのを防ぐため
	template <typename ObjectType,
	          std::enable_if_t<
	              std::disjunction_v<std::is_same<ObjectType, ObjectTypes>...>,
	              std::nullptr_t> = nullptr>
	inline operator const ObjectType&() const volatile&;

	template <typename ObjectType,
	          std::enable_if_t<
	              std::disjunction_v<std::is_same<ObjectType, ObjectTypes>...>,
	              std::nullptr_t> = nullptr>
	inline operator ObjectType&() &;

	template <typename ObjectType,
	          std::enable_if_t<
	              std::disjunction_v<std::is_same<ObjectType, ObjectTypes>...>,
	              std::nullptr_t> = nullptr>
	inline operator ObjectType() &&;

public:
	using variant_base = std::variant<ObjectTypes...>;

	static_assert(sizeof...(ObjectTypes) >= 2);

	template <class ObjectType,
	          std::enable_if_t<std::is_constructible_v<
	                               std::variant<ObjectTypes...>, ObjectType&&>,
	                           std::nullptr_t> = nullptr>
	inline variant_object(ObjectType&& another);

	template <typename... FromTypes>
	inline variant_object(const variant_object<FromTypes...>& another);

	template <typename... FromTypes>
	inline variant_object(variant_object<FromTypes...>&& another) noexcept;

	inline virtual ~variant_object() noexcept = 0;
};

struct direct_object
    : public variant_object<boolean_object, integer_object, real_object,
                            string_object, name_object, array_object,
                            dictionary_object, stream_object, null_object> {
	using base = variant_object;
	using base::base;
};
struct onstream_direct_object
    : onstream_variant_base<
          variant_object<onstream_boolean_object, onstream_integer_object,
                         onstream_real_object, onstream_string_object,
                         onstream_name_object, onstream_array_object,
                         onstream_dictionary_object, onstream_stream_object,
                         onstream_null_object>,
          direct_object> {
	using base = onstream_variant_base;
	using base::base;
};

struct direct_object_or_ref
    : public variant_object<boolean_object, integer_object, real_object,
                            string_object, name_object, array_object,
                            dictionary_object, stream_object, null_object,
                            indirect_reference> {
	using base = variant_object;
	using base::base;
};
struct onstream_direct_object_or_ref
    : onstream_variant_base<
          variant_object<onstream_boolean_object, onstream_integer_object,
                         onstream_real_object, onstream_string_object,
                         onstream_name_object, onstream_array_object,
                         onstream_dictionary_object, onstream_stream_object,
                         onstream_null_object, onstream_indirect_reference>,
          direct_object_or_ref> {
	using base = onstream_variant_base;
	using base::base;
};

struct non_null_direct_object_or_ref
    : public variant_object<boolean_object, integer_object, real_object,
                            string_object, name_object, array_object,
                            dictionary_object, stream_object,
                            indirect_reference> {
	using base = variant_object;
	using base::base;
};
struct onstream_non_null_direct_object_or_ref
    : onstream_variant_base<
          variant_object<onstream_boolean_object, onstream_integer_object,
                         onstream_real_object, onstream_string_object,
                         onstream_name_object, onstream_array_object,
                         onstream_dictionary_object, onstream_stream_object,
                         onstream_indirect_reference>,
          non_null_direct_object_or_ref> {
	using base = onstream_variant_base;
	using base::base;
};

/* NOTE: Visual Studio バグのため、以下のロジックは使えない
template <typename INT, typename REAL>
struct number_object_base: virtual variant_object<INT, REAL> {
public:
  constexpr operator double() const noexcept;

public:
  using base = variant_object<INT, REAL>;
  using base::base;
};
struct number_object: number_object_base<integer_object, real_object> {
  using base = number_object_base;
  using base::base;
};
struct onstream_number_object
    : number_object_base<onstream_integer_object, onstream_real_object>,
      onstream_variant_base<
          variant_object<onstream_integer_object, onstream_real_object>,
          number_object> {
public:
  using base = number_object_base;
  using base::base;
};
#pragma region number_object_base
template <typename INT, typename REAL>
constexpr number_object_base<INT, REAL>::operator double() const noexcept {
  return std::visit(
      [](const auto& concrete_number) noexcept -> double {
        using T = std::decay_t<decltype(concrete_number)>;

        if constexpr (std::is_same_v<INT, T>) {
          return static_cast<double>(
              static_cast<typename INT::int_type>(concrete_number));
        } else if constexpr (std::is_same_v<REAL, T>) {
          return static_cast<double>(concrete_number);
        } else {
          static_assert([] {
            return false;
          }());
        }
      },
      static_cast<const typename base::variant_base&>(*this));
}
#pragma endregion // region number_object_base
*/

struct number_object: variant_object<integer_object, real_object> {
public:
	constexpr operator double() const noexcept;

public:
	using base = variant_object;
	using base::base;
};
struct onstream_number_object
    : onstream_variant_base<
          variant_object<onstream_integer_object, onstream_real_object>,
          number_object> {
public:
	constexpr operator double() const noexcept;

public:
	using base = onstream_variant_base;
	using base::base;
};

} // namespace pdfparser::object_types

#include "pdfparser.object_types.ipp"
