#pragma once

#include "pdfparser.xref_types.hpp"

#include <cassert>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace pdfparser::object_types {
class boolean_object {
public:
	template <typename BoolT = bool, std::enable_if_t<std::is_same_v<BoolT, bool>,
	                                                  std::nullptr_t> = nullptr>
	constexpr boolean_object(BoolT value = false) noexcept : m_value(value) {}
	constexpr operator bool() const noexcept {
		return m_value;
	}

private:
	bool m_value;
};
class integer_object {
public:
	using int_type = long long;

	template <typename IntegerT,
	          std::enable_if_t<std::is_integral_v<IntegerT> &&
	                               !std::is_same_v<IntegerT, bool>,
	                           std::nullptr_t> = nullptr>
	constexpr operator IntegerT() const {
		if constexpr (std::is_signed_v<IntegerT>) {
			if (!(std::numeric_limits<IntegerT>::min() <= m_value &&
			      m_value <= std::numeric_limits<IntegerT>::max())) {
				throw std::overflow_error("overflow");
			}
		} else {
			static_assert(std::is_unsigned_v<IntegerT>);

			const std::make_unsigned_t<int_type> m_value_u = m_value;

			if (m_value < 0 || !(std::numeric_limits<IntegerT>::min() <= m_value_u &&
			                     m_value_u <= std::numeric_limits<IntegerT>::max())) {
				throw std::overflow_error("overflow");
			}
		}

		return static_cast<IntegerT>(m_value);
	}

	[[nodiscard]] constexpr bool
	    operator==(const integer_object& rhs) const noexcept {
		return m_value == rhs.m_value;
	}
	[[nodiscard]] constexpr bool
	    operator!=(const integer_object& rhs) const noexcept {
		return !(*this == rhs);
	}

public:
	constexpr integer_object() noexcept : m_value(0) {}
	template <typename IntegerT,
	          std::enable_if_t<std::is_integral_v<IntegerT> &&
	                               !std::is_same_v<IntegerT, bool>,
	                           std::nullptr_t> = nullptr>
	constexpr integer_object(IntegerT value) {
		if (value >= 0) {
			if (value > std::numeric_limits<int_type>::max()) {
				throw std::overflow_error("overflow");
			}
		} else {
			assert(value < 0);
			if (value < std::numeric_limits<int_type>::min()) {
				throw std::overflow_error("overflow");
			}
		}
		m_value = value;
	}

private:
	int_type m_value;
};
class real_object {
public:
	template <typename RealT = double,
	          std::enable_if_t<std::is_floating_point_v<RealT>, std::nullptr_t> =
	              nullptr>
	constexpr real_object(RealT value = 0.0) noexcept : m_value(value) {}
	constexpr operator double() const noexcept {
		return m_value;
	}

private:
	double m_value;
};

class string_object: public std::string {
private:
	using base = std::string;

public:
	using base::base;
};
class name_object: public std::string {
private:
	using base = std::string;

public:
	using base::base;
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
} // namespace std

namespace pdfparser::object_types {
class array_object;
class dictionary_object;
class stream_object;
struct null_object;
struct indirect_reference;

using any_direct_object =
    std::variant<boolean_object, integer_object, real_object, string_object,
                 name_object, array_object, dictionary_object, stream_object,
                 null_object>;
using any_direct_object_or_ref =
    std::variant<boolean_object, integer_object, real_object, string_object,
                 name_object, array_object, dictionary_object, stream_object,
                 null_object, indirect_reference>;
using non_null_direct_object_or_ref =
    std::variant<boolean_object, integer_object, real_object, string_object,
                 name_object, array_object, dictionary_object, stream_object,
                 indirect_reference>;
using number_object = std::variant<integer_object, real_object>;
double number_to_double(const number_object& number_obj) noexcept;

class array_object: public std::vector<any_direct_object_or_ref> {
private:
	using base = std::vector<any_direct_object_or_ref>;

public:
	using base::base;
};
class dictionary_object
    : public std::unordered_map<name_object, non_null_direct_object_or_ref> {
private:
	using base = std::unordered_map<name_object, non_null_direct_object_or_ref>;

public:
	using base::base;
};

class stream_object {
public:
	std::string get_decoded_data() const;
	inline bool operator==(const stream_object& rhs) const noexcept;
	inline bool operator!=(const stream_object& rhs) const noexcept;

public:
	template <class DictionaryT = dictionary_object, class DataT = std::string>
	stream_object(DictionaryT&& stream_dictionary, DataT&& encoded_data);

private:
	dictionary_object m_stream_dictionary;
	std::string       m_encoded_data;
	std::string       m_decoded_data;
};
struct null_object {
	constexpr bool operator==(const null_object& rhs) const noexcept;
	constexpr bool operator!=(const null_object& rhs) const noexcept;
};
struct indirect_reference {
	xref_types::object_t     object_number;
	xref_types::generation_t generation_number;

	constexpr bool operator==(const indirect_reference& rhs) const noexcept;
	constexpr bool operator!=(const indirect_reference& rhs) const noexcept;
};

constexpr null_object null;
} // namespace pdfparser::object_types

// definition of inline functions
namespace pdfparser::object_types {
bool stream_object::operator==(const stream_object& rhs) const noexcept {
	return std::tie(m_stream_dictionary, m_encoded_data, m_decoded_data) ==
	       std::tie(rhs.m_stream_dictionary, rhs.m_encoded_data,
	                rhs.m_decoded_data);
}
bool stream_object::operator!=(const stream_object& rhs) const noexcept {
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
} // namespace pdfparser::object_types

// definition of template functions
namespace pdfparser::object_types {
template <class DictionaryT, class DataT>
stream_object::stream_object(DictionaryT&& stream_dictionary,
                             DataT&&       encoded_data)
    : m_stream_dictionary(std::forward<DictionaryT>(stream_dictionary)),
      m_encoded_data(std::forward<DataT>(encoded_data)) {}
} // namespace pdfparser::object_types
