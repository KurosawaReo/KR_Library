/*
   - KR.Input - (Unity)
   ver.2026/03/24
*/
using System;
using UnityEngine;
using UnityEngine.InputSystem;
using UE = UnityEngine; //別名で使えるように.

/// <summary>
/// 座標管理をする用の追加機能.
/// </summary>
namespace KR.Unity.Input
{
    /// <summary>
    /// マウスボタン判定用.
    /// </summary>
    public enum MouseID
    { 
        Left,       //左クリック.
        Right,      //右クリック.
        Middle,     //ホイール.
        SideFront,  //横の手前.
        SideBack,   //横の奥.
    }

    /// <summary>
    /// static関数.
    /// </summary>
    public static partial class Func
    {
        /// <summary>
        /// マウスクリック判定.
        /// </summary>
        public static bool IsPushMouse(MouseID id)
        {
            return UE.Input.GetMouseButton((int)id);
        }
        /// <summary>
        /// マウスクリック判定.
        /// </summary>
        public static bool IsPushMouseDown(MouseID id)
        {
            return UE.Input.GetMouseButtonDown((int)id);
        }
        /// <summary>
        /// マウスクリック判定.
        /// </summary>
        public static bool IsPushMouseUp(MouseID id)
        {
            return UE.Input.GetMouseButtonUp((int)id);
        }

        /// <summary>
        /// マウス座標取得.
        /// </summary>
        public static Vector2 GetMousePos()
        {
            Vector2 mPos = UE.Input.mousePosition;
            Vector2 wPos = Camera.main.ScreenToWorldPoint(mPos);

            return wPos;
        }
        /// <summary>
        /// 上下左右の操作取得.
        /// </summary>
        public static Vector2 GetMove4dir()
        {
            Vector2 input = new Vector2(
                UE.Input.GetAxisRaw("Horizontal"), //水平方向入力.
                UE.Input.GetAxisRaw("Vertical")    //垂直方向入力.
            );
            return input;
        }
    }

    /// <summary>
    /// Input管理(InputSystem用)
    /// </summary>
    public class InputMngKR
    {
        //InputActionファイル.
        InputActionAsset actionFile;
        //入力があった時に呼ばれる関数.
        event Action<string, InputAction.CallbackContext> onActionEvent;
        //関数アドレス保存用.
        Action<string, InputAction.CallbackContext> func;

        /// <summary>
        /// コンストラクタ.
        /// </summary>
        /// <param name="_actionFile">InputActionファイル</param>
        /// <param name="_func">関数アドレス</param>
        public InputMngKR(InputActionAsset _actionFile, Action<string, InputAction.CallbackContext> _func)
        {
            onActionEvent += _func;

            actionFile = _actionFile;
            func       = _func;

            Enable(); //有効にする処理.
        }

        /// <summary>
        /// 有効にする.
        /// </summary>
        private void Enable()
        {
            //全てのactionMapsとactionsをループ.
            foreach (var i in actionFile.actionMaps)
            {
                foreach (var j in i.actions)
                {
                    j.Enable();
                    //「?.Invoke」= もし登録されていたら呼ぶ.
                    j.performed += ctx => onActionEvent?.Invoke(j.name, ctx);
                    j.canceled  += ctx => onActionEvent?.Invoke(j.name, ctx);
                }
            }
        }
        /// <summary>
        /// 無効にする.
        /// </summary>
        public void Disable()
        {
            //全てのactionMapsとactionsをループ.
            foreach (var i in actionFile.actionMaps)
            {
                foreach (var j in i.actions)
                {
                    j.Disable();
                }
            }
            onActionEvent -= func; //登録していた関数を解放.
        }
    }
}