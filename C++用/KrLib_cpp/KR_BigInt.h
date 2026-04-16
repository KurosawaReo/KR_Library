/*
   - KR_BigInt.h - (C++)
   ver.2026/04/16

   膨大な数値を扱えるint型。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_CPP_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
    /*
       big int型(long long型でも入らない膨大な桁数も計算可能)
       | [+] 完成 | [-] 完成 | [*] 完成 | [/] 完成 | [%] 完成 |
       
       num     : 符号なしの値(1桁ずつ管理)
       isMinus : 符号(trueなら-)

       numが負の値になったら(=符号が入れ替わったら)
       符号(isMinus)を反転して、numの全ての桁に-1をかける。
    */
    class BigInt
    {
    //▼ ===== 変数 ===== ▼.
    private:
        vector<char> num; //符号なしの値(1桁ずつ管理)
        bool isMinus;     //符号(trueなら-)

    //▼ ===== 関数 ===== ▼.
    public:
        //コンストラクタ.
        BigInt();
        BigInt(string _num);
        BigInt(int    _num);
        //get.
        string  GetStr() const; //文字列で取得.
        BigInt  GetAbs() const; //絶対値で取得.
        //set.
        void    SetStr(string _num);
        void    SetNum(int    _num);

        //符号反転.
        void    ReverseSign();
        //桁追加(×10^n)
        void    AddDigit(unsigned int n);
        //桁削除(×0.1^n)
        void    RemDigit(unsigned int n);
        //桁の計算.
        void    Digit();

        //割り算.
        void    Division  (const BigInt&, BigInt* syou, BigInt* amari) const;
        //比較.
        bool    Comparison(const BigInt&, bool bigger, bool smaller, bool equal) const;
        
        //四則演算子.
        BigInt  operator+ (const BigInt& other) const;
        BigInt  operator- (const BigInt& other) const;
        BigInt  operator* (const BigInt& other) const;
        BigInt  operator/ (const BigInt& other) const;
        BigInt  operator% (const BigInt& other) const;
        //複合代入演算子.
        BigInt& operator+=(const BigInt& other);
        BigInt& operator-=(const BigInt& other);
        BigInt& operator*=(const BigInt& other);
        BigInt& operator/=(const BigInt& other);
        BigInt& operator%=(const BigInt& other);
        //比較演算子.
        bool    operator> (const BigInt& other) const;
        bool    operator< (const BigInt& other) const;
        bool    operator>=(const BigInt& other) const;
        bool    operator<=(const BigInt& other) const;
        bool    operator==(const BigInt& other) const;
    };
}