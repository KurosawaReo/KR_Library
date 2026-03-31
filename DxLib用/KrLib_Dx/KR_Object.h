/*
   - KR_Object.h - (DxLib)
   ver.2026/02/07

   オブジェクト機能。
   継承して使うことで、Draw/Calc/Inputの一部機能をオブジェクト指向で使える。

   [class]
   ObjectCir: 円形
   ObjectBox: 四角形
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif
//[include] hで使うもの.
#include "KR_Draw.h"
#include "KR_Timer.h"

/*
   [画像について]
   DrawImgで「=」演算子を禁止にしてるため
   Objectのメンバに入れるとObject自体も「=」演算子が使えなくなる.
   そのため、外部からポインタで送る方式を使っている.
*/

//KrLib名前空間.
namespace KR
{
	//オブジェクト(図形)[継承想定]
	class ObjectShape
	{
	//▼ ===== 変数 ===== ▼.
	private:
		vector<string> useImg{};    //使う画像データ.
		int            useImgNo{};  //使う画像データのindex.
		Timer          tmImgAnim{}; //画像切り替え用タイマー.

	public:
		DBL_XY offset{};   //画像をずらす量.
		bool   isActive{}; //有効かどうか.

	//▼ ===== 関数 ===== ▼.
	protected:
		//コンストラクタ.
		ObjectShape() : 
			useImg(0), tmImgAnim(TimerMode::CountDown, 0), offset(0, 0), isActive(true)
		{}
		//画像更新.
		void UpdateImg();

	public:
		//virtual(中身が変わるため、派生クラスで設定する)
		virtual void      SetPos   (DBL_XY)       = 0;
		virtual DBL_XY    GetPos   ()       const = 0;
		virtual DBL_XY*   GetPosPtr()             = 0;
		virtual DBL_XY    GetSize  ()       const = 0;
		virtual ResultInt DrawShape(bool isFill = true, bool isAnti = false, bool isCameraDisp = true) const = 0;

		//画像.
		void      SetDrawImg     (string name);
		void      SetDrawImgs    (vector<string> names, float changeTime);
		void      SetStopImgAnim (bool isStop);
		//計算(Calcの機能)
		void      FixPosInArea   (DBL_RECT rect);
		bool      IsOutInArea    (DBL_RECT rect, bool isCompOut);
		double    Dist			 (DBL_XY pos);
		DBL_XY    ArcPos		 (double ang, double len);
		double    FacingAng		 (DBL_XY targetPos);
		//操作(Inputの機能)
		void      MoveKey4Dir    (float speed);
		void      MovePad4Dir    (float speed);
		void      MovePadStick   (float speed);
		void      MoveMousePos   (bool isMoveX = true, bool isMoveY = true);
		//描画(Drawの機能)
		ResultInt DrawGraph      (                                                          Anchor anc = Anchor::Mid, bool isFloat = false, bool isCameraDisp = true);
		ResultInt DrawRectGraph  (DBL_RECT rect,                                            Anchor anc = Anchor::Mid, bool isFloat = false, bool isCameraDisp = true);
		ResultInt DrawExtendGraph(DBL_XY sizeRate,                                          Anchor anc = Anchor::Mid, bool isFloat = false, bool isCameraDisp = true);
		ResultInt DrawRotaGraph  (double ang, double sizeRate = 1.0, INT_XY pivot = {0, 0},                           bool isFloat = false, bool isCameraDisp = true);
	};

	//オブジェクト(円)[継承想定]
	class ObjectCir : public ObjectShape
	{
	//▼ ===== 変数 ===== ▼.
	public:
		Circle cir{}; //当たり判定と座標.

	//▼ ===== 関数 ===== ▼.
	public:
		//コンストラクタ.
		ObjectCir() {
			cir.r     = 10;       //デフォルト半径.
			cir.color = 0xFFFFFF; //デフォルト色.
		}
		//get.
		Circle    GetCir   () const { return cir; }
		//override.
		void      SetPos   (DBL_XY _pos)       override { cir.pos = _pos; }
		DBL_XY    GetPos   ()            const override { return cir.pos; }
		DBL_XY*   GetPosPtr()                  override { return &cir.pos; }
		DBL_XY    GetSize  ()            const override { return {cir.r*2, cir.r*2}; }
		ResultInt DrawShape(bool isFill = true, bool isAnti = false, bool isCameraDisp = true) const override;

		//当たり判定(Calcの機能)
		bool      HitCheckCir (const Circle& cir)  const;
		bool      HitCheckBox (const Box&    box)  const;
		bool      HitCheckLine(const Line&   line) const;
	};

	//オブジェクト(四角形)[継承想定]
	class ObjectBox : public ObjectShape
	{
	//▼ ===== 変数 ===== ▼.
	public:
		Box box{}; //当たり判定と座標.

	//▼ ===== 関数 ===== ▼.
	public:
		//コンストラクタ.
		ObjectBox() {
			box.size  = {20, 20}; //デフォルトサイズ.
			box.color = 0xFFFFFF; //デフォルト色.
		}
		//get.
		Box       GetBox() const { return box; }
		//override.
		void      SetPos   (DBL_XY _pos)       override { box.pos = _pos; }
		DBL_XY    GetPos   ()            const override { return box.pos; }
		DBL_XY*   GetPosPtr()                  override { return &box.pos; }
		DBL_XY    GetSize  ()            const override { return box.size; }
		ResultInt DrawShape(bool isFill = true, bool isAnti = false, bool isCameraDisp = true) const override;

		//当たり判定(Calcの機能)
		bool      HitCheckCir(const Circle& cir) const;
		bool      HitCheckBox(const Box&    box) const;
	};

	//オブジェクト(グリッド上専用)
	class ObjectGrid
	{
	public:
		INT_XY pos{};      //座標.
		bool   isActive{}; //有効かどうか.

		//描画.
		ResultInt Draw(const DrawImg& img, INT_XY gridPos, INT_XY gridSize);
	};
}