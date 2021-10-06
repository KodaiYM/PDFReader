#pragma once

#include <variant>

template <class... FromTypes>
struct variant_cast_box {
	std::variant<FromTypes...> object;

	template <class... ToTypes>
	operator std::variant<ToTypes...>() const&;

	template <class... ToTypes>
	operator std::variant<ToTypes...>() &&;
};

template <class... FromTypes>
variant_cast_box<FromTypes...> variant_cast(std::variant<FromTypes...> object);

#include "variant_cast.ipp"
