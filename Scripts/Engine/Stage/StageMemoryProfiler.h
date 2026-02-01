/*
 *	@file	StageMemoryProfiler.h
 *  @author oorui
 */

#ifndef _STAGEMEMORYPROFILER_H_
#define _STAGEMEMORYPROFILER_H_

#include <string>
#include <fstream>

 /*
  *	ステージの当たり判定においてのメモリ消費量を確認する
  */
class StageMemoryProfiler {
private:
	static std::ofstream file;
	static size_t peakWorkingSet;
public:

	/*
	 *	初期化
	 *  @param	const std::string& ファイルパス
	 *  @tips	csvファイルを指定場所に作成、あれば中身をクリアしてうわガキ
	 */
	static void Initialize(const std::string& filePath);

	/*
	 *	メモリ使用量を記録
	 *  @param	const std::string& ファイル名
	 *  @tips	メモリの使用量をファイルに書き込む
	 */
	static void Log(const std::string& label);

	/*
	 *	毎フレームのメモリ消費量を見る
	 */
	static void UpdatePeak();

	/*
	 *	最大使用メモリを記録
	 */
	static void LogPeak();

	/*
	 *	csvへの書き込みを終了する
	 */
	static void Execute();

};

#endif // !_STAGEMEMORYPROFILER_H_
