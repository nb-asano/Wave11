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
 * @file	RiffWavReader.h
 * @brief	RIFF-WAVファイルを読み込むクラスのヘッダー
 */
// ----------------------------------------------------------------------------
#ifndef _RIFFWAVREADER_H_
#define _RIFFWAVREADER_H_

#include <cstring>
#include "BinaryReader.h"

#if !(defined(_MSC_VER) && defined(_WAVEFORMATEX_))
// ----------------------------------------------------------------------------
/**
 * @brief WAVEFORMATEX構造体
 * いわゆるWAVEFORMATEXで、外部で定義されていることもある。
 */
// ----------------------------------------------------------------------------
typedef struct {
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	WORD cbSize;
} WAVEFORMATEX;
#endif

// ----------------------------------------------------------------------------
/**
 * @brief RIFF-WAVファイルの読み込みクラス
 * このクラスはスレッドセーフではない。
 */
// ----------------------------------------------------------------------------
class RiffWavReader : public BinaryReader
{
public:
	RiffWavReader() : BinaryReader(), hdr_(), streamOffset_(0), streamLength_(0) {
		::memset(&hdr_, 0, sizeof(hdr_));
	}
	virtual ~RiffWavReader() {}

	//! ストリーム読み込みの準備を行います。
	bool prepare();
	/**
	 * @brief	FormatTagを取得する
	 * @return	WAVEFORMATEXに含まれるFormatTagの値
	 */
	WORD getFormatTag() const { return hdr_.wFormatTag; }
	/**
	 * @brief	チャンネル数を取得する
	 * @return	WAVEFORMATEXに含まれるチャンネル数の値
	 */
	WORD getChannels() const { return hdr_.nChannels; }
	/**
	 * @brief	サンプリングレート数を取得する
	 * @return	WAVEFORMATEXに含まれるnSamplesPerSecの値
	 */
	DWORD getSamplesPerSec() const { return hdr_.nSamplesPerSec; }
	/**
	 * @brief	オーディオフレームサイズを取得する
	 * @return	WAVEFORMATEXに含まれるnBlockAlignの値
	 */
	WORD getBlockAlign() const { return hdr_.nBlockAlign; }
	/**
	 * @brief	量子化ビット数を取得する
	 * @return	WAVEFORMATEXに含まれる量子化ビット数の値
	 */
	WORD getBitPerSample() const { return hdr_.wBitsPerSample; }
	/**
	 * @brief	読み込み可能なストリーム長を取得する
	 * @return	実際に読み込み可能なストリームのバイトサイズ
	 */
	DWORD getLength() const { return streamLength_; }
	
	//! フレーム単位でストリーム読み込みを行います
	int getSamples(void*, const size_t&);
	//! バイト単位でのストリーム読み込みを行います
	int getStream(void*, const size_t&);
	//! バイト単位でのストリーム読み込みを行います
	int getStream(void*, const size_t&, size_t&);

private:
	//! WAVEFORMATEXヘッダー
	WAVEFORMATEX hdr_;
	//! ストリーム開始バイトオフセット
	long streamOffset_;
	//! 実際のストリームのバイトサイズ
	DWORD streamLength_;

	/**
	 * @brief	有効RIFF-WAV判定
	 * @return	有効なRIFF-WAVファイルなら真
	 */
	bool isRiffWav() const {
		// 浮動小数点の場合
		if (hdr_.wFormatTag == 3 && hdr_.wBitsPerSample / 8 == 4) {
			return (hdr_.nBlockAlign == (hdr_.wBitsPerSample / 8 * hdr_.nChannels)	// フレームサイズ
				&& hdr_.nAvgBytesPerSec == (hdr_.nSamplesPerSec * hdr_.nBlockAlign));	// データレート
		}
		return (hdr_.wFormatTag == 1	// ストリーム形式
			&& hdr_.nBlockAlign == (hdr_.wBitsPerSample / 8 * hdr_.nChannels)	// フレームサイズ
			&& hdr_.nAvgBytesPerSec == (hdr_.nSamplesPerSec * hdr_.nBlockAlign));	// データレート
	}
	/**
	 * @brief	ストリーム終端判定
	 * @return	読み込み位置がストリーム終端に到達していれば真
	 */
	bool isEnd() {
		return (this->tell() >= streamOffset_ + static_cast<long>(streamLength_));
	}
};

#endif // !_RIFFWAVREADER_H_
