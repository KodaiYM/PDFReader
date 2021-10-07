#pragma once

#include "pdfparser.graphics_state.hpp"

#include <stack>

namespace pdfparser {
class graphics_state_stack {
public:
	void store();
	void restore() noexcept(noexcept(m_graphics_states.pop()));
	[[nodiscard]] graphics_state&
	    top() noexcept(noexcept(m_graphics_states.top()));
	[[nodiscard]] const graphics_state& top() const
	    noexcept(noexcept(m_graphics_states.top()));

public:
	graphics_state_stack();

private:
	std::stack<graphics_state> m_graphics_states;
};
} // namespace pdfparser
