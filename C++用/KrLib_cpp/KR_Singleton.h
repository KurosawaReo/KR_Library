/*
   - KR_Singleton.h - (C++)
   ver.2026/04/02

   シングルトンクラス用機能。(試験的)
*/
#pragma once

/*
   [TODO]
   Singleton::GetInst()を初めて実行した時に実体生成するため
   ManagerBaseと併用して使う場合、App::InitDx()が動く前に実体登録ができない問題がある。
*/

/*
   シングルトンテンプレ[継承想定]

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
    Singleton() {}  //継承クラスからのみ生成可,
    ~Singleton() {}
};