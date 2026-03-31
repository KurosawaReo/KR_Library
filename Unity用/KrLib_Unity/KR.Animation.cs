/*
   - KR.Animation - (Unity)
   ver.2026/03/30
*/
using System;
using UnityEngine;

/// <summary>
/// アニメーション用の追加機能.
/// </summary>
namespace KR.Unity.Animation
{
    /// <summary>
    /// プログラムで作るアニメーション用機能
    /// [継承想定]
    /// </summary>
    public class AnimProgKR : MonoBehaviour
    {
        float timer;                 //タイマー(0.0～1.0)
        float maxSec;                //アニメーションが完了するまでの時間(秒)
        Func<float, float> easeFunc; //タイマー(0.0～1.0)の進み方を変化させる関数(イージング)

        //get.
        public float GetTimer() => easeFunc(timer);

        /// <summary>
        /// アニメーションが終了したか.
        /// </summary>
        public bool IsFinished() => timer >= 1.0f;

        /// <summary>
        /// 初期化処理.
        /// </summary>
        /// <param name="_maxSec">アニメーション完了時間(秒)</param>
        /// <param name="_ease">イージング関数</param>
        public void InitAnim(float _maxSec, Func<float, float> _easeFunc)
        {
            maxSec = _maxSec;
            easeFunc = _easeFunc;
        }

        /// <summary>
        /// リセット処理.
        /// </summary>
        public void ResetAnim()
        {
            timer = 0.0f;
        }

        /// <summary>
        /// 更新処理.
        /// </summary>
        public void UpdateAnim()
        {
            //maxSec秒かけてタイマー進行.
            timer += Time.deltaTime / maxSec;
            //0.0～1.0に制限.
            timer = Mathf.Clamp01(timer);
        }

        /// <summary>
        /// 消去する.
        /// </summary>
        public void Delete()
        {
            Destroy(gameObject);
        }
    }

    /// <summary>
    /// UnityのAnimationで作るアニメーション用機能
    /// [継承想定]
    /// </summary>
    public class AnimUnityKR : MonoBehaviour
    {
        Animator animr; //Animatorコンポーネント.

        /// <summary>
        /// 初期化処理.
        /// </summary>
        public void InitAnim()
        {
            animr = GetComponent<Animator>();
        }

        /// <summary>
        /// アニメーションの再生時間を取得.
        /// </summary>
        /// <returns>再生時間</returns>
        public float GetTimer()
        {
            float ret = -1.0f;

            if (animr)
            {
                //Animatorの現在の状態を取得.
                AnimatorStateInfo animInfo = animr.GetCurrentAnimatorStateInfo(0);
                //再生時間を保存.
                ret = animInfo.normalizedTime;
            }
            return ret;
        }

        /// <summary>
        /// アニメーションが終了したか.
        /// </summary>
        public bool IsFinished()
        {
            return GetTimer() > 1.0f; //1.0(=100%)を越えたら終了済.
        }

        /// <summary>
        /// 消去する.
        /// </summary>
        public void Delete()
        {
            Destroy(gameObject);
        }
    }
}