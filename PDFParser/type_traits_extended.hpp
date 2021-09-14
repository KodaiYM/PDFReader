#pragma once

#include <type_traits>

template <template <typename...> class ExpectedT, class ActualT>
struct is_same_template: std::false_type {};

template <template <typename...> class ExpectedT, typename... Args>
struct is_same_template<ExpectedT, ExpectedT<Args...>>: std::true_type {};

template <template <typename...> class ExpectedT, class ActualT>
constexpr bool is_same_template_v = is_same_template<ExpectedT, ActualT>::value;
