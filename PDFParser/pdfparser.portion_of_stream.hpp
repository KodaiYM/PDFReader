#pragma once

#include <ios>
#include <optional>

namespace pdfparser {
// HACK:
// �X�g���[����̈ʒu���������̂�t����ꍇ�́A�X�g���[�����̂ƌ��т��Ė����Ɩ������n�߂�̂ŁA
// �����̎d�g�݂��l����ׂ�
// �Ǝv���Ă����ǁA�����̃X�g���[���𓯎��Ɏg�����Ƃ͂���܂�Ȃ����當������l����΂������c
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
