#pragma once

namespace PDFParser {
public
ref struct PDFPage: System::IEquatable<PDFPage ^> {
	property double Width;
	property double Height;

	bool Equals(Object ^ obj) override {
		return this->Equals(static_cast<PDFPage ^>(obj));
	}
	virtual bool Equals(PDFPage ^ other) {
		return this == other;
	}
	bool operator==(PDFPage ^ other) {
		return Width == other->Width && Height == other->Height;
	}
	bool operator!=(PDFPage ^ other) {
		return !(this == other);
	}
};
} // namespace PDFParser
