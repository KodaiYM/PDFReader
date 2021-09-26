#pragma once

#include "PDFReader.PDFContents.hpp"
#include "PDFReader.PDFPage.hpp"
#include "pdfparser.pdfcontents_builder.hpp"
#include "pdfparser.space.hpp"

#include <gcroot.h>

namespace pdfparser {
class pdfpage_builder {
public:
	[[nodiscard]] PDFReader::PDFPage ^ to_pdfpage() const noexcept;
	void set_media_box(rectangle_data media_box) noexcept;
	void set_contents(PDFReader::PDFContents ^ page_content) noexcept;

private:
	gcroot<PDFReader::PDFPage ^> m_page;
};
} // namespace pdfparser
