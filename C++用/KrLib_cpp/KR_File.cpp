/*
   - KR_File.cpp - (C++)
*/
#include "KR_File.h"

//KrLib名前空間.
namespace KR
{
    //EOFかどうか.
    bool File::IsEOF() {
        return fs.eof();
    }

    //ファイルを開く.
    void File::Open(const std::string& filePath, FileOpenMode mode) {

        Close();           //先に閉じる.
        MakeDir(filePath); //ディレクトリがなければ作成.

        //列挙値をモードに変換.
        std::ios::openmode stdMode;
        switch (mode)
        {
        case FileOpenMode::Read:
            stdMode = std::ios::in;
            break;
        case FileOpenMode::Write:
            stdMode = std::ios::out;
            break;
        case FileOpenMode::Append:
            stdMode = std::ios::app;
            break;
        case FileOpenMode::Trunc:
            stdMode = std::ios::trunc;
            break;
        case FileOpenMode::Binary:
            stdMode = std::ios::binary;
            break;
        }
        //ファイルを開く.
        fs.open(filePath, stdMode);
        if (!fs.is_open()) {
            throw ErrorMsg(_T("File::Open"), _T("ファイル読み込み失敗"));
        }
    }

    //ファイルを閉じる.
    void File::Close() {

        //もし開かれていたら.
        if (fs.is_open()) {
            fs.close(); //閉じる.
        }
    }

    //ディレクトリ作成.
    void File::MakeDir(const std::filesystem::path& filePath) {

        //親ディレクトリを取得.
        const auto parentDir = filePath.parent_path();
        if (parentDir.empty()) return;

        //ディレクトリを作る(既に存在してても問題ない)
        std::error_code ec;
        std::filesystem::create_directories(parentDir, ec);
        if (ec) {
            throw ErrorMsg(_T("File::MakeDir"), _T("ディレクトリ作成失敗"));
        }
    }

    //string型読み込み(1行)
    string File::ReadString()
    {
        //エラーチェック.
        if (!fs.is_open()) {
            throw ErrorMsg(_T("File::ReadString"), _T("ファイルが開かれてない"));
        }

        //1行読み込む.
        string line;
        if (!std::getline(fs, line)) {
            throw ErrorMsg(_T("File::ReadString"), _T("ファイル読み込み失敗"));
        }
        return line;
    }
    //int型読み込み(1行)
    int File::ReadInt()
    {
        //まず文字列で読み込む.
        std::string str = ReadString();
        //int型に変換.
        try {
            return std::stoi(str);
        }
        catch (...) {
            throw ErrorMsg(_T("File::ReadInt"), _T("int型変換失敗"));
        }
    }
    //string型書き込み.
    void File::WriteString(const std::string& data)
    {
        //エラーチェック.
        if (!fs.is_open()) {
            throw ErrorMsg(_T("File::WriteString"), _T("ファイルが開かれてない"));
        }
        //書き込む.
        fs << data;
    }
    //int型書き込み.
    void File::WriteInt(int data)
    {
        //エラーチェック.
        if (!fs.is_open()) {
            throw ErrorMsg(_T("File::WriteInt"), _T("ファイルが開かれてない"));
        }
        //書き込む.
        fs << data;
    }
}