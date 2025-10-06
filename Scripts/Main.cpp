/*
 *	@file	Main.cpp
 */

#include "Engine/Engine.h"

#include <DxLib.h>
#include <EffekseerForDXLib.h>

/*
 * @brief		Windowプログラムのエントリーポイント
 * @param[in]	HINSTANCE	今回は不要
 * @param[in]	HINSTANCE	今回は不要
 * @param[in]	LPSTR		今回は不要
 * @param[in]	int			今回は不要
 * @return		int
 * @tips		WINAPI
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Engine engine;
	return engine.Run();
}
