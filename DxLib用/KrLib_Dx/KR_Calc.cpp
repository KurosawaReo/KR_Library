/*
   - KR_Calc.cpp - (DxLib)
*/
#include "KR_Calc.h"

//KrLib名前空間.
namespace KR
{
	//計算用の関数群.
	namespace Calc
	{
		//当たり判定(円と円)
		bool HitCirCir(const Circle& cir1, const Circle& cir2) {

			//距離差.
			double x = cir1.pos.x - cir2.pos.x;
			double y = cir1.pos.y - cir2.pos.y;
			//距離が半径の合計以下なら(√を削減するために2乗して計算)
			if (x*x + y*y <= pow(cir1.r+cir2.r, 2)) {
				return true; //hit.
			}
			else {
				return false;
			}
		}
		//当たり判定(四角と四角)
		bool HitBoxBox(const Box& box1, const Box& box2) {

			//中央基準座標での判定.
			if (fabs(box1.pos.x - box2.pos.x) <= (box1.size.x + box2.size.x)/2 &&
				fabs(box1.pos.y - box2.pos.y) <= (box1.size.y + box2.size.y)/2
			){
				return true; //hit.
			}
			else {
				return false;
			}
		}
		//当たり判定(四角と円)
		bool HitBoxCir(const Box& box, const Circle& cir) {

			DBL_XY nearest = cir.pos;

			//円の中心から一番近い四角形の点を求める.
			NumLimRange(&nearest.x, box.pos.x - box.size.x/2, box.pos.x + box.size.x/2);
			NumLimRange(&nearest.y, box.pos.y - box.size.y/2, box.pos.y + box.size.y/2);
			//円の中心との距離.
			double dis = Dist(cir.pos, nearest);

			return (dis <= cir.r); //距離が半径以下ならhit.
		}
		//当たり判定(線と円)
		bool HitLineCir(const Line& line, const Circle& cir) {

			//線の始点と終点から傾きを求める.
			double katamuki;
			{
				double x = line.edPos.x - line.stPos.x;
				double y = line.edPos.y - line.stPos.y;
				if (x != 0) {
					katamuki = y / x;
				}
				else {
					katamuki = 999; //0割対策.
				}
			}
			//線を方程式にした時の切片.
			double seppen = line.stPos.y - line.stPos.x * katamuki;

			//線～円の距離.
			double dis1;
			{
				//直線: ax + by + c = 0
				//bを1として「y = 」の形にする→ y = -ax - c
				double a = -katamuki;
				double b = 1;
				double c = -seppen;
				//公式: d = |ax + by + c|/√(a^2 + b^2)
				dis1 = fabs(a*cir.pos.x + b*cir.pos.y + c) / sqrt(a*a + b*b);
			}
			//線の中点～円の中心の距離.
			double dis2;
			{
				double x = cir.pos.x - MidPos(line.stPos, line.edPos).x;
				double y = cir.pos.y - MidPos(line.stPos, line.edPos).y;
				//距離: d = √(x^2 + y^2) (三平方の定理)
				dis2 = sqrt(x*x + y*y);
			}

			//hit条件.
			if (dis1 <= cir.r &&                               //条件1: 線に触れている.
				dis2 <= Dist(line.stPos, line.edPos)/2 + cir.r //条件2: 線を直径とする円に触れている.
			){
				return true;
			}
			return false;
		}
		//当たり判定(扇形と点)
		bool HitPie(const Pie& pie, DBL_XY pos) {

			//扇形の中心からの距離.
			double distLen = Dist(pie.pos, pos);
			//扇形の中心からの角度.
			double ang = FacingAng(pie.pos, pos);
			//扇形の中心角.
			double centerAng = pie.stAng + pie.arcAng/2;
			//角度差(1.0～-1.0の範囲, 距離差が少ないほど1.0に近づく)
			double distAng = cos(_rad(centerAng-ang));

			//hit条件.
			if (distLen <= pie.r                   && //条件1: 扇形の半径内に入っている.
				distAng >= cos(_rad(pie.arcAng/2))    //条件2: 扇形の角度の範囲に入っている.
			){
				return true;
			}
			return false;
		}

		//範囲内に座標を補正する.
		void FixPosInArea(DBL_XY* pos, INT_XY size, DBL_RECT rect) {

			if (pos->x < rect.left  + size.x/2) { pos->x = rect.left  + size.x/2; }
			if (pos->y < rect.up    + size.y/2) { pos->y = rect.up    + size.y/2; }
			if (pos->x > rect.right - size.x/2) { pos->x = rect.right - size.x/2; }
			if (pos->y > rect.down  - size.y/2) { pos->y = rect.down  - size.y/2; }
		}
		//エリアの範囲外かどうか.
		bool IsOutInArea(DBL_XY pos, INT_XY size, DBL_RECT rect, bool isCompOut) {

			//完全に出たら範囲外とする.
			if (isCompOut) {
				if (pos.x < rect.left  - size.x/2) { return true; }
				if (pos.y < rect.up    - size.y/2) { return true; }
				if (pos.x > rect.right + size.x/2) { return true; }
				if (pos.y > rect.down  + size.y/2) { return true; }
			}
			//ちょっとでも出たら範囲外とする.
			else {
				if (pos.x < rect.left  + size.x/2) { return true; }
				if (pos.y < rect.up    + size.y/2) { return true; }
				if (pos.x > rect.right - size.x/2) { return true; }
				if (pos.y > rect.down  - size.y/2) { return true; }
			}

			return false; //範囲内.
		}

