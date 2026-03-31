/*
   - KR.Object - (Unity)
   ver.2026/03/30
*/
using UnityEngine;
using UE = UnityEngine;
using System;

using KR.Unity.Position;
using KR.Unity.Variable;
using KR.Unity.Inspector;

/// <summary>
/// オブジェクト管理用の追加機能.
/// </summary>
namespace KR.Unity.Object
{
    /// <summary>
    /// コンポーネント集.
    /// </summary>
    public class Components
    {
        public SpriteRenderer sr;
        public Rigidbody2D    rb2d;
        public Animator       animr;
    }

    /// <summary>
    /// static関数.
    /// </summary>
    public static class Func
    {
        /// <summary>
        /// 指定された名前の子オブジェクトを検索して返す.
        /// </summary>
        public static Transform FindChildByName(this Transform parent, string name)
        {
            foreach (Transform child in parent)
            {
                if (child.name == name)
                {
                    return child;
                }
            }
            return null;
        }
    }

    /// <summary>
    /// オブジェクトデータ(2D用)[継承想定]
    /// </summary>
    [RequireComponent(typeof(SpriteRenderer))]
    public class ObjectKR : MonoBehaviour
    {
    //▼コンポーネント.
        protected Components cmp;

    //▼private変数.
        private Vector3KR pos;      //座標データ.
        private Vector3KR vel;      //速度データ.

        private Vector3   facing;   //向いてる方向.
        private bool      isActive; //有効かどうか.
        private bool      isFlip;   //反転するかどうか.
        private bool      isGround; //着地しているか.

    //▼private変数.[入力可]
        [Header("- ObjectKR -")] 
        [Space(4)]
        [SerializeField] 
            private bool    isAutoInit = true;                      //自動で値を初期化するか.
        [InspectorDisable("isAutoInit", false, true), SerializeField] 
            private Vector3 initPos;                                //初期座標.
        [InspectorDisable("isAutoInit", false, true), SerializeField] 
            private Vector3 initFacing;                             //初期向き.
        [InspectorDisable("isAutoInit", false, true), SerializeField] 
            private bool    initActive = true;                      //有効かどうか.
        [InspectorDisable("isAutoInit", false, true), SerializeField] 
            private bool    initFlip   = false;                     //反転するかどうか.

        [SerializeField] private Vector2 size = new Vector2(1, 1);  //当たり判定サイズ.
        [SerializeField] private IntR    hp;                        //体力.

    //▼public.
        //set, get.
        public Vector3KR Pos {
            get => TryGet(pos);
            set => pos = value;
        }
        public Vector3KR Vel {
            get => TryGet(vel);
            set => vel = value;
        }
        public float Gravity {
            get           => TryGet(cmp).rb2d.gravityScale;
            protected set => TryGet(cmp).rb2d.gravityScale = value;
        }
        public Vector2 Size {
            get => size;
        }
        public Vector2 Facing {
            get => facing;
        }
        public Color Color {
            get           => TryGet(cmp).sr.color;
            protected set => TryGet(cmp).sr.color = value;
        }
        public int Hp {
            get => hp.Now;
            protected set => hp.Now = value;
        }
        public bool IsActive {
            get => isActive; 
            protected set {
                isActive = value;
                gameObject.SetActive(value); //設定.
            }
        }
        public bool IsFlip {
            get => isFlip;
            protected set {
                isFlip = value;
                TryGet(cmp).sr.flipX = value; //設定.
            }
        }
        public bool IsGround
        {
            get => isGround;
            protected set => isGround = value;
        }

        /// <summary>
        /// ObjectKR初期化.
        /// </summary>
        public void InitObjKR()
        {
            //初期化.
            pos = new Vector3KR();
            vel = new Vector3KR();
            cmp = new Components();
            //コンポーネント取得.
            cmp.sr    = GetComponent<SpriteRenderer>();
            cmp.rb2d  = GetComponent<Rigidbody2D>();
            cmp.animr = GetComponent<Animator>();
            //サイズ取得.
            size = new Vector2(cmp.sr.bounds.size.x * size.x, cmp.sr.bounds.size.y * size.y);
            //自動初期化モードなら.
            if (isAutoInit)
            {
                initPos    = transform.position;     //初期座標登録.
                initFacing = new Vector2(1, 0);      //右.
                initActive = gameObject.activeSelf;  //アクティブ状態取得.
                initFlip   = cmp.sr.flipX;           //反転状態取得.
            }
            else
            {
                ResetObjKR(); //リセット処理.
            }
        }

