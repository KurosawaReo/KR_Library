/*
   - KR_Matrix.cpp - (DxLib)
*/
#include "KR_Matrix.h"

/*
   [解説]

   | a  b  c |
   | d  e  f |
   | 0  0  1 |

   座標(x, y) → (x, y, 1)として

   x' = a*x + b*y + c*1
   y' = d*x + e*y + f*1

   a,b,d,e: 回転, 拡大縮小などの変形(角度によってa,b,d,eのどこが変動するかが変わる)
   c,f    : 平行移動.
*/

//KrLib名前空間.
namespace KR
{

// ▼*--=<[ Matrix3 ]>=--*▼ //

	//行列 + 行列.
	Matrix3 Matrix3::operator+(const Matrix3& other) const {
		
		Matrix3 ret;                     //結果を入れる用.
		const int size = _int(m.size()); //行列のサイズ.

		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				//そのまま足す.
				ret.m[x][y] = m[x][y] + other.m[x][y];
			}
		}
		return ret;
	}

	//行列 * 行列.
	Matrix3 Matrix3::operator*(const Matrix3& other) const {

		Matrix3 ret;                     //結果を入れる用.
		const int size = _int(m.size()); //行列のサイズ.

		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				//横列 * 縦列 を加算する.
				for (int i = 0; i < size; i++) {
					ret.m[x][y] += m[x][i] * other.m[i][y];
				}
			}
		}
		return ret;
	}

	//行列 * 数値.
	Matrix3 Matrix3::operator*(const double num) const {

		Matrix3 ret;                     //結果を入れる用.
		const int size = _int(m.size()); //行列のサイズ.

		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				//そのまま掛け算.
				ret.m[x][y] = m[x][y] * num;
			}
		}
		return ret;
	}

	//行列 * 座標.
	DBL_XY Matrix3::operator*(const DBL_XY& pos) const {
		
		DBL_XY ret; //結果を入れる用.
		//行列の3行目は使わない.
		ret.x = m[0][0] * pos.x + m[1][0] * pos.y + m[2][0];
		ret.y = m[0][1] * pos.x + m[1][1] * pos.y + m[2][1];

		return ret;
	}

	//複合代入演算子.
	Matrix3 Matrix3::operator+=(const Matrix3& other) const {
		return *this + other;
	}
	Matrix3 Matrix3::operator*=(const Matrix3& other) const {
		return *this * other;
	}
	Matrix3 Matrix3::operator*=(const double   num)   const {
		return *this * num;
	}
	DBL_XY  Matrix3::operator*=(const DBL_XY& pos)   const {
		return *this * pos;
	}

// ▼*--=<[ TransformMat ]>=--*▼ //

	//コンストラクタ.
	TransformMat::TransformMat() { 
		mat = GetIdentityMatrix(); 
	}

	//移動量.
	void TransformMat::Translate(DBL_XY trans) {
		//行列合成.
		mat = ToMatrixTrans(trans) * mat;
	}
	//回転.
	void TransformMat::Rotate(double rot, DBL_XY pivot) {
		Matrix3 toOrigin = ToMatrixTrans(pivot * -1);
		Matrix3 toPivot  = ToMatrixTrans(pivot);
		//行列合成.
		mat = toPivot
			* ToMatrixRotate(rot)
			* toOrigin
			* mat;
	}
	//拡大縮小.
	void TransformMat::Scale(DBL_XY scale, DBL_XY pivot) {
		Matrix3 toOrigin = ToMatrixTrans(pivot * -1);
		Matrix3 toPivot  = ToMatrixTrans(pivot);
		//行列合成.
		mat = toPivot
			* ToMatrixScale(scale)
			* toOrigin
			* mat;
	}

