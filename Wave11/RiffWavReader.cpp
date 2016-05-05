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
 * @file	RiffWavReader.cpp
 * @brief	RIFF-WAVファイルを読み込むクラスの実装
 */
// ----------------------------------------------------------------------------
#include "RiffWavReader.h"

// ----------------------------------------------------------------------------
// ストリーム読み込みの準備を行います。
/**
 * ヘッダー部分をパースし、読み込み位置をストリーム先頭に合わせます
 * return	正常終了で真。RIFF-WAV出ない場合は偽となる。
 */
// ----------------------------------------------------------------------------
bool RiffWavReader::prepare()
{
	// ファイルサイズ取得
	this->seek(0, SEEK_END);
	long fileEnd = this->tell();
	if (fileEnd < 0) {
		return false;
	}
	this->seek(0, SEEK_SET);

	try {
		char buf[5] = { 0 };
		size_t size = this->readBytes(buf, 4);
		if (size != 4 || strncmp(buf, "RIFF", 4) != 0) {
			return false;
		}
		
		// 全体サイズは読み込みサイズで判定するので無視
		if (!this->seek(4, SEEK_CUR)) {
			return false;
		}

		size = this->readBytes(buf, 4);
		if (size != 4 || strncmp(buf, "WAVE", 4) != 0) {
			return false;
		}

		// WAVEチャンク内にfmtチャンクが見つかるまでチャンクを読み捨てる
		while (1) {
			::memset(buf, 0, sizeof(buf));
			size = this->readBytes(buf, 4);
			if (size != 4) {
				return false;
			}
			if (strncmp(buf, "fmt ", 4) == 0) {
				break;
			} else {
				DWORD skipsize = this->readDWORD();
				this->seek(skipsize, SEEK_CUR);
			}
		}

		// ヘッダー読み込み
		DWORD cksize = this->readDWORD();
		hdr_.wFormatTag = this->readWORD();
		hdr_.nChannels = this->readWORD();
		hdr_.nSamplesPerSec = this->readDWORD();
		hdr_.nAvgBytesPerSec = this->readDWORD();
		hdr_.nBlockAlign = this->readWORD();
		hdr_.wBitsPerSample = this->readWORD();

		// 有効PCM判定
		if (!isRiffWav()) {
			return false;
		}

		// 不要データの読み飛ばし
		if (cksize > 16) {
			if (!this->seek(cksize - 16, SEEK_CUR)) {
				return false;
			}
		}

		// ストリーム開始位置確認
		::memset(buf, 0, sizeof(buf));
		size = this->readBytes(buf, 4);
		if (size != 4 || strncmp(buf, "data", 4) != 0) {
			return false;
		}

		// 有効ストリームサイズ取得
		cksize = this->readDWORD();
		streamOffset_ = this->tell();
		if (streamOffset_ + static_cast<long>(cksize) > fileEnd) {
			streamLength_ = fileEnd - streamOffset_;
		} else {
			streamLength_ = static_cast<long>(cksize);
		}
	} catch (const WavIoException&) {
		return false;
	}

	return true;
}	
// ----------------------------------------------------------------------------
// フレーム単位でストリーム読み込みを行います
/**
 * フレーム単位でストリームを読み込みます。
 *
 * @param[in]	buf		データを格納する十分なサイズのバッファのポインタ。
 * @param[in]	count	読み取るフレーム数
 *
 * retval	0	正常終了
 * retval	1	ファイル終端到達
 * retval	-1	RIFF-WAVファイルではない
 * retval	-2	引数異常
 * retval	-3	読み込みエラー
 */
// ----------------------------------------------------------------------------
int RiffWavReader::getSamples(void* buf, const size_t& count)
{
	return getStream(buf, count * getBlockAlign());
}
// ----------------------------------------------------------------------------
// バイト単位でのストリーム読み込みを行います
/**
 * バイト単位でストリームを読み込みます。
 *
 * @param[in]	buf		データを格納する十分なサイズのバッファのポインタ。
 * @param[in]	count	読み取るバイト数
 *
 * retval	0	正常終了
 * retval	1	ファイル終端到達
 * retval	-1	RIFF-WAVファイルではない
 * retval	-2	引数異常
 * retval	-3	読み込みエラー
 */
// ----------------------------------------------------------------------------
int RiffWavReader::getStream(void* buf, const size_t& count)
{
	size_t a;
	return getStream(buf, count, a);
}
// ----------------------------------------------------------------------------
// バイト単位でのストリーム読み込みを行います
/**
 * バイト単位でストリームを読み込みます。
 *
 * @param[in]	buf		データを格納する十分なサイズのバッファのポインタ。
 * @param[in]	count	読み取るバイト数
 * @param[out]	result	実際に読みだしたバイト数
 *
 * retval	0	正常終了
 * retval	1	ファイル終端到達
 * retval	-1	RIFF-WAVファイルではない
 * retval	-2	引数異常
 * retval	-3	読み込みエラー
 */
// ----------------------------------------------------------------------------
int RiffWavReader::getStream(void* buf, const size_t& count, size_t& result)
{
	if (!isRiffWav()) return -1;
	if (count == 0) return 0;
	if (buf == nullptr) return -2;
	if (isEnd()) return -3;

	try {
		result = this->readBytes(buf, count);
	} catch (const WavIoException&) {
		return -3;
	}
	return isEnd() ? 1 : 0;
}
