/*
   - main.cpp - (DxLib)
   ver.2026/02/04

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
	//初期化処理.
	ResultInt err = App::InitDx(800, 600, true, 60, false);
	if (err.GetCode() < 0) { return -1; } //初期化エラー.
	//ループ処理.
	App::LoopDx();

	return 0;
}