/*
   - KR_Singleton.h - (C++)
   ver.2026/04/27

   シングルトンクラス用機能。(試験的)
*/
#pragma once

/*
   シングルトンテンプレ [継承想定]

   これも入れて使う。コンストラクタをprivateにしても使えるように。
   friend class Singleton<T>; //Tにはクラス名を入れる.
*/
template <typename T>
class Singleton
{
public:
    static T& GetInst() {
        static T inst; //実体生成.
        return inst;
    }

    //使用禁止.
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() {}  //継承クラスからのみ生成可.
    ~Singleton() {}
};