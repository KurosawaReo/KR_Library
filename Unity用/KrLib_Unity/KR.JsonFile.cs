/*
   - KR.JsonFile - (Unity)
   ver.2026/03/24
*/
using UnityEngine;
using System.IO;

/// <summary>
/// Jsonファイルを扱う用の追加機能.
/// </summary>
namespace KR.Unity.JsonFile
{
    /// <summary>
    /// static定数.
    /// </summary>
    public static class Const
    {
        //Jsonファイルの保存パス.
        public const string FILE_PATH_EDITOR = "/Jsons/";
        public const string FILE_PATH_BUILD  = "/MySaveData/";
    }

    /// <summary>
    /// static関数.
    /// </summary>
    public static class Func
    {
        /// <summary>
        /// 保存するパスを取得.
        /// Jsonファイルがまだない場合は新規作成する.
        /// </summary>
        /// <param name="_fileName">Jsonファイル名</param>
        /// <returns>適切なパス</returns>
        public static string GetFilePath(string _fileName)
        {
            //Unityエディタ実行中.
            if (Application.isEditor)
            {
                //Assetsの中に保存.
                return Application.dataPath + Const.FILE_PATH_EDITOR + _fileName;
            }
            //ビルド後.
            else
            {
                //OSごとの安全な場所.
                return Application.persistentDataPath + Const.FILE_PATH_BUILD + _fileName;
            }
        }

        /// <summary>
        /// ファイルが存在するか.
        /// </summary>
        /// <param name="_fileName">ファイル名</param>
        /// <returns>判定結果</returns>
        public static bool IsExistFile(string _fileName)
        {
            return File.Exists(_fileName);
        }

        /// <summary>
        /// Jsonファイルの読み込み.
        /// </summary>
        /// <typeparam name="T">データの型(classなど)</typeparam>
        /// <param name="_path">ファイルのパス</param>
        /// <returns>読み込んだデータ</returns>
        public static T LoadJsonFile<T>(string _path)
        {
            try
            {
                string strJson = File.ReadAllText(_path); //読み込み.
                return JsonUtility.FromJson<T>(strJson);  //読み込んだデータを返す.
            }
            //エラー処理.
            catch (IOException error)
            {
                Debug.LogError("[Error] failed to load json file: " + error.Message);

                return default(T); //多分nullのようなものを返す.
            }
        }

        /// <summary>
        /// Jsonファイルへ保存(上書き)
        /// </summary>
        /// <typeparam name="T">データの型(classなど)</typeparam>
        /// <param name="_data">保存するデータ(型は自由)</param>
        /// <param name="_path">ファイルのパス</param>
        public static void SaveJsonFile<T>(T _data, string _path)
        {
            try
            {
                string strJson = JsonUtility.ToJson(_data, true); //データをJsonに変換.
                File.WriteAllText(_path, strJson);                //Jsonファイルに書き込み.
            }
            //エラー処理.
            catch (IOException error)
            {
                Debug.LogError("[Error] failed to save json file: " + error.Message);
            }
        }
    }
}