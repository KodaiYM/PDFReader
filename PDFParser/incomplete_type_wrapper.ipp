#pragma once

#include "incomplete_type_wrapper.hpp"

template <class T>
incomplete_type_wrapper<T>::incomplete_type_wrapper()
    : m_value(std::make_unique<T>()) {}

template <class T>
incomplete_type_wrapper<T>::incomplete_type_wrapper(
    const incomplete_type_wrapper& source)
    : m_value(std::make_unique<T>(*source.m_value.get())) {}

template <class T>
incomplete_type_wrapper<T>::incomplete_type_wrapper(
    incomplete_type_wrapper&& source)
    : m_value(std::move(source.m_value)) {}

template <class T>
incomplete_type_wrapper<T>::incomplete_type_wrapper(const T& value)
    : m_value(std::make_unique<T>(value)) {}

template <class T>
incomplete_type_wrapper<T>::incomplete_type_wrapper(T&& value)
    : m_value(std::make_unique<T>(std::move(value))) {}

template <class T>
template <class U,
          std::enable_if_t<std::is_constructible_v<T, U&&>, std::nullptr_t>>
incomplete_type_wrapper<T>::incomplete_type_wrapper(U&& value)
    : m_value(std::make_unique<T>(std::forward<U&&>(value))) {}

template <class T>
auto incomplete_type_wrapper<T>::operator=(const incomplete_type_wrapper& rhs)
    -> incomplete_type_wrapper& {
	if (rhs != *this) {
		m_value = std::make_unique<T>(*rhs.m_value.get());
	}
	return *this;
}
template <class T>
auto incomplete_type_wrapper<T>::operator=(incomplete_type_wrapper&& rhs)
    -> incomplete_type_wrapper& {
	if (rhs != *this) {
		m_value = std::move(rhs.m_value);
	}
	return *this;
}

template <class T>
incomplete_type_wrapper<T>::operator T&() & noexcept {
	return *m_value;
}
template <class T>
incomplete_type_wrapper<T>::operator const T&() const volatile& noexcept {
	return *const_cast<const incomplete_type_wrapper*>(this)->m_value;
}
template <class T>
incomplete_type_wrapper<T>::operator T() && noexcept {
	return static_cast<incomplete_type_wrapper&&>(*this).get();
}

template <class T>
template <class U>
incomplete_type_wrapper<T>::operator U() & {
	return *m_value;
}
template <class T>
template <class U>
incomplete_type_wrapper<T>::operator U() const& {
	return *m_value;
}
template <class T>
template <class U>
incomplete_type_wrapper<T>::operator U() && {
	return static_cast<incomplete_type_wrapper&&>(*this).get();
}

template <class T>
T& incomplete_type_wrapper<T>::get() & noexcept {
	return *m_value;
}
template <class T>
const T& incomplete_type_wrapper<T>::get() const& noexcept {
	return *m_value;
}
template <class T>
T incomplete_type_wrapper<T>::get() && noexcept {
	T value = std::move(*m_value);
	m_value.reset();
	return std::move(value);
}

template <class T>
bool operator==(const incomplete_type_wrapper<T>& lhs,
                const incomplete_type_wrapper<T>& rhs) {
	return lhs.get() == rhs.get();
}
template <class T>
bool operator!=(const incomplete_type_wrapper<T>& lhs,
                const incomplete_type_wrapper<T>& rhs) {
	return !(lhs == rhs);
}
