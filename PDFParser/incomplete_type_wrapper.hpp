#pragma once

#include <memory>
#include <type_traits>

template <class T>
class incomplete_type_wrapper {
public:
	operator T&() & noexcept;
	operator const T&() const& noexcept;
	operator T() && noexcept;

	template <class U>
	operator U() &;
	template <class U>
	operator U() const&;
	template <class U>
	operator U() &&;

	T&       get() & noexcept;
	const T& get() const& noexcept;
	T        get() && noexcept;

public:
	incomplete_type_wrapper();
	incomplete_type_wrapper(const incomplete_type_wrapper& source);
	incomplete_type_wrapper(incomplete_type_wrapper&& source);

	incomplete_type_wrapper(const T& value);
	incomplete_type_wrapper(T&& value);

	template <class U, std::enable_if_t<std::is_constructible_v<T, U&&>,
	                                    std::nullptr_t> = nullptr>
	incomplete_type_wrapper(U&& value);

public:
	incomplete_type_wrapper& operator=(const incomplete_type_wrapper& rhs);
	incomplete_type_wrapper& operator=(incomplete_type_wrapper&& rhs);

private:
	std::unique_ptr<T> m_value;
};

template <typename T>
bool operator==(const incomplete_type_wrapper<T>& lhs,
                const incomplete_type_wrapper<T>& rhs);
template <typename T>
bool operator!=(const incomplete_type_wrapper<T>& lhs,
                const incomplete_type_wrapper<T>& rhs);

#include "incomplete_type_wrapper.ipp"
