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
 * @file	RiffWavWriter.cpp
 * @brief	RIFF-WAVファイルを書き出すクラスの実装
 */
// ----------------------------------------------------------------------------
#include "RiffWavWriter.h"

// ----------------------------------------------------------------------------
/**
 * フレーム単位でストリームを読み込みます。
 *
 * @param[in]	qbit	データを格納する十分なサイズのバッファのポインタ。
 * @param[in]	ch		チャンネル数
 * @param[in]	ch		サンプリングレート
 * @param[in]	ch		WAVE_FORMAT_PCMなら真、WAVE_FORMAT_IEEE_FLOATなら偽
 */
// ----------------------------------------------------------------------------
RiffWavWriter::RiffWavWriter(WORD qbit, WORD ch, WORD fs, bool fmt)
: BinaryWriter(),
  qbit_(qbit),
  ch_(ch),
  fs_(fs),
  fmt_(fmt)
{
}
// ----------------------------------------------------------------------------
// ストリーム書き出しの準備を行います。
/**
 * RIFF-WAVのヘッダーファイルを書き出しします。\n
 * ストリームの書き出し前に実行する必要があります。
 *
 * return	書き出しに成功すれば真
 */
// ----------------------------------------------------------------------------
bool RiffWavWriter::prepare()
{
	const char header[] = {
		'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ',
		0x10, 0, 0, 0
	};
	const char headerInt[] = {
		1, 0
	};
	const char headerFloat[] = {
		3, 0
	};
	const char dataHeader[] = {
		'd', 'a', 't', 'a'
	};

	try {
		size_t wret = this->writeBytes(header, 20);
		if (wret != 20) {
			return false;
		}

		wret = this->writeBytes((fmt_ ? headerInt : headerFloat), 2);
		if (wret != 2) {
			return false;
		}

		this->writeWORD(ch_);
		this->writeDWORD(fs_);
		int datav = fs_ * qbit_ / 8 * ch_;
		this->writeDWORD(datav);
		short dwBlock = qbit_ / 8 * ch_;
		this->writeWORD(dwBlock);
		this->writeWORD(qbit_);

		wret = this->writeBytes(dataHeader, 4);
		if (wret != 4) {
			return false;
		}

		this->writeDWORD(0);
	} catch (const WavIoException&) {
		return false;
	}

	return true;
}
// ----------------------------------------------------------------------------
// ストリーム書き出しを終了します。
/**
* RIFF-WAVの書き出しを終了します。\n
* この関数呼び出しが完了するとRIFF-WAVヘッダーにファイルサイズが書き込まれた状態に
* なります。
*
* return	正常終了で真
*/
// ----------------------------------------------------------------------------
bool RiffWavWriter::riffFinalize()
{
	// 終端に移動してファイルサイズ取得
	if (!this->seek(0, SEEK_END)) {
		return false;
	}
	long fileSize = this->tell();

	try {
		// 全体サイズ書き出し
		if (!this->seek(4, SEEK_SET)) {
			return false;
		}
		fileSize -= 8;	// チャンクヘッダー分減らす
		this->writeDWORD(fileSize);

		// ストリームサイズ書き出し
		if (!this->seek(40, SEEK_SET)) {
			return false;
		}
		fileSize -= 36;
		this->writeDWORD(fileSize);
	} catch (const WavIoException&) {
		return false;
	}
	return true;
}
