/*
   - KR_Format.h - (C++)
   ver.2026/01/28

   文字列のフォーマット機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_CPP_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//フォーマット(文字列に変数を挿入する)
	MY_STRING StrFormat(MY_STRING text, ...);
}