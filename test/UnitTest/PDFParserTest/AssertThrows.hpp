#pragma once

#include "testtool.h"

#include <exception>
#include <typeinfo>

#define AssertThrows(Exception, ...)                                           \
	do {                                                                         \
		using namespace System;                                                    \
		using fail          = AssertFailedException;                               \
		String ^ common_msg = "AssertThrows に失敗しました。"              \
		                      "<" #Exception ">"                                   \
		                      " をスローすべきですが、";                           \
		try {                                                                      \
			__VA_ARGS__;                                                             \
			throw gcnew fail(common_msg + "何もスローしませんでした。");             \
		} catch (Exception&) { /* success */                                       \
		} catch (fail ^) {                                                         \
			throw; /* rethrow */                                                     \
		} catch (const std::exception& e) {                                        \
			throw gcnew fail(common_msg + "<" + gcnew String(typeid(e).name()) +     \
			                 ">"                                                     \
			                 " をスローしました。\n"                        \
			                 "内容: " +                                              \
			                 gcnew String(e.what()));                                \
		} catch (...) {                                                            \
			throw gcnew fail(common_msg + "非標準例外をスローしました。");           \
		}                                                                          \
	} while (0)
