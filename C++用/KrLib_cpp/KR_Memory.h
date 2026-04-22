/*
   - KR_Memory.h - (C++)
   ver.2026/04/23

   メモリ管理機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_CPP_GLOBAL
  #include "KR_Global.h"
#endif

/*----------------------------------*/
//メモリリーク検出用.
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
  #define new new(_NORMAL_BLOCK, __FILE__, __LINE__) //new演算子の代用.
#endif
/*----------------------------------*/

//KrLib名前空間.
namespace KR
{
    //メモリリーク検出設定用[継承不可]
    class MemoryLeak final
    {
    //▼ ===== 実体 ===== ▼.
    private:
        static MemoryLeak inst; //実体を入れる用.

    //▼ ===== 関数 ===== ▼.
    public:
        //コンストラクタ.
        MemoryLeak() {
            //デバッグ欄にログを出す設定.
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        }

        //使用禁止.
        MemoryLeak(const MemoryLeak&) = delete;
        MemoryLeak& operator=(const MemoryLeak&) = delete;
    };

    //ポインタ型を扱うクラス [試作品]
    template<typename T>
    class TypePtr
    {
    private:
        T* pointer; //ポインタを入れる用.

    public:
        //コンストラクタ.
        TypePtr() : pointer(nullptr) {}
        //デストラクタ.
        ~TypePtr() {
            Delete();
        }
        //get.
        T* GetPtr() const { return pointer; }

        bool New();    //メモリ確保.
        void Delete(); //メモリ解放.
    };
}