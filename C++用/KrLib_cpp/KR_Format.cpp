/*
   - KR_Format.cpp - (C++)
*/
#include "KR_Format.h"

//[include] cppでのみ使うもの.
#include <cstdarg> //va_list, va_start, va_end
#include <tchar.h> //TCHAR, _vsntprintf

//KrLib名前空間.
namespace KR
{
	//フォーマット(文字列に変数を挿入する)
	MY_STRING StrFormat(MY_STRING text, ...) {

        TCHAR ret[256];

        va_list args;
        va_start(args, text); //可変引数の開始.

        //sprintfの変化形.
        //TCHAR型対応, va_list型対応.
        int err = _vsntprintf_s(
            ret, _countof(ret), //TCHAR型配列とサイズ.
            _TRUNCATE,          //終端の設定.
            text.c_str(),       //文字列("%d"などが入ったもの)
            args                //挿入する変数(複数可)
        );

        va_end(args); //可変引数の終了.

        //エラーが発生したら"Error"と返す.
        if (err < 0) { return _T("Error"); }
        //結果を返す.
        return ret;
	}
}