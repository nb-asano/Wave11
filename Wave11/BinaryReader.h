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
 * @file	BinaryReader.h
 * @brief	バイナリ読み込みクラスのヘッダー
 */
// ----------------------------------------------------------------------------
#ifndef _BINARYREADER_H_
#define _BINARYREADER_H_

#include <cstdio>
#include "BinaryIO.h"
#include "Noncopyable.h"

// ----------------------------------------------------------------------------
/**
 * @brief バイナリ読み込みクラス
 * 読み込み対象はファイルのみ
 * @attention 2GB以上のファイル読み込みは正常動作しない。
 */
// ----------------------------------------------------------------------------
class BinaryReader : public BinaryIo, private Noncopyable
{
public:
	BinaryReader() : BinaryIo(), fp_(nullptr) {}
	/** デストラクタでファイルは自動クローズする */
	virtual ~BinaryReader() { close(); }

	/**
	 * @brief	読み込み対象のファイルをオープンする
	 * @param[in] path ファイルのパス
	 * @return	オープンに成功すれば真
	 */
	bool open(const tstring& path) {
		if ((fp_ = ::_tfopen(path.c_str(), _T("rb"))) == nullptr) {
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
	 * @brief	指定されたバイト数のデータを読み込む
	 * @param[in] buf		読み込んだデータを格納するバッファ
	 * @param[in] size		読み込むバイトサイズ
	 * @return	実際に読み込んだバイトサイズ
	 * @exception	WavIoException	ファイル未オープン
	 */
	size_t readBytes(void* buf, size_t size) throw(WavIoException) {
		if (buf == nullptr || size == 0) return 0;
		if (fp_ == nullptr) throw WavIoException("file isn't opened.");
		return ::fread(buf, 1, size, fp_);
	}
	/**
	 * @brief	ファイルの読み書き位置を設定
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
	 * @brief	32bit長のデータを32bit符号なし整数値として読み込む。
	 * データの読み込みができない場合は例外を投入する。
	 * @return	読み取った32bit符号なし整数
	 * @exception	WavIoException	読み込みエラー発生
	 */
	DWORD readDWORD() throw (WavIoException) {
		BYTE buf[4];
		if (readBytes(buf, 4) != 4) {
			throw WavIoException("stdio read error.");
		}
		return toDWORD(buf);
	}
	/**
	 * @brief	16bit長のデータを16bit符号なし整数値として読み込む。
	 * データの読み込みができない場合は例外を投入する。
	 * @return	読み取った16bit符号なし整数
	 * @exception	WavIoException	読み込みエラー発生
	 */
	WORD readWORD() throw (WavIoException) {
		BYTE buf[2];
		if (readBytes(buf, 2) != 2) {
			throw WavIoException("stdio read error.");
		}
		return toWORD(buf);
	}
	/**
	 * @brief	8bit長のデータを8bit符号なし整数値として読み込む。
	 * データの読み込みができない場合は例外を投入する。
	 * @return	読み取った8bit符号なし整数
	 * @exception	WavIoException	読み込みエラー発生
	 */
	WORD readBYTE() throw (WavIoException) {
		BYTE buf;
		if (readBytes(&buf, 1) != 1) {
			throw WavIoException("stdio read error.");
		}
		return buf;
	}

private:
	//! 処理対象ファイルポインタ
	FILE*  fp_;
};

#endif // !_BINARYREADER_H_