		//距離を求める.
		//[座標1,座標2 → 長さ]
		double Dist(INT_XY pos1, INT_XY pos2) {
			return (pos1 - pos2).Dist(); //斜辺の長さを返す.
		}
		double Dist(DBL_XY pos1, DBL_XY pos2) {
			return (pos1 - pos2).Dist(); //斜辺の長さを返す.
		}
		//中点座標を求める.
		//[座標1,座標2 → 中点座標]
		DBL_XY MidPos(DBL_XY pos1, DBL_XY pos2) {
			return (pos1 + pos2)/2; //xとyの平均を返す.
		}
		//角度と長さから円周上の座標を求める.
		//[座標1,角度,長さ → 座標2]
		DBL_XY ArcPos(DBL_XY pos, double ang, double len) {
			return pos + AngToVector(ang) * len; //終点座標を返す.
		}
		//始点座標から対象座標への方向を求める.
		//[座標1,座標2 → 角度]
		//[返り値:-180.0～180.0]
		double FacingAng(DBL_XY from, DBL_XY to) {
			return (to - from).Angle();
		}
		//角度からベクトルを求める.
		DBL_XY AngToVector(double ang) {
			return { cos(_rad(ang)), sin(_rad(ang)) }; //vector(-1.0～+1.0)を返す.
		}

		//前方宣言(.cpp内でのみ使うため)
		vector<double> CreateOpenUniformKnots(int controlCount, int degree);
		double         BSplineBasis          (int i, int degree, double t, const vector<double>& knots);

		//ベジエ曲線から一点を取得.
		DBL_XY BezierPoint(const BezierLine& bLine, double time) {

			double x = 
				      (1 - time) * (1 - time) * (1 - time) * bLine.stPos.x 
				+ 3 * (1 - time) * (1 - time) *      time  * bLine.stContrPos.x
				+ 3 * (1 - time) *      time  *      time  * bLine.edContrPos.x 
				+          time  *      time  *      time  * bLine.edPos.x;
			double y = 
				      (1 - time) * (1 - time) * (1 - time) * bLine.stPos.y
				+ 3 * (1 - time) * (1 - time) *      time  * bLine.stContrPos.y
				+ 3 * (1 - time) *      time  *      time  * bLine.edContrPos.y
				+          time  *      time  *      time  * bLine.edPos.y;

			return { x, y };
		}
		//スプライン曲線から一点を取得.
		DBL_XY SplinePoint(const Spline& spline, int degree, double time)
		{
			const int  count = _int(spline.points.size());
			const auto knots = CreateOpenUniformKnots(count, degree);

			//timeが0.0(始点)以前なら.
			if (time <= 0.0) {
				return spline.points.front(); //始点の座標.
			}
			//timeが1.0(終点)以降なら.
			if (time >= 1.0) {
				return spline.points.back();  //終点の座標.
			}

			DBL_XY p{ -1, -1 };

			for (int j = 0; j < count; j++) {
				double b = BSplineBasis(j, degree, time, knots);
				p.x += b * spline.points[j].x;
				p.y += b * spline.points[j].y;
			}
			return p;
		}
		//Knots生成.
		vector<double> CreateOpenUniformKnots(int controlCount, int degree)
		{
			//TODO: 整理する.
			const int knotCount = controlCount + degree + 1;
			vector<double> knots(knotCount);

			for (int i = 0; i < knotCount; i++) {
				if (i <= degree) {
					knots[i] = 0.0f;
				}
				else if (i >= controlCount) {
					knots[i] = 1.0f;
				}
				else {
					knots[i] =
						(double)(i - degree) /
						(double)(controlCount - degree);
				}
			}
			return knots;
		}
		//Bスプライン基底関数.
		double BSplineBasis(int i, int degree, double t, const vector<double>& knots)
		{
			//TODO: 整理する.
			//0次.
			if (degree == 0) {
				return (knots[i] <= t && t < knots[i + 1]) ? 1.0f : 0.0f;
			}

			double left = 0.0;
			double right = 0.0;

			const double denom1 = knots[i + degree] - knots[i];
			const double denom2 = knots[i + degree + 1] - knots[i + 1];

			if (denom1 > 0.0f) {
				left = (t - knots[i]) / denom1
					* BSplineBasis(i, degree - 1, t, knots);
			}
			if (denom2 > 0.0f) {
				right = (knots[i + degree + 1] - t) / denom2
					* BSplineBasis(i + 1, degree - 1, t, knots);
			}

			return left + right;
		}

