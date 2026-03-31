/*
   - KR_Memory.h - (DxLib)
   ver.2026/02/12

   行列の計算機能、ポリゴン操作機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//3×3行列.
	struct Matrix3
	{
		array<array<double, 3>, 3> m{}; //float m[x][y]

		//演算子.
		Matrix3 operator+ (const Matrix3& other) const; //行列 + 行列.
		Matrix3 operator* (const Matrix3& other) const; //行列 * 行列.
		Matrix3 operator* (const double   num)   const; //行列 * 数値.
		DBL_XY  operator* (const DBL_XY&  pos)   const; //行列 * 座標.
		//複合代入演算子.
		Matrix3 operator+=(const Matrix3& other) const;
		Matrix3 operator*=(const Matrix3& other) const;
		Matrix3 operator*=(const double   num)   const;
		DBL_XY  operator*=(const DBL_XY&  pos)   const;
	};

	/*
	   移動, 回転, 拡大縮小を行列で管理する。
	   基本は変化量として使う。
	*/
	class TransformMat
	{
	private:
		Matrix3 mat; //行列.

	public:
		//コンストラクタ.
		TransformMat();
		//get.
		Matrix3 GetMatrix() const { return mat; }

		void Translate(DBL_XY trans);               //移動量.
		void Rotate   (double rot,   DBL_XY pivot); //回転量.
		void Scale    (DBL_XY scale, DBL_XY pivot); //拡大縮小量.
	};

	//行列の取得.
	Matrix3 GetIdentityMatrix ();
	Matrix3 GetInverseMatrix  (Matrix3 mat);
	//行列化する.
	Matrix3 ToMatrixTrans (DBL_XY  trans);
	Matrix3 ToMatrixRotate(double  ang);
	Matrix3 ToMatrixScale (DBL_XY  scale);

	//行列の反映.
	void    AffectMatrix      (Polygon& poly, Matrix3 mat);
}