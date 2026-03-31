/*
   - KR.Game - (Unity)
   ver.2026/03/24
*/
using UnityEngine.SceneManagement;

/// <summary>
/// ゲーム全体で使う機能.
/// </summary>
namespace KR.Unity.Game
{
    /// <summary>
    /// static関数.
    /// </summary>
    public static class Func
    { 
        /// <summary>
        /// シーン切り替え.
        /// </summary>
        public static void ChangeScene(string sceneName)
        {
            if (!string.IsNullOrEmpty(sceneName))
            {
                SceneManager.LoadScene(sceneName); //シーン移動.
            }
        }
    }
}