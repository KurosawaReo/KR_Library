/*
   - KR.RegisterScript - (Unity)
   ver.2026/03/30
*/
using UnityEngine;

/// <summary>
/// KR_Libで使うScriptableObject集.
/// </summary>
namespace KR.Unity.RegisterScript
{
    /// <summary>
    /// 使用するprefabのパーツを登録する.
    /// Create > MyTools > MapPartsで新しく出せる.
    /// </summary>
    [CreateAssetMenu(fileName = "New Parts", menuName = "MyTools/MapParts")]
    public class MapParts : ScriptableObject
    {
        public GameObject[] prefabs; //これでGameObjectを登録できる.
    }
}