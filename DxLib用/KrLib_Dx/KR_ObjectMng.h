/*
   - KR_ObjectMng.h - (DxLib)
   ver.2026/04/16

   オブジェクトを扱う管理クラス。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif
//[include] hで使うもの.
#include "KR_ManagerBase.h"
#include "KR_Object.h"

//KrLib名前空間.
namespace KR
{
	/*
	   オブジェクト管理クラス [試作品]
	   ObjectShapeを継承したクラスを登録すれば、まとめてInit, Update, Drawなどの関数を実行できる.
	*/
	class ObjectMng
	{
	//▼ ===== 変数 ===== ▼.
	private:
		list<ObjectShape*> objects; //object配列.

	//▼ ===== 関数 ===== ▼.
	public:
		//get.
		int GetObjectCnt() const { return objects.size(); }

		//object追加.
		void AddObject(ObjectShape* obj) {
			obj->Init();            //初期化処理.
			objects.push_back(obj); //配列に追加.
		}
		
		void Update(); //更新処理.
		void Draw();   //描画処理.
	};
}