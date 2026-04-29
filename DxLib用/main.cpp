/*
   - main.cpp - (DxLib)
   ver.2026/04/29

   プログラムの開始地点(テンプレ)
*/
#include "KrLib_Dx/KR_Global.h"
#include "KrLib_Dx/KR_App.h"
#include "KrLib_Dx/KR_Input.h"
#include "KrLib_Dx/KR_ManagerBase.h"
#include "KrLib_Dx/KR_ManagerInsts.h"
#include "KrLib_Dx/KR_SceneMng.h"
#include "KrLib_Dx/KR_Sound.h"
using namespace KR;

class GameManager : public ManagerBase
{
private:
public:
	//コンストラクタ.
	GameManager(int order) : ManagerBase(order) {}

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

int WINAPI WinMain(
	_In_     HINSTANCE hinstance,
	_In_opt_ HINSTANCE hPrevinstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow
){
	//Managerクラス実体生成.
	//引数で実行順(order値)を入力する.
	ManagerInsts::NewManager<InputMng>(0);
	ManagerInsts::NewManager<SoundMng>(1);
	ManagerInsts::NewManager<SceneMng>(2);

	ManagerInsts::NewManager<GameManager>(3);

	try {
		//初期化処理.
		App::InitDx(WINDOW_WID, WINDOW_HEI, IS_WINDOW_MODE, FPS, false);
	}
	catch (const ErrorMsg& err) {
		//エラーメッセージ.
		Debug::Log(_T("InitDx"), err.GetResult());
	}

	try {
		//ループ処理.
		App::LoopDx();
	}
	catch (const ErrorMsg& err) {
		//エラーメッセージ.
		Debug::Log(_T("LoopDx"), err.GetResult());
	}

	return 0;
}