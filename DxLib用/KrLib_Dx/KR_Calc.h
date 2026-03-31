/*
   - KR_Calc.h - (DxLib)
   ver.2026/02/12

   当たり判定、物理、アニメーション曲線などの計算機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//イージングタイプ.
	enum class EaseType
	{
		InQuad,
		OutQuad,
		InOutQuad,
		OutInQuad,
	};
	//ループ波形タイプ.
	enum class WaveType
	{
		CosLoop,
	};

	//計算用の関数群.
	namespace Calc
	{
		//当たり判定.
		bool		HitCirCir			(const Circle& cir1, const Circle& cir2);
		bool		HitBoxBox			(const Box&    box1, const Box&    box2);
		bool		HitBoxCir			(const Box&    box,  const Circle& cir);
		bool		HitLineCir			(const Line&   line, const Circle& cir);
		bool        HitPie				(const Pie&    pie,  DBL_XY pos);

		//範囲限界.
		void		FixPosInArea		(DBL_XY* pos, INT_XY size, DBL_RECT rect);
		bool		IsOutInArea			(DBL_XY  pos, INT_XY size, DBL_RECT rect, bool isCompOut);

		//角度,ベクトル.
		double		Dist				(INT_XY pos1,  INT_XY pos2);
		double		Dist				(DBL_XY pos1,  DBL_XY pos2);
		DBL_XY		MidPos				(DBL_XY pos1,  DBL_XY pos2);
		DBL_XY		ArcPos				(DBL_XY pos, double ang, double len);
		double		FacingAng			(DBL_XY from, DBL_XY to);
		DBL_XY      AngToVector		    (double ang);

		//ベジエ曲線,スプライン曲線.
		DBL_XY		BezierPoint			(const BezierLine& bLine,  double time);
		DBL_XY      SplinePoint         (const Spline&     spline, int degree, double time);

		//物理系.
		void        PhysicsVel          (DBL_XY* vel, DBL_XY maxVel, bool isGround, double gravity, double friction, double airDrag);

		//値の曲線変動(アニメーション用)
		double      AnimEase			(EaseType type, double time);
		double      AnimWave			(WaveType type, double time);

		//値の操作.
		int         RandNum				(int st, int ed, bool isDxRand = false);
		vector<int> RandNums			(int st, int ed, int count, bool isDxRand = false);
		double      GetDecimal			(double num);
	};
}