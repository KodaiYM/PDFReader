#pragma once

#include <initializer_list>
#include <ios>
#include <iterator>
#include <map>
#include <tuple>
#include <variant>

/************************
  Cross Reference Types
 ************************/
namespace pdfparser { namespace xref_types {
	using object_t     = uint32_t; // object number type
	using generation_t = uint16_t; // generation number type

	struct xref_inuse_entry final {
		object_t       object_number;
		generation_t   generation_number;
		std::streamoff byte_offset;

		bool operator==(const xref_inuse_entry& rhs) const {
			return std::tie(this->object_number, this->generation_number,
			                this->byte_offset) == std::tie(rhs.object_number,
			                                               rhs.generation_number,
			                                               rhs.byte_offset);
		}
		bool operator!=(const xref_inuse_entry& rhs) const {
			return !(*this == rhs);
		}
	};
	struct xref_free_entry final {
		object_t     object_number;
		generation_t next_used_generation_number;
		object_t     next_free_object_number;

		bool operator==(const xref_free_entry& rhs) const {
			return std::tie(this->object_number, this->next_used_generation_number,
			                this->next_free_object_number) ==
			       std::tie(rhs.object_number, rhs.next_used_generation_number,
			                rhs.next_free_object_number);
		}
		bool operator!=(const xref_free_entry& rhs) const {
			return !(*this == rhs);
		}
	};
	using xref_entry = std::variant<xref_inuse_entry, xref_free_entry>;

	class xref_table_iterator;
	class xref_table_const_iterator;
	class xref_table final {
	public:
		using key_type        = object_t;
		using value_type      = xref_entry;
		using reference       = value_type&;
		using const_reference = const value_type&;
		using size_type       = std::size_t;
		using difference_type = std::ptrdiff_t;
		using iterator =
		    xref_table_iterator; // é¿ç€ÇÃèä const ÇæÇ™ÅAstd::set Ç…ïÌÇ¡ÇƒçÏÇÈ
		using const_iterator         = xref_table_const_iterator;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		iterator               begin();
		const_iterator         begin() const;
		const_iterator         cbegin() const;
		iterator               end();
		const_iterator         end() const;
		const_iterator         cend() const;
		reverse_iterator       rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;
		reverse_iterator       rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		const_reference at(key_type n) const;

		void      insert(const value_type& insert_entry);
		void      insert(value_type&& insert_entry);
		iterator  erase(const_iterator erase_position);
		iterator  erase(const value_type& erase_entry);
		size_type erase(object_t erase_object_number);

	public:
		bool operator==(const xref_table& rhs) const;
		bool operator!=(const xref_table& rhs) const;

	public:
		xref_table() = default;
		xref_table(std::initializer_list<value_type> entry_list);

	private:
		using table_type = std::map<key_type, value_type>;
		table_type m_table;

		friend class iterator;
		friend class const_iterator;
	};

	class xref_table_const_iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type        = xref_entry;
		using difference_type   = std::ptrdiff_t;
		using pointer           = const value_type*;
		using reference         = const value_type&;

	public:
		reference                  operator*() const;
		xref_table_const_iterator& operator++();
		xref_table_const_iterator  operator++(int);
		xref_table_const_iterator& operator--();
		xref_table_const_iterator  operator--(int);
		bool operator==(const xref_table_const_iterator& rhs) const;
		bool operator!=(const xref_table_const_iterator& rhs) const;

	public:
		xref_table_const_iterator(xref_table::table_type::const_iterator table_it);

	private:
		xref_table::table_type::const_iterator m_table_it;

		friend xref_table;
	};
	class xref_table_iterator: public xref_table_const_iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type        = xref_entry;
		using difference_type   = std::ptrdiff_t;
		using pointer           = const value_type*;
		using reference         = const value_type&;

	public:
		reference operator*() const;
		xref_table_iterator& operator++();
		xref_table_iterator  operator++(int);
		xref_table_iterator& operator--();
		xref_table_iterator  operator--(int);
		bool                 operator==(const xref_table_iterator& rhs) const;
		bool                 operator!=(const xref_table_iterator& rhs) const;

	public:
		using xref_table_const_iterator::xref_table_const_iterator;
		xref_table_iterator(xref_table::table_type::iterator table_it);
	};
}} // namespace pdfparser::xref_types
