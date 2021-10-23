#pragma once

#include <ios>

namespace pdfparser {
class portion_of_stream {
public:
	/// <summary>
	/// get position of this object
	/// </summary>
	/// <returns>reference to position of this object</returns>
	inline const std::streampos& position() const& noexcept;

	/// <summary>
	/// get position of this object
	/// </summary>
	/// <returns>copy of position of this object</returns>
	inline std::streampos position() && noexcept;

public:
	inline explicit portion_of_stream(std::streampos position) noexcept;
	inline virtual ~portion_of_stream() noexcept = 0;

private:
	std::streampos m_position;
};
} // namespace pdfparser

#include "pdfparser.portion_of_stream.ipp"
