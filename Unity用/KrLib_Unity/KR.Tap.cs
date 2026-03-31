/*
   - KR.Tap - (Unity)
   ver.2026/03/24
*/
using UnityEngine;
using KR.Unity.Timer;

/// <summary>
/// タップ管理をする用の追加機能.
/// </summary>
namespace KR.Unity.Tap
{
    /// <summary>
    /// タップしたかどうかを管理する.
    /// </summary>
    public class TapKR
    {
        private bool    isError;         //エラーチェック用.

        private bool    isTap;           //タップしているか.
        private bool    isDblTap;        //ダブルタップしているか.
        private bool    isSwiped;        //スワイプ済か.
        private bool    isSwipeJudged;   //スワイプ判定済か.

        private TimerKR tmDblTap;        //ダブルタップ猶予計測用.
        private TimerKR tmSwipe;         //スワイプ　　猶予計測用.

        private Vector2 startTapPos;     //タップ開始位置.
        private Vector2 endTapPos;       //タップ終了位置.

        private Vector2 swipeVec;        //スワイプした方向.
        private float   swipePow;        //スワイプした強さ.

        //get.
        public bool    IsTap       { get => isTap; }
        public bool    IsDblTap    { get => isDblTap; }
        public Vector2 StartTapPos { get => startTapPos; }
        public Vector2 EndTapPos   { get => endTapPos; }
        public Vector2 SwipeVec    { get => swipeVec; }
        public float   SwipePow    { get => swipePow; }

        /// <summary>
        /// コンストラクタ.
        /// </summary>
        /// <param name="_timeDblTap">ダブルタップ判定時間</param>
        /// <param name="_timeSwipe">スワイプ判定時間</param>
        public TapKR(float _timeDblTap, float _timeSwipe)
        {
            tmDblTap = new TimerKR(_timeDblTap);
            tmSwipe  = new TimerKR(_timeSwipe);

            isError = true; //最初はエラー判定ON.
        }

        /// <summary>
        /// タップデータの更新.
        /// </summary>
        public void Update()
        {
            OffErrorMode();

            tmDblTap.TimerDown(); //タイマー減少.

            //タップ中で、まだスワイプが完了していなければ.
            if (isTap && !isSwiped) 
            {
                tmSwipe.TimerDown(); //タイマー減少.
                //スワイプ猶予が終わったら.
                if (tmSwipe.Time <= 0)
                {
                    SwipeJudge(); //タップ終了する前に判定を行う.
                }
            }
        }

        /// <summary>
        /// 押下時にこれを実行させる.
        /// </summary>
        public void TapDown()
        {
            CheckError();

            isTap    = true;  //タップ開始.
            isSwiped = false; //スワイプ処理未完了.

            tmSwipe.Reset();                        //計測開始.
            startTapPos = Input.Func.GetMousePos(); //座標保存.

            //一定時間内に押していれば.
            if (tmDblTap.Time > 0) {
                isDblTap = true;   //ダブルタップした.
                tmDblTap.Time = 0; //リセット.
            }
            else {
                tmDblTap.Reset(); //計測開始.
            }
        }

        /// <summary>
        /// 押上時にこれを実行させる.
        /// </summary>
        public void TapUp()
        {
            CheckError();

            isTap         = false; //タップ終了.
            isDblTap      = false; //ダブルタップ終了.
            isSwipeJudged = false; //再判定可能に.

            endTapPos = Input.Func.GetMousePos(); //座標保存.

            //スワイプ猶予が残っている(=判定がまだなら)
            if (tmSwipe.Time > 0)
            {
                SwipeJudge(); //ここでスワイプ判定.
            }
        }

        /// <summary>
        /// スワイプが完了すると1度切りでtrueになる.
        /// </summary>
        public bool IsSwipedOnce()
        {
            if (isSwiped)
            {
                isSwiped = false; //以降はfalseに.
                return true;      //スワイプ完了.
            }
            return false; //スワイプ未完了.
        }

        /// <summary>
        /// スワイプ判定.
        /// </summary>
        private void SwipeJudge()
        {
            //スワイプ判定してないなら.
            if (!isSwipeJudged)
            {
                //現在地と開始位置の差.
                Vector2 dis = Input.Func.GetMousePos() - startTapPos;
                //移動していれば.
                if (dis != Vector2.zero)
                {
                    swipeVec = dis.normalized; //スワイプ方向.
                    swipePow = dis.magnitude;  //スワイプ力.
                }
                //全く同じ位置なら.
                else
                {
                    swipeVec = Vector2.zero;
                    swipePow = 0;
                }
                //スワイプ済み.
                isSwiped = true;
                isSwipeJudged = true;
            }
        }

        /// <summary>
        /// エラーチェック.
        /// </summary>
        private void CheckError()
        {
            //エラーメッセージ.
            if (isError) { Debug.LogError("[MyTap] Update関数が実行されていません"); }
        }
        /// <summary>
        /// エラー処理解除.
        /// </summary>
        private void OffErrorMode()
        {
            //Updateが実行されればエラーにしない.
            if (isError) { isError = false; }
        }
    }
}