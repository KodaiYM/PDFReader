#pragma once

#include "testtool.h"

#include <exception>
#include <typeinfo>

#define AssertThrows(Exception, ...)                                           \
	do {                                                                         \
		using namespace System;                                                    \
		using fail          = AssertFailedException;                               \
		String ^ common_msg = "AssertThrows �Ɏ��s���܂����B"              \
		                      "<" #Exception ">"                                   \
		                      " ���X���[���ׂ��ł����A";                           \
		try {                                                                      \
			__VA_ARGS__;                                                             \
			throw gcnew fail(common_msg + "�����X���[���܂���ł����B");             \
		} catch (Exception&) { /* success */                                       \
		} catch (fail ^) {                                                         \
			throw; /* rethrow */                                                     \
		} catch (const std::exception& e) {                                        \
			throw gcnew fail(common_msg + "<" + gcnew String(typeid(e).name()) +     \
			                 ">"                                                     \
			                 " ���X���[���܂����B\n"                        \
			                 "���e: " +                                              \
			                 gcnew String(e.what()));                                \
		} catch (...) {                                                            \
			throw gcnew fail(common_msg + "��W����O���X���[���܂����B");           \
		}                                                                          \
	} while (0)