        /// <summary>
        /// ObjectKRリセット.
        /// </summary>
        public void ResetObjKR()
        {
            IsActive = initActive;
            isGround = false;
            hp.Reset();
            ResetPos();
        }
        /// <summary>
        /// 位置だけリセット.
        /// </summary>
        public void ResetPos()
        {
            Pos    = initPos;
            facing = initFacing;
            IsFlip = initFlip;
        }

        /// <summary>
        /// ObjectKR更新.
        /// </summary>
        public void UpdateObjKR()
        {
            transform.position = Pos;
        }

        /// <summary>
        /// 移動処理.
        /// </summary>
        public void Move(Vector2 vec, float speed)
        {
            //移動.
            Pos += vec * speed * Time.deltaTime;
            //方向の保存.
            facing = vec;
        }
        /// <summary>
        /// 移動処理.
        /// </summary>
        /// <param name="lim">限界座標</param>
        public void Move(Vector3 vec, float speed, LBRT lim)
        {
            //移動.
            Pos += vec * speed * Time.deltaTime;
            Pos = Position.Func.FixPosInArea(pos, size, lim); //移動限界.
            //方向の保存.
            facing = vec;
        }

        /// <summary>
        /// 移動速度を与える.
        /// </summary>
        public void AddForce(Vector3 vec, float pow)
        {
            cmp.rb2d.AddForce(vec * pow);
        }
        /// <summary>
        /// 移動速度を与える.
        /// </summary>
        /// <param name="mode">力を与えるモード</param>
        public void AddForce(Vector2 vec, float pow, ForceMode2D mode)
        {
            cmp.rb2d.AddForce(vec * pow, mode);
        }

        /// <summary>
        /// ジャンプ可能ならジャンプする.
        /// </summary>
        public bool TryJump(float pow)
        {
            //着地しているなら.
            if (isGround)
            {
                //瞬間的に上に力を加える.
                AddForce(Vector2.up, pow, ForceMode2D.Impulse);

                isGround = false; //ジャンプしている.
                return true;      //ジャンプ成功.
            }
            return false; //ジャンプ失敗.
        }

        /// <summary>
        /// Animatorのパラメーターをセット(Trigger)
        /// </summary>
        public void SetAnimParam(string name)
        {
            cmp.animr.SetTrigger(name);
        }
        /// <summary>
        /// Animatorのパラメーターをセット(Bool)
        /// </summary>
        public void SetAnimParam(string name, bool value)
        {
            cmp.animr.SetBool(name, value);
        }
        /// <summary>
        /// Animatorのパラメーターをセット(Int)
        /// </summary>
        public void SetAnimParam(string name, int value)
        {
            cmp.animr.SetInteger(name, value);
        }
        /// <summary>
        /// Animatorのパラメーターをセット(Float)
        /// </summary>
        public void SetAnimParam(string name, float value)
        {
            cmp.animr.SetFloat(name, value);
        }

        /// <summary>
        /// 値のgetを試みる(初期化忘れ対策)
        /// </summary>
        public T TryGet<T>(T value)
        {
            return Variable.Func.TryGet(value, "[ObjectKR] InitObjKR関数を実行していません");
        }
    }

    /// <summary>
    /// prefabを扱う用.
    /// </summary>
    [Serializable] public class PrefabKR
    {
        [SerializeField] GameObject prefab; //prefabデータ.
        [SerializeField] GameObject inObj;  //prefabを入れる所.

        /// <summary>
        /// prefab新規作成.
        /// </summary>
        /// <returns>作成したprefab</returns>
        public GameObject NewPrefab()
        {
            GameObject obj = null;

            if (prefab) {
                obj = UE.Object.Instantiate(prefab);      //生成.
            }
            if (inObj) {
                obj.transform.SetParent(inObj.transform); //親オブジェクトを設定.
            }
            return obj;
        }
    }
}