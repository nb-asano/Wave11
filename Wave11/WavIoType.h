/**
* Copyright (c) 2012-2016 Sakura-Zen soft All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
// ----------------------------------------------------------------------------
/**
 * @file	WavIoType.h
 * @brief	共通ヘッダー
 */
// ----------------------------------------------------------------------------
#ifndef _WAVIOTYPE_H_
#define _WAVIOTYPE_H_

#include <string>
#include <sstream>
#include <exception>

#if defined(_WIN32) && defined(_MSC_VER)
#include <Windows.h>
#include <tchar.h>

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ifstream<TCHAR> tifstream;
typedef std::basic_istringstream<TCHAR> itstringstream;
typedef std::basic_ostringstream<TCHAR> otstringstream;

#else

typedef char TCHAR;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef std::basic_string<char> tstring;
typedef std::basic_ifstream<char> tifstream;
typedef std::basic_istringstream<char> itstringstream;
typedef std::basic_ostringstream<char> otstringstream;

#define _T
#define _tfopen fopen

#endif

// ----------------------------------------------------------------------------
/**
 * @brief ストリーム読み込みの例外クラス
 */
// ----------------------------------------------------------------------------
class WavIoException : public std::runtime_error
{
public:
	/**
	 * @param[in]	what_arg	例外メッセージ
	 */
	explicit WavIoException(const std::string& what_arg) : runtime_error(what_arg.c_str()) {}
	WavIoException() : runtime_error("WavIoException") {}
	virtual ~WavIoException(void) {}
};


#endif // !_WAVIOTYPE_H_
