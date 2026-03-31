/*
   - KR.Variable - (Unity)
   ver.2026/03/30
*/
using UnityEngine;
using System;

/// <summary>
/// 変数用の追加機能.
/// </summary>
namespace KR.Unity.Variable
{
    /// <summary>
    /// 範囲ありint型変数.
    /// </summary>
    [Serializable] public struct IntR //int range.
    {
        [SerializeField] private int max;  //最大値.
        [SerializeField] private int min;  //最小値.
        [SerializeField] private int init; //初期化値.
        private int now; //現在値.

        //set, get.
        public int Now //現在値.
        {
            get => now;
            set {
                now = value;
                if (now > max) { now = max; } //最大に設定.
                if (now < min) { now = min; } //最小に設定.
            }
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public IntR(int _max, int _min, int _init)
        {
            max  = _max;
            min  = _min;
            init = _init;
            now  = _init; //初期値に設定.
        }
        /// <summary>
        /// リセット.
        /// </summary>
        public void Reset()
        {
            now = init;
        }
    }

    /// <summary>
    /// 範囲ありfloat型変数.
    /// </summary>
    [Serializable] public struct FloatR //float range.
    {
        [SerializeField] private float max;  //最大値.
        [SerializeField] private float min;  //最小値.
        [SerializeField] private float init; //初期化値.
        private float now; //現在値.

        //set, get.
        public float Now //現在値.
        {
            get => now;
            set {
                now = value;
                if (now > max) { now = max; } //最大に設定.
                if (now < min) { now = min; } //最小に設定.
            }
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public FloatR(float _max, float _min, float _init)
        {
            max  = _max;
            min  = _min;
            init = _init;
            now  = _init; //初期値に設定.
        }
        /// <summary>
        /// リセット.
        /// </summary>
        public void Reset()
        {
            now = init;
        }
    }

    /// <summary>
    /// Variable関数.
    /// </summary>
    public static class Func
    {
        /// <summary>
        /// 値のgetを試みる(初期化忘れ対策)
        /// </summary>
        public static T TryGet<T>(T value, string errorMng)
        {
            //nullならエラーを出す.
            if (value == null) {
                Debug.LogError(errorMng);
            }
            return value;
        }

        /*
           where T : IComparable<T>
           →大小比較できる(CompareToが使える)型だけOK

           [CompareTo()の返り値]
           +1: 大きい
            0: 同じ
           -1: 小さい
        */

        /// <summary>
        /// 値が範囲内に含まれるか判定.
        /// </summary>
        public static bool IsInRange<T>(T value, T min, T max) where T : System.IComparable<T>
        {
            return value.CompareTo(min) >= 0 && value.CompareTo(max) <= 0;
        }
    }
}