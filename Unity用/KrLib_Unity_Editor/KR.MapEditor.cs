/*
   - KR.MapEditor - (Unity)
   ver.2026/03/30

   フォルダ: Assets/Editorに入れる
*/
using UnityEngine;
using UnityEditor;
using System.IO;

using KR.Unity.Position;
using KR.Unity.Variable;
using KR.Unity.RegisterScript;

/// <summary>
/// マップ拡張エディタを追加する.
/// </summary>
namespace KR.Unity.MapEditor
{
    /// <summary>
    /// CSVでマップデータを管理する
    /// </summary>
    public class MapEditor : EditorWindow
    {
        MapParts mapParts; //パーツとして使うprefab(ScriptableObject)

        int[,]  mapData;   //マップの2D配列(番号で何を置くか管理)
        int     gridWid;   //グリッドの横幅.
        int     gridHei;   //グリッドの縦幅.
        float   gridSize;  //グリッドのサイズ.
        int     selectNo;  //現在選択中のprefab番号.
        Vector2 scrollPos; //スクロール位置.

        string  filePath;  //CSVファイルの保存先パス.

        /// <summary>
        /// エディタ画面を開く.
        /// </summary>
        [MenuItem("MyTools/CSV Map Editor")]
        public static void OpenWindow()
        {
            GetWindow<MapEditor>("CSV Map Editor");
        }

        /// <summary>
        /// 初期化処理, エディタ版のStart()のようなもの.
        /// </summary>
        private void OnEnable()
        {
            mapData = new int[gridWid, gridHei];
            filePath = "Assets/Resources/.csv"; //初期パス.
            gridSize = 1;
        }
        /// <summary>
        /// GUI設定.
        /// </summary>
        private void OnGUI()
        {
            MakeSettings();
            MakePrefabSelector();
            MakeMapGrid();
            MakeButtons();
        }

        /// <summary>
        /// エディタで設定できる欄を作る.
        /// </summary>
        void MakeSettings()
        {
            DrawSeparator();
            EditorGUILayout.LabelField("- Settings -", EditorStyles.boldLabel); //ラベルを表示する.

            filePath = EditorGUILayout.TextField ("File Path",   filePath);
            EditorGUILayout.Space(3); //GUIのスペースを作る.
            gridWid  = EditorGUILayout.IntField  ("Grid Width",  gridWid);
            gridHei  = EditorGUILayout.IntField  ("Gird Height", gridHei);
            gridSize = EditorGUILayout.FloatField("Grid Size",   gridSize);
            EditorGUILayout.Space(); //GUIのスペースを作る.

            //サイズが変わったら新しく配列を作る.
            if (mapData.GetLength(0) != gridWid || mapData.GetLength(1) != gridHei)
            {
                //サイズが小さい方を優先.
                int minX = Mathf.Min(gridWid, mapData.GetLength(0));
                int minY = Mathf.Min(gridHei, mapData.GetLength(1));
                //サイズが0でなければ.
                if (minX > 0 && minY > 0)
                {
                    int[,] tmpData = new int[minX, minY];
                    //仮保存.
                    for (int y = 0; y < minY; y++) {
                        for (int x = 0; x < minX; x++)
                        {
                            tmpData[x,y] = mapData[x,y];
                        }
                    }
                    //サイズ変更.
                    mapData = new int[gridWid, gridHei];
                    //元のデータを反映.
                    for (int y = 0; y < minY; y++) {
                        for (int x = 0; x < minX; x++)
                        {
                            mapData[x,y] = tmpData[x,y];
                        }
                    }
                }
                GenerateMap();
            }
            //マップがまだないなら作成.
            else if(mapData == null)
            {
                mapData = new int[gridWid, gridHei];
                GenerateMap();
            }
        }

