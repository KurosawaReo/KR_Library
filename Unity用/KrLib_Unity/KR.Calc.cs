/*
   - KR.Calc - (Unity)
   ver.2026/03/24
*/
using UnityEngine;
using System;
using Unity.VisualScripting;

/// <summary>
/// 計算を扱う用の追加機能.
/// </summary>
namespace KR.Unity.Calc
{
    /// <summary>
    /// static関数.
    /// </summary>
    public static class Func
    {
        /// <summary>
        /// プラスかマイナスかを取得.
        /// </summary>
        /// <param name="num">元の値</param>
        /// <returns>符号(0/+1/-1)</returns>
        public static int GetNumSign(float num)
        {
            //0なら0、それ以外は符号を返す.
            return (num == 0) ? 0 : (num > 0) ? +1 : -1;
        }

        /// <summary>
        /// 値が範囲内にあるかどうか.
        /// </summary>
        /// <param name="num">判定値</param>
        /// <param name="low">最低値</param>
        /// <param name="high">最大値</param>
        /// <returns>範囲内ならtrue</returns>
        public static bool IsRangeNum(int num, int low, int high)
        {
            return (low <= num) && (num <= high);
        }
        /// <summary>
        /// 値が範囲内にあるかどうか.
        /// </summary>
        /// <param name="num">判定値</param>
        /// <param name="low">最低値</param>
        /// <param name="high">最大値</param>
        /// <returns>範囲内ならtrue</returns>
        public static bool IsRangeNum(double num, double low, double high)
        {
            return (low <= num) && (num <= high);
        }

        public static float FacingAng(Vector2 from, Vector2 to)
        {
            //距離差.
            Vector2 dis = to - from;
            //距離差を角度に.
            return VectorToDeg(dis);
        }

        /// <summary>
        /// ベクトルから角度を求める.
        /// </summary>
        /// <param name="dir">ベクトル方向</param>
        /// <returns>角度(Deg)</returns>
        public static float VectorToDeg(Vector2 dir)
        {
            return Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg;
        }

        /// <summary>
        /// 0.0～99.9の乱数値を取得(確率計算用)
        /// </summary>
        /// <returns></returns>
        public static float Random100()
        {
            return UnityEngine.Random.Range(0.0f, 99.9f);
        }

        /// <summary>
        /// 入力情報から移動方向を求める.
        /// </summary>
        public static Vector2 CalcInputVec(Vector2 input)
        {
            //操作してるなら.
            if (input != Vector2.zero)
            {
                //角度(theta:シータ)をラジアンで求める.
                var theta = Mathf.Atan2(input.y, input.x); //tan(タンジェント)
                //xとyの移動量を求める.
                var y = Mathf.Sin(theta); //sin = y成分.
                var x = Mathf.Cos(theta); //cos = x成分.
                //ほぼ0の値なら、0とみなす(計算上誤差があるため)
                if (Mathf.Abs(y) < 0.0001f) { y = 0; }
                if (Mathf.Abs(x) < 0.0001f) { x = 0; }

                return new Vector2(x, y);
            }
            else
            {
                return Vector2.zero; //(0, 0)を返す.
            }
        }

        /// <summary>
        /// 列挙体の要素数取得.
        /// </summary>
        /// <typeparam name="T">列挙体の型</typeparam>
        /// <returns>要素数</returns>
        public static int GetEnumLen<T>()
        {
            return Enum.GetValues(typeof(T)).Length;
        }
    }
}