#include "PDFParser.object_types.h"

namespace pdfparser::object_types {
#pragma region stream_object
std::string stream_object::get_decoded_data() const {
	// HACK: Filter�ɉ����ăf�R�[�h
	return m_decoded_data;
}
#pragma endregion // region stream_object
} // namespace pdfparser::object_types
