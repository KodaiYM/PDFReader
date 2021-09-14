#include "pdfparser.object_types.hpp"

namespace pdfparser::object_types {
#pragma region number_object
double pdfparser::object_types::number_to_double(
    const number_object& number_obj) noexcept {
	return std::visit(
	    [](const auto& concrete_number) noexcept -> double {
		    using T = std::decay_t<decltype(concrete_number)>;
		    if constexpr (std::is_same_v<integer_object, T>) {
			    return static_cast<double>(
			        static_cast<integer_object::int_type>(concrete_number));
		    } else if constexpr (std::is_same_v<real_object, T>) {
			    return static_cast<double>(concrete_number);
		    } else {
			    static_assert(false);
		    }
	    },
	    number_obj);
}
#pragma endregion // region number_object

#pragma region stream_object
std::string stream_object::get_decoded_data() const {
	// HACK: Filterに応じてデコード
	return m_decoded_data;
}
#pragma endregion // region stream_object
} // namespace pdfparser::object_types
