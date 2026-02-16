/*
 *	@file	StageMemoryProfiler.cpp
 *  @author oorui
 */

#include "StageMemoryProfiler.h"


#include <Windows.h>
#include <Psapi.h>
#include <filesystem>
unsigned __int64 StageMemoryProfiler::peakWorkingSet = 0;
std::ofstream StageMemoryProfiler::file;
/*
 *	初期化
 *  @param	const std::string& ファイルパス
 *  @tips	csvファイルを指定場所に作成、あれば中身をクリアして上書き
 */
void StageMemoryProfiler::Initialize(const std::string& filePath) {
	// CSVを出力するフォルダが存在しない場合は作成する
	std::filesystem::create_directories(
		std::filesystem::path(filePath).parent_path()
	);

	// CSVファイルを新規作成、すでにあったら中身をクリアする
	file.open(filePath, std::ios::out);
	file << "計測箇所,メモリ使用量(MB),メモリ使用量(B)\n";
}


/*
 *	メモリ使用量を記録
 *  @param	const std::string& ファイル名
 *  @tips	メモリの使用量をファイルに書き込む
 */
void StageMemoryProfiler::Log(const std::string& label) {
	// ファイルが開かれていなければ、なにもしない
	if (!file.is_open()) return;

	// メモリ使用状況を保存する構造体
	PROCESS_MEMORY_COUNTERS pmc;

	// 現在のメモリ情報を取得
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		// ラベルとメモリ使用量をCSV形式で出力
		file << label << ","
			<< (pmc.WorkingSetSize / MEMORY_NUM) / MEMORY_NUM << ","
			<< pmc.WorkingSetSize << "\n";
	}
}

/*
 *	毎フレームのメモリ消費量を見る
 */
void StageMemoryProfiler::UpdatePeak() {
	// メモリ使用状況を保存する構造体
	PROCESS_MEMORY_COUNTERS pmc;
	// 現在のメモリ情報を取得
	if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		return;
	}

	// これまでの最大値より大きければ更新
	if (pmc.WorkingSetSize > peakWorkingSet) {
		peakWorkingSet = pmc.WorkingSetSize;
	}
}
/*
 *	最大使用メモリを記録
 */
void StageMemoryProfiler::LogPeak() {
	if (!file.is_open()) return;

	// KB / B 表記の両方を出力
	file << "最大使用量,"
		<< (peakWorkingSet / MEMORY_NUM) / MEMORY_NUM << ","
		<< peakWorkingSet << "\n";
}

/*
 *	csvへの書き込みを終了する
 */
void StageMemoryProfiler::Execute() {
	// ファイルが開いていれば閉じる
	// ファイルが開いていれば閉じる
	if (file.is_open()) {
		file.close();
	}
	// 最大値をリセット
	peakWorkingSet = 0;
}
