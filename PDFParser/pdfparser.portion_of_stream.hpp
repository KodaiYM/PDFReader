#pragma once

#include <ios>
#include <optional>

namespace pdfparser {
// HACK:
// ストリーム上の位置を示すものを付ける場合は、ストリーム自体と結びついて無いと矛盾し始めるので、
// そこの仕組みを考えるべし
// と思ってたけど、複数のストリームを同時に使うことはあんまりないから文脈から考えればいいか…
class portion_of_stream {
public:
	inline const std::streampos& position() const& noexcept;
	inline std::streampos        position() && noexcept;

public:
	inline explicit portion_of_stream(std::streampos position) noexcept;
	inline virtual ~portion_of_stream() noexcept = 0;

private:
	std::streampos m_position;
};
} // namespace pdfparser

#include "pdfparser.portion_of_stream.ipp"
