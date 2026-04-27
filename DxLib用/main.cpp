/*
   - main.cpp - (DxLib)
   ver.2026/04/27

   プログラムの開始地点(テンプレ)
*/
#include "KrLib_Dx/KR_App.h"
#include "KrLib_Dx/KR_ManagerBase.h"
using namespace KR;

class GameManager : public ManagerBase
{
private:
public:
	//コンストラクタ.
	GameManager() : ManagerBase(0) {}

	//初期化.
	void Init() override {
	}
	//リセット.
	void Reset() override {
	}
	//更新.
	void Update() override {
	}
	//描画.
	void Draw() override {
	}
};
GameManager gameMng;

int WINAPI WinMain(
	_In_     HINSTANCE hinstance,
	_In_opt_ HINSTANCE hPrevinstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow
){
	try {
		//初期化処理.
		App::InitDx(WINDOW_WID, WINDOW_HEI, IS_WINDOW_MODE, FPS, false);
	}
	catch (const ErrorMsg& err) {
		Debug::Log(_T("InitDx"), err.GetResult());
	}

	try {
		//ループ処理.
		App::LoopDx();
	}
	catch (const ErrorMsg& err) {
		Debug::Log(_T("LoopDx"), err.GetResult());
	}

	return 0;
}