        /// <summary>
        /// 設置するprefabを選択する欄を作る.
        /// </summary>
        void MakePrefabSelector()
        {
            DrawSeparator();
            EditorGUILayout.LabelField("- Parts -", EditorStyles.boldLabel); //ラベルを表示する.

            mapParts = (MapParts)EditorGUILayout.ObjectField("Map Parts", mapParts, typeof(MapParts), false);

            if (mapParts != null && mapParts.prefabs != null && mapParts.prefabs.Length > 0)
            {
                EditorGUILayout.Space(3);

                int columns = 4;
                int count = mapParts.prefabs.Length;
                int rows = Mathf.CeilToInt((float)count / columns);

                for (int y = 0; y < rows; y++)
                {
                    EditorGUILayout.BeginHorizontal();
                    for (int x = 0; x < columns; x++)
                    {
                        int index = y * columns + x;
                        if (index >= count) break;

                        GameObject prefab = mapParts.prefabs[index];
                        if (prefab == null) continue;

                        Texture2D preview = AssetPreview.GetAssetPreview(prefab);
                        if (preview == null) preview = AssetPreview.GetMiniThumbnail(prefab);

                        if (GUILayout.Button(preview, GetTileButtonStyle(index), GUILayout.Width(64), GUILayout.Height(64)))
                        {
                            selectNo = index;
                        }
                    }
                    EditorGUILayout.EndHorizontal();
                }
            }
            else
            {
                //メッセージボックスを表示する.
                EditorGUILayout.HelpBox("使用するパーツをセットしてください。", MessageType.None, false);
            }

            GUI.backgroundColor = Color.white; //背景色リセット.
            EditorGUILayout.Space();
        }

        /// <summary>
        /// CSVの値をいじるグリッドを作る.
        /// </summary>
        void MakeMapGrid()
        {
            DrawSeparator();
            EditorGUILayout.LabelField("- Map Data -", EditorStyles.boldLabel); //ラベルを表示する.

            //マップファイルが存在するなら.
            if (File.Exists(filePath))
            {
                scrollPos = EditorGUILayout.BeginScrollView(scrollPos);

                for (int y = 0; y < gridHei; y++)
                {
                    EditorGUILayout.BeginHorizontal();
                    for (int x = 0; x < gridWid; x++)
                    {
                        int id = mapData[x, y];

                        //セルごとの色を設定.
                        Color bgColor;
                        if (id == 0)
                        {
                            bgColor = new Color(0.7f, 0.7f, 0.7f);
                        }
                        else if (id == selectNo+1)
                        {
                            bgColor = Color.HSVToRGB((id*0.15f) % 1f, 0.5f, 1f); //IDごとに色分け.
                        }
                        else
                        {
                            bgColor = new Color(1.0f, 1.0f, 1.0f);
                        }
                        GUI.backgroundColor = bgColor; //色反映.

                        //ボタン描画.
                        string label = id.ToString();
                        if (GUILayout.RepeatButton(label, GUILayout.Width(20), GUILayout.Height(20)))
                        {
                            if (Event.current.button == 1)
                            {
                                mapData[x, y] = 0;          //右クリックで消去.
                            }
                            else
                            {
                                mapData[x, y] = selectNo+1; //左クリックで配置.
                            }
                            GenerateMap();
                        }

                        //背景色リセット.
                        GUI.backgroundColor = Color.white;
                    }
                    EditorGUILayout.EndHorizontal();
                }

                EditorGUILayout.EndScrollView();
            }
            //マップファイルが存在しないなら.
            else
            {
                EditorGUILayout.HelpBox("CSVファイルが存在しません", MessageType.Warning);
            }
            EditorGUILayout.Space();
        }

        /// <summary>
        /// 操作用ボタン(Clear, Save, Load)
        /// </summary>
        void MakeButtons()
        {
            //Begin～Endの中で、均等に埋まるようにボタンを配置してくれる.

            EditorGUILayout.BeginHorizontal(); //横幅の始まり.

            if (GUILayout.Button("Clear Map"))
            {
                mapData = new int[gridWid, gridHei];
                GenerateMap();
            }
            if (GUILayout.Button("Save Map"))
            {
                SaveMap();
            }
            if (GUILayout.Button("Load Map"))
            {
                LoadMap();
                GenerateMap();
            }

            EditorGUILayout.EndHorizontal(); //横幅の終わり.
        }