// ▼*--=<[ Function ]>=--*▼ //

	//単位行列を取得(行列の初期値のようなもの)
	Matrix3 GetIdentityMatrix() {

		Matrix3 ret;
		//初期値.
		ret.m[0][0] = 1; ret.m[1][0] = 0; ret.m[2][0] = 0;
		ret.m[0][1] = 0; ret.m[1][1] = 1; ret.m[2][1] = 0;
		ret.m[0][2] = 0; ret.m[1][2] = 0; ret.m[2][2] = 1;
		return ret;
	}

	//逆行列を取得.
	Matrix3 GetInverseMatrix(Matrix3 mat) {

		Matrix3 ret; //結果を入れる用.

		//行列式 det(A) = aei+bfg+cdh-afh-bdi-ceg
		double det =
		      mat.m[0][0] * mat.m[1][1] * mat.m[2][2]
			+ mat.m[1][0] * mat.m[2][1] * mat.m[0][2]
			+ mat.m[2][0] * mat.m[0][1] * mat.m[2][1]
			- mat.m[0][0] * mat.m[2][1] * mat.m[1][2]
			- mat.m[1][0] * mat.m[0][1] * mat.m[2][2]
			- mat.m[2][0] * mat.m[1][1] * mat.m[2][0];

		//det(A)の場合、逆行列は存在しない.
		//浮動小数点型の関係上、0に近ければ0に等しいとみなす.
		if (det < 0.001 && det > -0.001) {
			return GetIdentityMatrix();	//単位行列のまま返す.
		}
		else {
			ret.m[0][0] =   mat.m[1][1] * mat.m[2][2] - mat.m[2][1] * mat.m[1][2];	//ei-fh
			ret.m[1][0] = -(mat.m[1][0] * mat.m[2][2] - mat.m[2][0] * mat.m[1][2]);	//-(bi-ch)
			ret.m[2][0] =   mat.m[1][0] * mat.m[2][1] - mat.m[2][0] * mat.m[1][1];	//bf-ce
			ret.m[0][1] = -(mat.m[0][1] * mat.m[2][2] - mat.m[2][1] * mat.m[0][2]);	//-(di-fg)
			ret.m[1][1] =   mat.m[0][0] * mat.m[2][2] - mat.m[2][0] * mat.m[0][2];	//ai-cg
			ret.m[2][1] = -(mat.m[0][0] * mat.m[2][1] - mat.m[2][0] * mat.m[0][1]);	//-(af-cd)
			ret.m[0][2] =   mat.m[0][1] * mat.m[1][2] - mat.m[1][1] * mat.m[0][2];	//dh-eg
			ret.m[1][2] = -(mat.m[0][0] * mat.m[1][2] - mat.m[1][0] * mat.m[0][2]);	//-(ah-bg)
			ret.m[2][2] =   mat.m[0][0] * mat.m[1][1] - mat.m[1][0] * mat.m[0][1];	//ae-bd
			//「1/det(A)」を掛ける.
			ret *= (1/det);

			return ret;
		}
	}

	//平行移動行列.
	Matrix3 ToMatrixTrans(DBL_XY trans) {

		Matrix3 ret = GetIdentityMatrix();
		//単位行列に移動量を合成.
		ret.m[2][0] = trans.x;
		ret.m[2][1] = trans.y;
		return ret;
	}
	//回転行列.
	Matrix3 ToMatrixRotate(double ang) {

		Matrix3 ret = GetIdentityMatrix();
		//単位行列に角度を合成.
		ret.m[0][0] =  cos(_rad(ang));
		ret.m[1][0] = -sin(_rad(ang));
		ret.m[0][1] =  sin(_rad(ang));
		ret.m[1][1] =  cos(_rad(ang));
		return ret;
	}
	//拡大縮小行列.
	Matrix3 ToMatrixScale(DBL_XY scale) {

		Matrix3 ret = GetIdentityMatrix();
		//単位行列にスケールを合成.
		ret.m[0][0] = scale.x;
		ret.m[1][1] = scale.y;
		return ret;
	}

	//行列をポリゴンに反映し、座標を移動する.
	void AffectMatrix(Polygon& poly, Matrix3 mat) {

		//位置データ.
		Matrix3 trans;
		trans.m[2][0] = mat.m[2][0];
		trans.m[2][1] = mat.m[2][1];
		//角度データ.
		Matrix3 rot = mat;
		rot.m[2][0] = 0;
		rot.m[2][1] = 0;

		//基準点の移動.
		poly.pos = trans * poly.pos;
		//頂点の移動.
		for (auto& i : poly.points) {
			i = rot * i;
		}
	}
}