#include "pdfparser.graphics_state_stack.hpp"

using namespace pdfparser;

graphics_state_stack::graphics_state_stack() {
	m_graphics_states.emplace();
}

void graphics_state_stack::store() {
	m_graphics_states.push(top());
}
void graphics_state_stack::restore() noexcept(
    noexcept(m_graphics_states.pop())) {
	m_graphics_states.pop();
}
[[nodiscard]] graphics_state&
    graphics_state_stack::top() noexcept(noexcept(m_graphics_states.top())) {
	return m_graphics_states.top();
}
[[nodiscard]] const graphics_state& pdfparser::graphics_state_stack::top() const
    noexcept(noexcept(m_graphics_states.top())) {
	return m_graphics_states.top();
}
