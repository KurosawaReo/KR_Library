/*
   - KR_Memory.cpp - (C++)
   ver.2026/01/28
*/
#include "KR_Memory.h"

//KrLib名前空間.
namespace KR
{
// ▼*--=<[ MemoryLeak ]>=--*▼ //

    MemoryLeak MemoryLeak::inst; //実体生成.

// ▼*--=<[ TypePtr ]>=--*▼ //

    //メモリ確保.
    template<typename T>
    bool TypePtr<T>::New() {
        //未確保なら.
        if (!pointer) {
            //確保を試みる.
            try {
                pointer = new T();
                return true;  //確保成功.
            }
            //tryの処理中、例外(bad_alloc)を受け取ったら.
            catch (const std::bad_alloc&) {
                return false; //確保失敗.
            }
        }
    }
    //メモリ解放.
    template<typename T>
    void TypePtr<T>::Delete() {
        //確保済みなら.
        if (pointer) {
            delete pointer; pointer = nullptr; //解放してnullに.
        }
    }
}