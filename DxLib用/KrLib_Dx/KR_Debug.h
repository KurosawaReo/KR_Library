/*
   - KR_Debug.h - (DxLib)
   ver.2026/04/23

   変数表示、マウス位置表示などのデバッグ機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif
//[include] hで使うもの.
#include "KR_Matrix.h"

//KrLib名前空間.
namespace KR
{
	//デバッグ用の関数群.
	namespace Debug {

		//表示系.
		void Log        (MY_STRING text);
		void Log        (MY_STRING text,  int     value);
		void Log        (MY_STRING text,  float   value);
		void Log        (MY_STRING text,  double  value);
		void Log        (MY_STRING text,  bool    value);
		void Log        (MY_STRING text,  INT_XY  pos);
		void Log        (MY_STRING text,  DBL_XY  pos);
		void Log        (MY_STRING text1, MY_STRING text2);
		void LogMousePos();
		void LogPadID   ();
		void LogMatrix3 (Matrix3 mat);
		void LogClear   ();

		void DispPos    (INT_XY pos, MY_COLOR color = ColorID::Red);
		void DispPos    (DBL_XY pos, MY_COLOR color = ColorID::Red);

		//動作系.
		void MouseToMoveCamera();
	}
}