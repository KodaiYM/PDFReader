#include "pdfparser.pdfcontents_builder.hpp"
#include "pdfparser.space.hpp"

using namespace pdfparser;

PDFReader::PDFContents ^ pdfparser::pdfcontents_builder::build() {
	return gcnew PDFReader::PDFContents;
}

void pdfcontents_builder::concat_CTM(const AffineMatrix& concat_matrix) {
	m_graphics_states.top().current_transformation_matrix *= concat_matrix;
}
void pdfcontents_builder::set_line_dash_pattern(
    line_style::dash_pattern_type line_dash_pattern) noexcept {
	m_graphics_states.top().line_dash_pattern = line_dash_pattern;
}
void pdfcontents_builder::set_flatness(double flatness) noexcept {
	m_graphics_states.top().flatness = flatness;
}
void pdfcontents_builder::set_line_join_style(
    line_style::join_type line_join_style) noexcept {
	m_graphics_states.top().line_join_style = line_join_style;
}
void pdfcontents_builder::set_line_cap_style(
    line_style::cap_type line_cap_style) noexcept {
	m_graphics_states.top().line_cap_style = line_cap_style;
}
void pdfcontents_builder::set_miter_limit(double miter_limit) noexcept {
	m_graphics_states.top().miter_limit = miter_limit;
}
void pdfcontents_builder::set_line_width(double line_width) noexcept {
	m_graphics_states.top().line_width = line_width;
}
void pdfcontents_builder::set_fill_color(chroma::color fill_color) noexcept {
	m_graphics_states.top().fill_color = fill_color;
}
void pdfcontents_builder::set_stroke_color(
    chroma::color stroke_color) noexcept {
	m_graphics_states.top().stroke_color = stroke_color;
}
void pdfcontents_builder::set_character_spacing(
    double character_spacing) noexcept {
	m_graphics_states.top().character_spacing = character_spacing;
}
void pdfcontents_builder::set_leading(double leading) noexcept {
	m_graphics_states.top().leading = leading;
}
void pdfcontents_builder::set_text_rendering_mode(
    text_style::rendering_mode text_rendering_mode) noexcept {
	m_graphics_states.top().text_rendering_mode = text_rendering_mode;
}
void pdfcontents_builder::set_text_rise(double text_rise) noexcept {
	m_graphics_states.top().text_rise = text_rise;
}
void pdfcontents_builder::set_word_spacing(double word_spacing) noexcept {
	m_graphics_states.top().word_spacing = word_spacing;
}
void pdfcontents_builder::set_horizontal_scaling(
    double horizontal_scaling) noexcept {
	m_graphics_states.top().horizontal_scaling = horizontal_scaling;
}

void pdfcontents_builder::set_text_matrices(double next_line_x,
                                            double next_line_y) noexcept {
	m_text_matrix = m_text_line_matrix =
	    AffineMatrix({1, 0, 0, 1, next_line_x, next_line_y});
}
void pdfcontents_builder::concat_text_matrices(
    double next_line_offset_x, double next_line_offset_y) noexcept {
	m_text_matrix = m_text_line_matrix =
	    AffineMatrix({1, 0, 0, 1, next_line_offset_x, next_line_offset_y}) *
	    m_text_line_matrix;
}
void pdfcontents_builder::move_to_next_line() {
	concat_text_matrices(0, -m_graphics_states.top().leading);
}
void pdfcontents_builder::begin_text() {
	m_text_matrix = m_text_line_matrix = AffineMatrix::Identity();
}
void pdfcontents_builder::end_text() noexcept {}
