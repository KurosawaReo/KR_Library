/*
   - KR.Timer - (Unity)
   ver.2026/03/24
*/
using UnityEngine;
using System.Collections;
using UE = UnityEngine; //別名で使えるように.

/// <summary>
/// 時間管理をする用の追加機能.
/// </summary>
namespace KR.Unity.Timer
{
    /// <summary>
    /// 時分秒.
    /// </summary>
    public struct TimerHMS
    {
        public int h;  //時.
        public int m;  //分.
        public int s;  //秒.
        public int cs; //コンマ秒.

        /// <summary>
        /// カウントダウン専用, 0秒になったかどうか.
        /// </summary>
        public bool IsEndTimer()
        {
            return (h <= 0 && m <= 0 && s <= 0 && cs <= 0);
        }
    }

    /// <summary>
    /// static関数.
    /// </summary>
    public static class Func
    {
        /// <summary>
        /// 秒を時分秒に変換.
        /// </summary>
        public static TimerHMS TimeToHMS(float _time)
        {
            TimerHMS tm = new TimerHMS();

            //時分秒の計算.
            tm.h = (int)_time / 3600;
            tm.m = (int)_time % 3600 / 60;
            tm.s = (int)_time % 3600 % 60;
            //コンマ秒の計算.
            tm.cs = (int)((_time - tm.h*3600 - tm.m*60 - tm.s)*100);

            return tm;
        }

        /// <summary>
        /// コルーチンの遅延用.
        /// </summary>
        /// <param name="_sec">待機する秒数</param>
        public static IEnumerator Delay(float _sec)
        {
            yield return new WaitForSeconds(_sec);
        }
    }

    /// <summary>
    /// タイマー管理用.
    /// </summary>
    public class TimerKR
    {
        private float now;  //計測時刻.
        private float init; //リセット時刻.
        //set, get.
        public float Time { 
            get => now; 
            set => now = value;
        }

        /// <summary>
        /// コンストラクタ.
        /// </summary>
        /// <param name="_initSec">リセット秒数</param>
        public TimerKR(float _initSec = 0)
        {
            now = init = _initSec;
        }
        /// <summary>
        /// タイマーリセット.
        /// </summary>
        public void Reset()
        {
            now = init;
        }
        /// <summary>
        /// タイマーを増やす.
        /// </summary>
        public void TimerUp()
        {
            //タイマー増加(1秒で+1)
            now += UE.Time.deltaTime;
        }
        /// <summary>
        /// タイマーを減らす.
        /// </summary>
        public void TimerDown()
        {
            //タイマー減少(1秒で-1)
            now -= (now > 0) ? UE.Time.deltaTime : 0;
        }
        /// <summary>
        /// 一定時間ごとにtrueを返す.
        /// (別途TimerDownを実行する必要あり)
        /// </summary>
        public bool IntervalTime()
        {
            //タイマーが0になったら.
            if (now <= 0) {
                Reset(); //タイマーリセット.
                return true;
            }
            else {
                return false;
            }
        }
    }
}