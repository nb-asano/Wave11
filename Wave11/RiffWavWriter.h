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
 * @file	RiffWavWriter.h
 * @brief	RIFF-WAVファイルを書き出すクラスのヘッダー
 */
// ----------------------------------------------------------------------------
#ifndef _RIFFWAVWRITER_H_
#define _RIFFWAVWRITER_H_

#include "BinaryWriter.h"

// ----------------------------------------------------------------------------
/**
 * @brief RIFF-WAVファイルの書き出しクラス
 * このクラスはスレッドセーフではない。
 */
// ----------------------------------------------------------------------------
class RiffWavWriter : public BinaryWriter
{
public:
	RiffWavWriter(WORD, WORD, WORD, bool = true);
	virtual ~RiffWavWriter() {}

	//! ストリーム書き出しの準備を行います。
	bool prepare();
	//! ストリーム書き出しを終了します。
	bool riffFinalize();

private:
	//! 量子化ビット数
	const WORD qbit_;
	//! チャンネル数
	const WORD ch_;
	//! サンプリングレート
	const DWORD fs_;
	//! 量子化フォーマット（真が整数型PCM）
	const bool fmt_;

	RiffWavWriter();
};

#endif // !_RIFFWAVWRITER_H_
