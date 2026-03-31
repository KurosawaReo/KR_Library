/*
   - KR.Inspector - (Unity)
   ver.2026/03/20

   セットで使用: KR.InspectorEditor
   フォルダ: 自由

   ↓参考サイト
   https://mu-777.hatenablog.com/entry/2022/09/04/113850
*/
/*
   [仕組み]
   ～Attributeクラスと、～Drawerクラスがある。 
   Attributeの方で値を保存し、Drawerの方で値を読み取る.
*/
using System;
using UnityEngine;

/// <summary>
/// インスペクター用の追加機能.
/// </summary>
namespace KR.Unity.Inspector
{
    /// <summary>
    /// [InspectorDisable()]
    /// これをつけると、条件付きで変数を有効にする.
    /// </summary>
    public partial class InspectorDisableAttribute : PropertyAttribute
    {
    //▼メンバ.
        public readonly string varName;          //変数名.
        public readonly Type   varType;          //変数型.
        public readonly bool   isDisable;        //無効かどうか.
        public readonly bool   isInvisiOffMode;  //offなら非表示にするか.

        public readonly string compStr;      //string型の比較値.
        public readonly int    compInt;      //int   型の比較値.
        public readonly float  compFloat;    //float 型の比較値.

        //▼コンストラクタ.
        /// <summary>
        /// メインのコンストラクタ(全ての型対応)
        /// </summary>
        private InspectorDisableAttribute(
            string _varName, Type _varType, bool _isDisable = false, bool _isInvisi = false
        ){
            varName         = _varName;
            varType         = _varType;
            isDisable       = _isDisable;
            isInvisiOffMode = _isInvisi;
        }
        /// <summary>
        /// bool型.
        /// </summary>
        /// <param name="_varName">変数名</param>
        /// <param name="_isReverseCondi">条件を反転させるか</param>
        /// <param name="_isInvisi">offなら非表示にするか</param>
        public InspectorDisableAttribute(
            string _varName, bool _isReverseCondi = false, bool _isInvisi = false
        )
            : this(_varName, typeof(bool), _isReverseCondi, _isInvisi) //自身のコンストラクタへ.
        {}
        /// <summary>
        /// string型.
        /// </summary>
        /// <param name="_varName">変数名</param>
        /// <param name="_value">値</param>
        /// <param name="_isReverseCondi">falseなら"value一致", trueなら"value不一致"</param>
        /// <param name="_isInvisi">offなら非表示にするか</param>
        public InspectorDisableAttribute(
            string _varName, string _value, bool _isReverseCondi = false, bool _isInvisi = false
        )
            : this(_varName, _value.GetType(), _isReverseCondi, _isInvisi) //自身のコンストラクタへ.
        {
            compStr = _value;
        }
        /// <summary>
        /// int型.
        /// </summary>
        /// <param name="_varName">変数名</param>
        /// <param name="_value">値</param>
        /// <param name="_isReverseCondi">falseなら"value一致", trueなら"value不一致"</param>
        /// <param name="_isInvisi">offなら非表示にするか</param>
        public InspectorDisableAttribute(
            string _varName, int _value, bool _isReverseCondi = false, bool _isInvisi = false
        )
            : this(_varName, _value.GetType(), _isReverseCondi, _isInvisi) //自身のコンストラクタへ.
        {
            compInt = _value;
        }
        /// <summary>
        /// float型.
        /// </summary>
        /// <param name="_varName">変数名</param>
        /// <param name="_value">値</param>
        /// <param name="_isGreaterCondi">falseなら"value以下", trueなら"value以上"</param>
        /// <param name="_isInvisi">offなら非表示にするか</param>
        public InspectorDisableAttribute(
            string _varName, float _value, bool _isGreaterCondi = true, bool _isInvisi = false
        )
            : this(_varName, _value.GetType(), _isGreaterCondi, _isInvisi) //自身のコンストラクタへ.
        {
            compFloat = _value;
        }
    }

    /// <summary>
    /// [ReadOnly]
    /// これをつけると、Inspectorで編集不可にする.
    /// </summary>
    public class ReadOnlyAttribute : PropertyAttribute
    {
        //中身なし.
    }
}