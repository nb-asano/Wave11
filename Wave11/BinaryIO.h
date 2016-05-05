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
 * @file	BinaryIo.h
 * @brief	バイナリ入出力関連のIF
 */
// ----------------------------------------------------------------------------
#ifndef _BINARYIO_H_
#define _BINARYIO_H_

#include "WavIoType.h"
#include <exception>

// ----------------------------------------------------------------------------
/**
 * @brief バイナリ読み書きのIOクラス
 *
 * このクラスから派生したクラスのインスタンスはスレッドセーフではない。
 */
// ----------------------------------------------------------------------------
class BinaryIo
{
public:
	/**
	 * デフォルトでリトルエンディアン
	 */
	BinaryIo() : isLittleEndian_(true) {}
	virtual ~BinaryIo() {}

	/**
	 * @brief	ファイルをオープンする
	 * @param[in] file ファイルのパス
	 * @return	オープンに成功すれば真
	 */
	virtual bool open(const tstring&) = 0;
	//! ファイルのクローズ
	virtual void close() = 0;
	/**
	 * @brief	ファイルの読み書き位置を設定
	 * @param[in] offs		ファイルの読み書き位置
	 * @param[in] origin	fseekの移動モード指定。SEEK_CUR/SEEK_END/SEEK_SETの3種。
	 * @return	移動に成功すれば真。
	 */
	virtual bool seek(long offs, int origin) = 0;
	/**
	 * @brief	ファイルのバイトオフセットを取得
	 * @return	ファイルのバイトオフセット。エラー発生時は-1。
	 */
	virtual long tell() = 0;
	/**
	 * @brief	処理エンディアン設定
	 * @param[in] le	リトルエンディアン指定時は真
	 */
	void setLittleEndian(const bool& le) { isLittleEndian_ = le; }
	/**
	 * @brief	処理エンディアン取得
	 * @return	リトルエンディアンなら真。
	 */
	bool isLittleEndian() const { return isLittleEndian_; }

protected:
	/**
	 * @brief	4byteのバイト列をエンディアンに従って32bit符号なし整数型に変換
	 * @param[in]	b	4バイトのバイト列の先頭のポインタ
	 * @return	変換した32bit符号なし整数値
	 *
	 * @attention	引数のエラーチェック等は一切ない。
	 */
	DWORD toDWORD(const BYTE* b) {
		if (isLittleEndian_) {
			return (static_cast<DWORD>(b[0])
				| (static_cast<DWORD>(b[1]) << 8)
				| (static_cast<DWORD>(b[2]) << 16)
				| (static_cast<DWORD>(b[3]) << 24));
		} else {
			return (static_cast<DWORD>(b[3])
				| (static_cast<DWORD>(b[2]) << 8)
				| (static_cast<DWORD>(b[1]) << 16)
				| (static_cast<DWORD>(b[0]) << 24));
		}
	}
	/**
	 * @brief	2byteのバイト列をエンディアンに従って16bit符号なし整数型に変換
	 * @param[in]	b	2バイトのバイト列の先頭のポインタ
	 * @return	変換した16bit符号なし整数値
	 *
	 * @attention	引数のエラーチェック等は一切ない。
	 */
	WORD toWORD(const BYTE* b) {
		if (isLittleEndian_) {
			return (static_cast<DWORD>(b[0])
				| (static_cast<DWORD>(b[1]) << 8));
		} else {
			return (static_cast<DWORD>(b[1])
				| (static_cast<DWORD>(b[0]) << 8));
		}
	}
	/**
	 * @brief	32bit符号なし整数値をエンディアンに従って4バイトのバイト列に変換
	 * @param[in,out]	b	4バイトのバイト列を格納可能なバッファのポインタ
	 * @param[in]		n	変換する32bit符号なし整数値
	 * @return	4バイトのバイト列
	 *
	 * @attention	引数のエラーチェック等は一切ない。
	 */
	BYTE* toBytes(BYTE* b, DWORD n) {
		if (isLittleEndian_) {
			b[0] = static_cast<BYTE>((n & 0x000000FF));
			b[1] = static_cast<BYTE>((n & 0x0000FF00) >> 8);
			b[2] = static_cast<BYTE>((n & 0x00FF0000) >> 16);
			b[3] = static_cast<BYTE>((n & 0xFF000000) >> 24);
		} else {
			b[3] = static_cast<BYTE>((n & 0x000000FF));
			b[2] = static_cast<BYTE>((n & 0x0000FF00) >> 8);
			b[1] = static_cast<BYTE>((n & 0x00FF0000) >> 16);
			b[0] = static_cast<BYTE>((n & 0xFF000000) >> 24);
		}
		return b;
	}
	/**
	 * @brief	16bit符号なし整数値をエンディアンに従って2バイトのバイト列に変換
	 * @param[in,out]	b	2バイトのバイト列を格納可能なバッファのポインタ
	 * @param[in]		n	変換する16bit符号なし整数値
	 * @return	2バイトのバイト列
	 *
	 * @attention	引数のエラーチェック等は一切ない。
	 */
	BYTE* toBytes(BYTE* b, WORD n)
	{
		if (isLittleEndian_) {
			b[0] = static_cast<BYTE>((n & 0x00FF));
			b[1] = static_cast<BYTE>((n & 0xFF00) >> 8);
		} else {
			b[1] = static_cast<BYTE>((n & 0x00FF));
			b[0] = static_cast<BYTE>((n & 0xFF00) >> 8);
		}
		return b;
	}

private:
	//! バイト列の処理エンディアン
	bool isLittleEndian_;
};

#endif // !_BINARYIO_H_
