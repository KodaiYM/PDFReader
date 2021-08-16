#include "PDFParser.xref_types.h"

#include <cassert>

namespace pdfparser::xref_types {
#pragma region xref_table
xref_table::xref_table(std::initializer_list<value_type> entry_list) {
	for (auto&& entry : entry_list) {
		insert(entry); // only copy is allowed on initializer_list
	}
}

auto xref_table::begin() -> iterator {
	return m_table.begin();
}
auto xref_table::begin() const -> const_iterator {
	return m_table.begin();
}
auto xref_table::cbegin() const -> const_iterator {
	return begin();
}
auto xref_table::end() -> iterator {
	return m_table.end();
}
auto xref_table::end() const -> const_iterator {
	return m_table.end();
}
auto xref_table::cend() const -> const_iterator {
	return end();
}
auto xref_table::rbegin() -> reverse_iterator {
	return m_table.rbegin();
}
auto xref_table::rbegin() const -> const_reverse_iterator {
	return m_table.rbegin();
}
auto xref_table::crbegin() const -> const_reverse_iterator {
	return rbegin();
}
auto xref_table::rend() -> reverse_iterator {
	return m_table.rend();
}
auto xref_table::rend() const -> const_reverse_iterator {
	return m_table.rend();
}
auto xref_table::crend() const -> const_reverse_iterator {
	return rend();
}

auto xref_table::at(key_type n) const -> const_reference {
	return m_table.at(n);
}
// NOTE: currently, generation_number is not used
auto xref_table::at(object_t object_number,
                    generation_t /* generation_number */) const
    -> const_reference {
	return at(object_number);
}
auto xref_table::find(key_type n) -> iterator {
	return m_table.find(n);
}
auto xref_table::find(key_type n) const -> const_iterator {
	return m_table.find(n);
}
// NOTE: currently, generation_number is not used
auto xref_table::find(object_t object_number,
                      generation_t /* generation_number */) -> iterator {
	return m_table.find(object_number);
}
// NOTE: currently, generation_number is not used
auto xref_table::find(object_t object_number,
                      generation_t /* generation_number */) const
    -> const_iterator {
	return m_table.find(object_number);
}
bool xref_table::contains(object_t     object_number,
                          generation_t generation_number) const {
	return find(object_number, generation_number) != end();
}

void xref_table::insert(const value_type& insert_entry) {
	object_t object_number = std::visit(
	    [](const auto& entry) { return entry.object_number; }, insert_entry);
	m_table.emplace(object_number, insert_entry);
}
void xref_table::insert(value_type&& insert_entry) {
	object_t object_number = std::visit(
	    [](const auto& entry) { return entry.object_number; }, insert_entry);
	m_table.emplace(object_number, std::move(insert_entry));
}
auto xref_table::erase(const_iterator erase_position) -> iterator {
	return m_table.erase(erase_position.m_table_it);
}
auto xref_table::erase(const value_type& erase_entry) -> iterator {
	object_t object_number = std::visit(
	    [](const auto& entry) { return entry.object_number; }, erase_entry);

	if (auto entry_it = m_table.find(object_number);
	    entry_it != m_table.end() && erase_entry == entry_it->second) {
		return m_table.erase(entry_it);
	} else {
		return m_table.end();
	}
}
auto xref_table::erase(object_t erase_object_number) -> size_type {
	return m_table.erase(erase_object_number);
}

bool xref_table::operator==(const xref_table& rhs) const {
	return this->m_table == rhs.m_table;
}
bool xref_table::operator!=(const xref_table& rhs) const {
	return !(*this == rhs);
}
#pragma endregion // region xref_table

#pragma region xref_table_const_iterator
xref_table_const_iterator::xref_table_const_iterator(
    xref_table::table_type::const_iterator table_it)
    : m_table_it(table_it) {}

auto xref_table_const_iterator::operator*() const -> reference {
	return m_table_it->second;
}
xref_table_const_iterator& xref_table_const_iterator::operator++() {
	++m_table_it;
	return *this;
}
xref_table_const_iterator xref_table_const_iterator::operator++(int) {
	auto copy_of_this = *this;
	++*this;
	return copy_of_this;
}
xref_table_const_iterator& xref_table_const_iterator::operator--() {
	--m_table_it;
	return *this;
}
xref_table_const_iterator xref_table_const_iterator::operator--(int) {
	auto copy_of_this = *this;
	--*this;
	return copy_of_this;
}
bool xref_table_const_iterator::operator==(
    const xref_table_const_iterator& rhs) const {
	return this->m_table_it == rhs.m_table_it;
}
bool xref_table_const_iterator::operator!=(
    const xref_table_const_iterator& rhs) const {
	return !(*this == rhs);
}
#pragma endregion // region xref_table_const_iterator

#pragma region xref_table_iterator
xref_table_iterator::xref_table_iterator(
    xref_table::table_type::iterator table_it)
    : xref_table_const_iterator(table_it) {}

auto xref_table_iterator::operator*() const -> reference {
	return xref_table_const_iterator::operator*();
}
auto xref_table_iterator::operator++() -> xref_table_iterator& {
	xref_table_const_iterator::operator++();
	return *this;
}
auto xref_table_iterator::operator++(int) -> xref_table_iterator {
	auto copy_of_this = *this;
	++*this;
	return *this;
}
auto xref_table_iterator::operator--() -> xref_table_iterator& {
	xref_table_const_iterator::operator--();
	return *this;
}
auto xref_table_iterator::operator--(int) -> xref_table_iterator {
	auto copy_of_this = *this;
	--*this;
	return *this;
}
bool xref_table_iterator::operator==(const xref_table_iterator& rhs) const {
	return xref_table_const_iterator::operator==(rhs);
}
bool xref_table_iterator::operator!=(const xref_table_iterator& rhs) const {
	return !(*this == rhs);
}
#pragma endregion // region xref_table_iterator
} // namespace pdfparser::xref_types
