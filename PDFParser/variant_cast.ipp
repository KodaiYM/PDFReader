#pragma once

#include "variant_cast.hpp"

#include <type_traits>

template <class... FromTypes>
template <class... ToTypes>
variant_cast_box<FromTypes...>::operator std::variant<ToTypes...>() const& {
	return std::visit(
	    [](const auto& concrete_object) -> std::variant<ToTypes...> {
		    using type = std::decay_t<decltype(concrete_object)>;
		    if constexpr ((... || std::is_same_v<type, ToTypes>)) {
			    return concrete_object;
		    } else {
			    throw std::bad_variant_access();
		    }
	    },
	    object);
}

template <class... FromTypes>
template <class... ToTypes>
variant_cast_box<FromTypes...>::operator std::variant<ToTypes...>() && {
	return std::visit(
	    [](auto&& concrete_object) -> std::variant<ToTypes...> {
		    using type = std::decay_t<decltype(concrete_object)>;
		    if constexpr ((... || std::is_same_v<type, ToTypes>)) {
			    return std::move(concrete_object);
		    } else {
			    throw std::bad_variant_access();
		    }
	    },
	    std::move(object));
}

template <class... Types>
variant_cast_box<Types...> variant_cast(std::variant<Types...> object) {
	return {object};
}
