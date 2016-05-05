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
 * @file	BinaryWriter.h
 * @brief	バイナリ書き込みクラスのヘッダー
 */
// ----------------------------------------------------------------------------
#ifndef _BINARYWRITER_H_
#define _BINARYWRITER_H_

#include <cstdio>
#include "BinaryIO.h"
#include "Noncopyable.h"

// ----------------------------------------------------------------------------
/**
 * @brief バイナリ書き出しクラス
 * 書き出し対象はファイルのみ
 * @attention 2GB以上のファイル書き出しは正常動作しない。
 */
// ----------------------------------------------------------------------------
class BinaryWriter : public BinaryIo, private Noncopyable
{
public:
	BinaryWriter() : BinaryIo(), fp_(nullptr) {}
	/** デストラクタでファイルは自動クローズする */
	virtual ~BinaryWriter() { close(); }

	/**
	 * @brief	書き出し対象のファイルをオープンする。書き出しは常に新規ファイル。
	 * @param[in] path ファイルのパス
	 * @return	オープンに成功すれば真
	 */
	bool open(const tstring& path) {
		if ((fp_ = ::_tfopen(path.c_str(), _T("wb"))) == nullptr) {
			return false;
		}
		return true;
	}
	//! ファイルのクローズ
	void close() {
		if (fp_) {
			::fclose(fp_);
			fp_ = nullptr;
		}
	}
	/**
	 * @brief	指定されたバイト数のデータを書き出す
	 * @param[in] buf		書き出しデータバッファ
	 * @param[in] size		書き出しデータのバイトサイズ
	 * @return	実際に書き出したバイトサイズ
	 * @exception	WavIoException	ファイル未オープン
	 */
	size_t writeBytes(const void* buf, size_t size) throw(WavIoException) {
		if (buf == nullptr || size == 0) return 0;
		if (fp_ == nullptr) throw WavIoException("file isn't opened.");
		return ::fwrite(buf, 1, size, fp_);
	}
	/**
	 * @brief	ファイルの書き出し位置を設定
	 * @param[in] offs		ファイルの読み書き位置
	 * @param[in] origin	fseekの移動モード指定。SEEK_CUR/SEEK_END/SEEK_SETの3種。
	 * @return	移動に成功すれば真。
	 */
	bool seek(long offs, int origin) {
		return (fp_ == nullptr) ? false : (::fseek(fp_, offs, origin) == 0);
	}
	/**
	 * @brief	ファイルのバイトオフセットを取得
	 * @return	ファイルのバイトオフセット。エラー発生時は-1。
	 */
	long tell() {
		return (fp_ == nullptr) ? -1 : ::ftell(fp_);
	}
	/**
	 * @brief	32bit長のデータを32bit符号なし整数値として書き出す。
	 * データの書き出しができない場合は例外を投入する。
	 * @exception	WavIoException	書き出しエラー発生
	 */
	void writeDWORD(DWORD n) throw (WavIoException) {
		BYTE buf[4];
		BYTE* p = toBytes(buf, n);
		if (writeBytes(p, 4) != 4) {
			throw WavIoException("stdio write error.");
		}
	}
	/**
	 * @brief	16bit長のデータを16bit符号なし整数値として書き出す。
	 * データの書き出しができない場合は例外を投入する。
	 * @exception	WavIoException	書き出しエラー発生
	 */
	void writeWORD(WORD n) throw (WavIoException) {
		BYTE buf[2];
		BYTE* p = toBytes(buf, n);
		if (writeBytes(p, 2) != 2) {
			throw WavIoException("stdio write error.");
		}
	}
	/**
	* @brief	8bit長のデータを8bit符号なし整数値として書き出す。
	* データの書き出しができない場合は例外を投入する。
	* @exception	WavIoException	書き出しエラー発生
	*/
	void writeBYTE(BYTE b) throw (WavIoException) {
		if (writeBytes(&b, 1) != 1) {
			throw WavIoException("stdio write error.");
		}
	}

private:
	//! 処理対象ファイルポインタ
	FILE*  fp_;
};

#endif // !_BINARYREADER_H_
