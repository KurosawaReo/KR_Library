/*
   - KR_File.h - (C++)
   ver.2026/04/27

   ファイルの読み取り/書き出し機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_CPP_GLOBAL
  #include "KR_Global.h"
#endif

#include <fstream>
#include <filesystem>

//KrLib名前空間.
namespace KR
{
    //ファイルを開くモード.
    enum class FileOpenMode
    {
        Read,   //読み込み.
        Write,  //書き込み.
        Append, //追記(末尾に追加)
        Trunc,  //開いた時に中身を空にする.
        Binary  //バイナリモード.
    };

    //ファイルクラス.
    class File final
    {
    private:
        std::fstream fs; //ファイルストリーム.

    public:
        //デストラクタ.
        ~File() {
            Close(); //ファイルを閉じる.
        }

        //EOFかどうか.
        bool IsEOF();

        //ファイルを開く.
        void Open(const std::string& filePath, FileOpenMode mode);
        //ファイルを閉じる.
        void Close();
        //ディレクトリ作成.
        void MakeDir(const std::filesystem::path& filePath);

        //読み込み, 書き込み.
        string ReadString ();
        int    ReadInt    ();
        void   WriteString(const std::string& data);
        void   WriteInt   (int data);

    };
}