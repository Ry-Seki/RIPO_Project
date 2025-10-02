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
#pragma region DxLibの初期化処理(理解するまでは触らない)
	SetOutApplicationLogValidFlag(FALSE);
	//ウィンドウのサイズを変更する
	SetGraphMode(800, 600, 32, 0);
	//起動時のウィンドウのモードの設定
	ChangeWindowMode(TRUE);		// true : Windowモード, false : FullScreen
	//背景色の設定
	SetBackgroundColor(196, 196, 196);

	//DirectXを仕様宣言　理由：Effekseerを使用するために必ず設定する -> DirectX9, 11
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//DxLibの初期化
	if (DxLib_Init() == -1)
		return 0;

	//Effekseerの初期化
	//Effecseer_Init(引数) 引数は最大のパーティクル量
	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return 0;
	}

	//描画する先を設定する　裏画面に変更する
	SetDrawScreen(DX_SCREEN_BACK);
	//図形描画のZバッファの有効化
	{
		//Zバッファを使用するかどうか	デフォルト : FALSE
		SetUseZBuffer3D(TRUE);
		//Zバッファに書き込みを行うか	デフォルト : FALSE
		SetWriteZBuffer3D(TRUE);
	}
	//ライティング
	{
		//ライトの計算をするかどうか	デフォルト : TRUE
		SetUseLighting(TRUE);
		//標準ライトを使用するかどうか	デフォルト : TRUE
		SetLightEnable(TRUE);
		//グローバル環境光の設定
		SetGlobalAmbientLight(GetColorF(0.3f, 0.3f, 0.3f, 1.0f));	//ライトの計算でα値は使わない	色がfloat型の場合、0.0f～1.0fの範囲

		//反射光の設定		Diffuse
		SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		//鏡面反射光の設定	Specular
		SetLightSpcColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		//環境光の設定		Ambient
		SetLightAmbColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	}
#pragma endregion

#pragma region ゲームの初期化処理

#pragma endregion
	//----------------------------------------
	//ゲームのメインループ
	//----------------------------------------
	while (1) {
		//ウィンドウのメッセージを処理する
		if (ProcessMessage() == -1)
			break;

		//エフェクシアの更新
		Effekseer_Sync3DSetting();

		//画面をクリアする
		ClearDrawScreen();

		//裏画面と表画面を切り換える
		ScreenFlip();
	}
#pragma region ゲームの解放処理

#pragma endregion

#pragma region DxLibの解放処理
	//Effekseerの終了
	Effkseer_End();
	//DxLibの終了
	DxLib_End();
#pragma endregion

	return 0;
}
