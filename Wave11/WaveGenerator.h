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
 * @file	WaveGenerator.h
 * @brief	PCMのテスト波形生成クラスのヘッダー
 */
// ----------------------------------------------------------------------------
#ifndef _WAVEGENERATOR_H_
#define _WAVEGENERATOR_H_

#if defined(_WIN32) && defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif // defined(_WIN32) && defined(_MSC_VER)
#include <math.h>
#include <cstdlib>

// ----------------------------------------------------------------------------
/**
 * @brief テスト波形の生成クラス
 * メソッドを繰り返し呼ぶことで波形を生成する
 */
// ----------------------------------------------------------------------------
class WaveGenerator
{
public:
	/**
	 * @param[in]	freq			周波数
	 * @param[in]	samplingRate	信号の発生サンプリングレート
	 */
	WaveGenerator(int freq, int samplingRate) : freq_(freq), samplingRate_(samplingRate), phase_(0.0){}
	virtual ~WaveGenerator() {}

	//! 位相をリセットします。
	void reset() { phase_ = 0.0; }

	/**
	 * @brief	ノコギリ波のサンプル生成
	 * @return	生成サンプル
	 */
	float SawSample() {
		phase_ += static_cast<float>(freq_) / samplingRate_;
		phase_ -= floor(phase_);
		return static_cast<float>(2.0*(phase_ - 0.5));
	}
	/**
	 * @brief	矩形波のサンプル生成
	 * @return	生成サンプル
	 */
	float PulseSample() {
		phase_ += static_cast<float>(freq_) / samplingRate_;
		phase_ -= floor(phase_);
		return (phase_ < 0.5) ? -1.f : 1.f;
	}
	/**
	 * @brief	三角波のサンプル生成
	 * @return	生成サンプル
	 */
	float TriangleSample() {
		phase_ += static_cast<float>(freq_) / samplingRate_;
		phase_ -= floor(phase_);
		return static_cast<float>(4.0 * ((phase_ > 0.5) ? 1.0 - phase_ : phase_) - 1.0);
	}
	/**
	 * @brief	正弦波のサンプル生成
	 * @return	生成サンプル
	 */
	float SinSample() {
		phase_ += static_cast<float>(freq_) / samplingRate_;
		phase_ -= floor(phase_);
		return static_cast<float>(sin(2.0 * M_PI * phase_));
	}
	/**
	 * @brief	ホワイトノイズのサンプル生成
	 * @return	生成サンプル
	 */
	float WnoiseSample() {
		return 2.f * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f);
	}
	/**
	 * @brief	ノコギリ波のサンプル生成（16bit）
	 * @return	生成サンプル
	 */
	short SawSample16() { return static_cast<short>(SawSample()*32767); }
	/**
	 * @brief	矩形波のサンプル生成（16bit）
	 * @return	生成サンプル
	 */
	short PulseSample16() { return static_cast<short>(PulseSample() * 32767); }
	/**
	 * @brief	三角波のサンプル生成（16bit）
	 * @return	生成サンプル
	 */
	short TriangleSample16() { return static_cast<short>(TriangleSample()* 32767); }
	/**
	 * @brief	正弦波のサンプル生成（16bit）
	 * @return	生成サンプル
	 */
	short SinSample16() { return static_cast<short>(SinSample16() * 32767); }
	/**
	 * @brief	ホワイトノイズのサンプル生成（16bit）
	 * @return	生成サンプル
	 */
	short WnoiseSample16() { return static_cast<short>(WnoiseSample() * 32767); }

private:
	//! 周波数
	const int freq_;
	//! サンプリングレート
	const int samplingRate_;
	//! 生成位相
	double phase_;

	WaveGenerator();
};

#endif // !_WAVEGENERATOR_H_