		/*
		   物理法則での速度減衰.
		   
		   vel     : 速度.
		   maxVel  : 限界速度(絶対値)
		   isGround: 接地しているか.
		   gravity : 重力.
		   friction: 摩擦.
		   airDrag : 空気抵抗.

		   (例)
		   初速度  : 10,-10
		   重力    : 0.8
		   摩擦    : 0.1
		   空気抵抗: 0.01
		*/
		void PhysicsVel(DBL_XY* vel, DBL_XY maxVel, bool isGround, double gravity, double friction, double airDrag)
		{
			//値の補正.
			maxVel.x = abs(maxVel.x);
			maxVel.y = abs(maxVel.y);
			NumLimRange(&friction,  0.0, 1.0);
			NumLimRange(&airDrag,   0.0, 1.0);

			//[1]速度.
			DBL_XY v = *vel;

			//[2]加速&抵抗.
			if (isGround) {
				v.x *= (1.0 - friction); //摩擦による減速.
				v.y = 0;                 //接地中は落下停止.
			}
			else {
				v *= (1.0 - airDrag); //空気抵抗による減速.
				v.y += gravity;       //重力.
			}

			//[3]限界速度.
			NumLimRange(&v.x, -maxVel.x, maxVel.x);
			NumLimRange(&v.y, -maxVel.y, maxVel.y);

			//[4]速度更新.
			*vel = v;
		}

		//値の曲線変動(アニメーション用)
		double AnimEase(EaseType type, double time) {

			//タイプ別.
			switch (type) {
				//InQuad: 徐々に加速.
				case EaseType::InQuad:
				{
					NumLimRange(&time, 0.0, 1.0); //0.0～1.0の範囲.
					return time * time;
				}
				//OutQuad: 徐々に減速.
				case EaseType::OutQuad:
				{
					NumLimRange(&time, 0.0, 1.0); //0.0～1.0の範囲.
					return 1 - (1 - time) * (1 - time);
				}
				//InOutQuad: 徐々に加速して減速.
				case EaseType::InOutQuad:
				{
					NumLimRange(&time, 0.0, 1.0); //0.0～1.0の範囲.
					return time < 0.5 ? 2 * time * time : 1 - pow(-2 * time + 2, 2) / 2;
				}
				//OutInQuad: 徐々に減速して加速.
				case EaseType::OutInQuad:
				{
					NumLimRange(&time, 0.0, 1.0); //0.0～1.0の範囲.
					if (time < 0.5) {
						//0.0→0.5まで.
						return 0.5 * sin(M_PI * time);
					}
					else {
						//0.5→1.0まで.
						return 0.5 + 0.5 * (1.0 - cos(M_PI * (time - 0.5)));
					}
				}
			}
			return 0; //未対応のタイプ.
		}
		double AnimWave(WaveType type, double time) {
			//タイプ別.
			switch (type) {
				//CosLoop: cos波のループ(0.0～1.0)
				case WaveType::CosLoop:
				{
					return 0.5 - cos(M_PI*time)/2;
				}
			}
			return 0; //未対応のタイプ.
		}

		//値の抽選.
		int RandNum(int st, int ed, bool isDxRnd) {

			int rnd = 0;

			//edがst以上の時のみ抽選.
			_if_check(st <= ed) {

				//DxLib用の乱数を使うかどうか.
				if (isDxRnd) {
					rnd = GetRand(ed - st); //st～endの差で抽選.
				}
				else {
					rnd = rand() % ((ed - st) + 1); //st～endの差で抽選.
				}
			}
			return st + rnd;
		}
		//値の抽選(重複なし複数)
		vector<int> RandNums(int st, int ed, int count, bool isDxRnd) {

			int unUsedCnt = (ed-st)+1;      //未使用数字はいくつあるか.
			vector<bool> isUsed((ed-st)+1); //各数字が使用済かどうか.
			vector<int>  ret   (count);     //抽選結果を入れる用.
	
			//抽選する回数分ループ.
			for (int i = 0; i < count; i++) {

				//未使用数字から何番目を選ぶか抽選.
				int cnt = RandNum(1, unUsedCnt, isDxRnd);
		
				//st～edの中で数字を選ぶ.
				for (int j = 0; j <= ed-st; j++) {
					//未使用数字なら.
					if (!isUsed[j]) {
						cnt--; //カウント-1
						if (cnt == 0) {
							ret[i] = st+j;    //この数字を抽選結果に保存.
							isUsed[j] = true; //この数字は使用済.
							unUsedCnt--;      //未使用枠-1.
							break;
						}
					}
				}
			}

			//抽選結果を返す.
			return ret;
		}
		//値から小数だけ取り出す.
		double GetDecimal(double num) {
			return fmod(num, 1.0); //1.0で割った余り.
		}
	}
}