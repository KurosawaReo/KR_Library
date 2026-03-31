/*
   - KR.Position - (Unity)
   ver.2026/03/30
*/
using Unity.VisualScripting;
using UnityEngine;

/// <summary>
/// 座標管理をする用の追加機能.
/// </summary>
namespace KR.Unity.Position
{
    /// <summary>
    /// 上下左右.
    /// </summary>
    public struct LBRT
    {
        public float left;
        public float bottom;
        public float right;
        public float top;

        //コンストラクタ.
        public LBRT(float _l, float _b, float _r, float _t)
        {
            left   = _l;
            bottom = _b;
            right  = _r;
            top    = _t;
        }
    }

    /// <summary>
    /// static関数.
    /// </summary>
    public static class Func
    {
        /// <summary>
        /// 画面の上下左右の座標を取得.
        /// </summary>
        public static LBRT GetWindowLBRT()
        {
            //カメラ取得.
            Camera cam = Camera.main;
            //カメラの描画領域を取得.
            Rect rect = cam.pixelRect;
            //画面の左上から右下座標を取得.
            Vector3 lb = cam.ScreenToWorldPoint(new Vector3(rect.xMin, rect.yMin));
            Vector3 rt = cam.ScreenToWorldPoint(new Vector3(rect.xMax, rect.yMax));

            return new LBRT(lb.x, lb.y, rt.x, rt.y);
        }

        /// <summary>
        /// 移動可能範囲内に補正する.
        /// </summary>
        /// <param name="pos">オブジェクト座標</param>
        /// <param name="size">オブジェクトサイズ</param>
        /// <param name="lim">限界座標(上下左右)</param>
        /// <returns>補正済座標</returns>
        public static Vector3KR FixPosInArea(Vector3KR pos, Vector2 size, LBRT lim)
        {
            if (pos.x < lim.left   + size.x/2) { pos.x = lim.left   + size.x/2; }
            if (pos.y < lim.bottom + size.y/2) { pos.y = lim.bottom + size.y/2; }
            if (pos.x > lim.right  - size.x/2) { pos.x = lim.right  - size.x/2; }
            if (pos.y > lim.top    - size.y/2) { pos.y = lim.top    - size.y/2; }

            return pos;
        }

        /// <summary>
        /// ローカル座標をワールド座標に変換.
        /// </summary>
        /// <param name="obj">親オブジェクト</param>
        /// <param name="lPos">ローカル座標</param>
        /// <returns>ワールド座標</returns>
        public static Vector2 LPosToWPos(GameObject obj, Vector2 lPos)
        {
            var wPos = obj.transform.TransformPoint(lPos);
            return wPos;
        }

        /// <summary>
        /// ワールド座標をローカル座標に変換.
        /// </summary>
        /// <param name="obj">親オブジェクト</param>
        /// <param name="wPos">ワールド座標</param>
        /// <returns>ローカル座標</returns>
        public static Vector2 WPosToLPos(GameObject obj, Vector2 wPos)
        {
            var lPos = obj.transform.InverseTransformPoint(wPos);
            return lPos;
        }
    }

    /// <summary>
    /// Vector3の機能拡張.
    /// </summary>
    public class Vector3KR
    {
        private Vector3 vec; //本体データ.

        //x,y,zのアクセス.
        public float x { get => vec.x; set => vec.x = value; }
        public float y { get => vec.y; set => vec.y = value; }
        public float z { get => vec.z; set => vec.z = value; }

        //コンストラクタ.
        public Vector3KR()
            => vec = Vector3.zero;
        public Vector3KR(Vector3 v)
            => vec = v;
        public Vector3KR(float x, float y, float z)
            => vec = new Vector3(x, y, z);

        //get.
        public Vector3 GetVector3() => vec;

        //「=」演算子用.
        public static implicit operator Vector3(Vector3KR v) => v.vec;
        public static implicit operator Vector3KR(Vector3 v) => new Vector3KR(v);
        //「+」演算子用.
        public static Vector3KR operator+(Vector3KR a, Vector3 b) => new Vector3KR(a.vec + b);
        //「-」演算子用.
        public static Vector3KR operator-(Vector3KR a, Vector3 b) => new Vector3KR(a.vec - b);
    }
}