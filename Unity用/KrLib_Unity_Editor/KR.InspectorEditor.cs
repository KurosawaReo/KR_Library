/*
   - KR.InspectorEditor - (Unity)
   ver.2026/03/20

   セットで使用: KR.Inspector
   フォルダ: Assets/Editorに入れる
*/
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

using KR.Unity.Inspector;

/// <summary>
/// インスペクター用の追加機能.
/// </summary>
namespace KR.Unity.InspectorEditor
{
    /// <summary>
    /// [InspectorDisable()]
    /// </summary>
    [CustomPropertyDrawer(typeof(InspectorDisableAttribute))]
    internal sealed class ConditionalDisableDrawer : PropertyDrawer
    {
    //▼メンバ.
        private delegate bool GetCondFunc(SerializedProperty prop, InspectorDisableAttribute attr);

        //???
        private Dictionary<Type, GetCondFunc> DisableCondFuncMap = new Dictionary<Type, GetCondFunc>() {
            { typeof(bool),   (prop, attr) => { return attr.isDisable ? !prop.boolValue : prop.boolValue;} },
            { typeof(string), (prop, attr) => { return attr.isDisable ? prop.stringValue == attr.compStr   : prop.stringValue != attr.compStr;  } },
            { typeof(int),    (prop, attr) => { return attr.isDisable ? prop.intValue    == attr.compInt   : prop.intValue    != attr.compInt;  } },
            { typeof(float),  (prop, attr) => { return attr.isDisable ? prop.floatValue  <= attr.compFloat : prop.floatValue  >  attr.compFloat;} }
        };

    //▼public関数.

        /// <summary>
        /// ???
        /// </summary>
        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            //???
            var attr = attribute as InspectorDisableAttribute;
            //???
            SerializedProperty prop;

            //パスを辿る, これがないとclassのメンバにアクセスできない.
            {
                string parentPath = property.propertyPath; //パス.
                int lastDot = parentPath.LastIndexOf('.'); //最後のドットを探す.
            
                //最後のドットがあれば.
                if (lastDot >= 0)
                {
                    string parent = parentPath.Substring(0, lastDot); //例:player
                    prop = property.serializedObject.FindProperty($"{parent}.{attr.varName}"); //親を含むパス.
                }
                else
                {
                    prop = property.serializedObject.FindProperty(attr.varName); //そのままのパス.
                }
                //値なしエラー.
                if (prop == null)
                {
                    Debug.LogError($"Not found '{attr.varName}' property");
                    EditorGUI.PropertyField(position, property, label, true); //???
                }
            }

            //???
            var isDisable = IsDisable(attr, prop);
            if (attr.isInvisiOffMode && isDisable)
            {
                return; //この先の処理をしない.
            }

            EditorGUI.BeginDisabledGroup(isDisable);                  //begin
            EditorGUI.PropertyField(position, property, label, true); //???
            EditorGUI.EndDisabledGroup();                             //end
        }

        /// <summary>
        /// Inspectorで入力欄を表示する高さを取得.
        /// </summary>
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            //???
            var attr = attribute as InspectorDisableAttribute;
            //???
            SerializedProperty prop;

            //パスを辿る, これがないとclassのメンバにアクセスできない.
            {
                string parentPath = property.propertyPath; //パス.
                int lastDot = parentPath.LastIndexOf('.'); //最後のドットを探す.
                //最後のドットがあれば.
                if (lastDot >= 0)
                {
                    string parent = parentPath.Substring(0, lastDot); //例:player
                    prop = property.serializedObject.FindProperty($"{parent}.{attr.varName}"); //親を含むパス.
                }
                else
                {
                    prop = property.serializedObject.FindProperty(attr.varName); //そのままのパス.
                }
            }
            //適切な高さを返す.
            if (attr.isInvisiOffMode && IsDisable(attr, prop))
            {
                return -EditorGUIUtility.standardVerticalSpacing;   //非表示時の表示位置.
            }
            else
            {
                return EditorGUI.GetPropertyHeight(property, true); //通常の表示位置.
            }
        }

        /// <summary>
        /// 無効かどうか取得.
        /// </summary>
        private bool IsDisable(InspectorDisableAttribute attr, SerializedProperty prop)
        {
            //何かを入れる用の変数.
            GetCondFunc condFunc;
            
            //???の取得を試みる.
            bool ret = DisableCondFuncMap.TryGetValue(attr.varType, out condFunc);
            if (!ret) //失敗エラー.
            {
                Debug.LogError($"{attr.varType} type is not supported");
                return false;
            }
            //結果を返す.
            return condFunc(prop, attr);
        }
    }

    /// <summary>
    /// [ReadOnly]
    /// </summary>
    [CustomPropertyDrawer(typeof(ReadOnlyAttribute))]
    public class ReadOnlyDrawer : PropertyDrawer
    {
        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            //編集不可にする.
            GUI.enabled = false;
            //Inspector描画.
            EditorGUI.PropertyField(position, property, label, true);
            //元に戻す.
            GUI.enabled = true;
        }
    }
}