#pragma once

#include "pdfparser.istream_extended_errors.hpp"
#include "pdfparser.whitespace_flags.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <ios>
#include <istream>
#include <optional>
#include <string_view>
#include <vector>

namespace pdfparser {
class istream_extended: public std::istream {
public:
	/// <summary>peek next character</summary>
	/// <returns>
	/// std::nullopt for the end of file, otherwise the next character
	/// </returns>
	[[nodiscard]] std::optional<char> peek();

	/// <summary>get next character</summary>
	/// <returns>
	/// next character
	/// </returns>
	[[nodiscard]] char get();

	/// <summary>check whether end of file or not</summary>
	/// <returns>true if and only if stream is on EOF</returns>
	[[nodiscard]] bool eof();

	/// <summary>get stream length</summary>
	/// <returns>stream length</returns>
	[[nodiscard]] std::streamsize length();

	/// <summary>
	/// get current stream position (byte offset from beginning of file)
	/// </summary>
	/// <returns>current byte offset from beginning of file</returns>
	[[nodiscard]] std::streamoff tell() const;

	/// <summary>
	/// move stream position to byte_offset_from_beginning_of_file
	/// </summary>
	/// <param name="byte_offset">byte offset from beginning of file</param>
	void seek(std::streamoff byte_offset);

	/// <summary>seek to end of stream</summary>
	void seek_to_end();

	/// <summary>
	/// If the current position is the beginning of a line,
	///  seek to the beginning of the previous line;
	/// otherwise, seek to the beginning of the current line.
	/// </summary>
	void seek_forward_head_of_line();

	/// <summary>advance the stream by one character</summary>
	istream_extended& operator++();

	/// <summary>
	/// ignore whitespaces specified by flags if they are present on m_stream.
	/// </summary>
	/// <param name="flags">
	/// bit flags to specify which whitespace characters to ignore
	/// </param>
	void ignore_if_present(whitespace_flags flags);

	/// <summary>
	/// consume attempt_str at the current position of the stream if it's present;
	/// otherwise, do nothing.
	/// </summary>
	/// <param name="attempt_str">string trying to consume</param>
	/// <returns>true if and only if consumed</returns>
	bool attempt(std::string_view attempt_str);

	/// <summary>
	/// Promise any string in the promise_list to be able to attempt.
	/// If none of the strings can be attempted, it throws
	/// istreram_extended_error(promised_failed).
	/// </summary>
	/// <param name="promise_list">
	/// list of strings which one of them is promised to be able to attempt
	/// </param>
	void promise(std::initializer_list<std::string_view> promise_list);

public:
	/// <summary>
	/// construct with stream buffer
	/// </summary>
	/// <param name="sb">buffer</param>
	explicit istream_extended(std::streambuf* sb);

	virtual ~istream_extended() = default;
};
} // namespace pdfparser
