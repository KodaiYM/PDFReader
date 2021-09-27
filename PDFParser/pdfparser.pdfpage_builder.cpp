#include "pdfparser.pdfpage_builder.hpp"

using namespace pdfparser;

pdfpage_builder::pdfpage_builder() {
	m_page = gcnew PDFReader::PDFPage;
}

[[nodiscard]] PDFReader::PDFPage ^
    pdfpage_builder::to_pdfpage() const noexcept {
	return m_page;
}

void pdfpage_builder::set_media_box(rectangle_data media_box) noexcept {
	// NOTE: ‰¼ŽÀ‘•
	m_page->Width =
	    media_box.greater_coordinates.x - media_box.less_coordinates.x + 1;
	m_page->Height =
	    media_box.greater_coordinates.y - media_box.less_coordinates.y + 1;
}
void pdfpage_builder::set_contents(PDFReader::PDFContents ^
                                   page_content) noexcept {
	// NOTE: –¢ŽÀ‘•
}