        /// <summary>
        /// CSV形式で保存.
        /// </summary>
        void SaveMap()
        {
            //ファイルがないなら中断.
            if (!File.Exists(filePath)) { return; }

            using (StreamWriter sw = new StreamWriter(filePath))
            {
                for (int y = 0; y < gridHei; y++)
                {
                    string[] row = new string[gridWid];
                    for (int x = 0; x < gridWid; x++)
                    {
                        row[x] = mapData[x, y].ToString();
                    }
                    sw.WriteLine(string.Join(",", row));
                }
            }

            AssetDatabase.Refresh();
            Debug.Log("Map saved to " + filePath);
        }

        /// <summary>
        /// CSVファイル読み込み.
        /// </summary>
        void LoadMap()
        {
            //ファイルがないなら中断.
            if (!File.Exists(filePath)) { return; }

            //CSV読み込み.
            string[] lines = File.ReadAllLines(filePath);
            gridHei = lines.Length;
            gridWid = lines[0].Split(',').Length;
            //新しいサイズで生成.
            mapData = new int[gridWid, gridHei];
            //読み込んだデータを反映.
            for (int y = 0; y < gridHei; y++)
            {
                string[] values = lines[y].Split(',');
                for (int x = 0; x < gridWid; x++)
                {
                    int.TryParse(values[x], out mapData[x, y]);
                }
            }

            Debug.Log("Map loaded from " + filePath);
        }

        /// <summary>
        /// prefabを設置する.
        /// </summary>
        void GenerateMap()
        {
            //マップパーツに不備があれば.
            if (mapParts == null || 
                mapParts.prefabs == null || 
                mapParts.prefabs.Length == 0)
            {
                return; //中断.
            }

            //古い親オブジェクトを削除.
            GameObject oldMap = GameObject.Find("GeneratedMap");
            if (oldMap != null) {
                DestroyImmediate(oldMap);
            }
            //新しい親オブジェクトを生成.
            GameObject parent = new GameObject("GeneratedMap");

            for (int y = 0; y < gridHei; y++) {
                for (int x = 0; x < gridWid; x++) {

                    //マップデータからid取得.
                    int id = mapData[x, y];

                    //iマップパーツが存在するなら.
                    if (Variable.Func.IsInRange(id, 1, mapParts.prefabs.Length)) 
                    {
                        //マップパーツのprefab選択.
                        GameObject select = mapParts.prefabs[id - 1];

                        if (select)
                        {
                            //prefab生成.
                            GameObject obj = (GameObject)PrefabUtility.InstantiatePrefab(select);

                            //画面の座標を取得.
                            LBRT    windowPos = Position.Func.GetWindowLBRT();
                            //基準点.
                            Vector3 startPos  = new Vector3(windowPos.left+0.5f, windowPos.top-0.5f, 0);
                            //ずらす座標.
                            Vector3 shift     = new Vector3(x, -y, 0) * gridSize;

                            obj.transform.position   = startPos + shift;
                            obj.transform.localScale = Vector3.one * gridSize;
                            obj.transform.SetParent(parent.transform);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 区切り線を描画する.
        /// </summary>
        private void DrawSeparator()
        {
            Color color = EditorGUIUtility.isProSkin
                ? new Color(0.3f, 0.3f, 0.3f)  // Dark  Skin
                : new Color(0.6f, 0.6f, 0.6f); // Light Skin

            Rect rect = EditorGUILayout.GetControlRect(false, 2);
            EditorGUI.DrawRect(rect, color);

            EditorGUILayout.Space(3); //スペースを入れる.
        }

        /// <summary>
        /// ボタンのスタイル設定.
        /// </summary>
        private GUIStyle GetTileButtonStyle(int index)
        {
            GUIStyle style = new GUIStyle(GUI.skin.button);

            //選択中のボタンなら.
            if (index == selectNo)
            {
                //選択中は背景を少し明るく＆枠を付ける.
                style.normal.background = Texture2D.whiteTexture;
                style.normal.textColor = Color.white;
            
                //背景色を設定(DrawTextureは不要)
                GUI.backgroundColor = new Color(0.5f, 0.8f, 1.0f);
            }
            else
            {
                GUI.backgroundColor = Color.white; //そのまま.
            }

            return style;
        }
    }
}