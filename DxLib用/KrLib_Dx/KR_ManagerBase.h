/*
   - KR_ManagerBase.h - (DxLib)
   ver.2026/04/27

   Managerの基底クラス。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif
//[include] ".h"ファイルで使うもの.
#include "KR_Object.h"

//KrLib名前空間.
namespace KR
{
	//管理クラスの自動実行モード.
	enum class MngAutoExe
	{
		Active,		//Update & Draw
		UpdateOnly, //Updateのみ.
		DrawOnly,	//Drawのみ.
		Stop		//実行しない.
	};

	/*
	   管理クラスの基礎 [継承想定]
	   
	   Init, Update, Drawは自動でAppクラスから呼び出される.
	   (order値が小さいほど先に実行)
	*/
	class ManagerBase
	{
	//▼ ===== 変数 ===== ▼.
	private:
		MngAutoExe mode;    //自動実行モード.
		MngAutoExe befMode; //自動実行モード(1つ前保存用)
		int        order;   //処理優先度.

	//▼ ===== 関数 ===== ▼.
	public:
		//コンストラクタ.
		ManagerBase(int _order, MngAutoExe _mode = MngAutoExe::Active);
		//デストラクタ(これがあると安全?)
		virtual ~ManagerBase() = default;
		
		//get.
		int GetOrder() const { return order; }

		//自動実行モード.
		void       SetAutoExeMode(MngAutoExe _mode) { 
			befMode  = mode;    //元のモードを保存.
			mode     = _mode;   //モード切り替え.
		}
		void       BackAutoExeMode() {
			auto tmp = mode;
			mode     = befMode; //1つ前のモードに戻す.
			befMode  = tmp;     //元のモードを保存.
		}
		MngAutoExe GetAutoExeMode() const { return mode; }

		//判定.
		bool IsAutoUpdate() const {
			return mode == MngAutoExe::Active || mode == MngAutoExe::UpdateOnly;
		}
		bool IsAutoDraw() const {
			return mode == MngAutoExe::Active || mode == MngAutoExe::DrawOnly;
		}

		//基本処理.
		virtual void Init()   = 0;
		virtual void Reset()  = 0;
		virtual void Update() = 0;
		virtual void Draw()   = 0;
	};
}