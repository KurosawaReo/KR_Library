/*
   - KR_File.h - (C++)
   ver.2026/01/28

   ファイルの読み取り/書き出し機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_CPP_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//ファイルクラス.
	class File
	{
	//▼ ===== 変数 ===== ▼.
	private:
		FILE* fp; //ファイルポインタ.

	//▼ ===== 関数 ===== ▼.
	private:
		ResultInt MakeDir(MY_STRING path); //フォルダを作成(なければ)

	public:
		//デストラクタ.
		~File() {
			Close(); //自動で閉じる.
		}

		ResultInt Open   (MY_STRING path, MY_STRING mode, bool isMakeDir = false); //ファイルを開く.
		void	  Close  ();								                       //ファイルを閉じる.

		MY_STRING ReadString ();               //読み込み(文字列)
		int       ReadInt    ();               //読み込み(数字)
		void      WriteString(MY_STRING data); //書き込み(文字列)
		void      WriteInt   (int data);       //書き込み(数字)
	};
}