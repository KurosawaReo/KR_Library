/*
   - KR_Num.cpp - (C++)
   ver.2026/01/28
*/
#include "KR_BigInt.h"

//KrLib名前空間.
namespace KR
{
    //コンストラクタ.
    BigInt::BigInt() {
        isMinus = false; //正の値.
        num.resize(1);   //1桁.
        num[0] = 0;      //初期値は0.
    }
    BigInt::BigInt(string _num) {
        SetStr(_num);    //数値として保存.
    }
    BigInt::BigInt(int _num) {
        SetNum(_num);    //数値として保存.
    }

    //get(文字列で取得)
    string BigInt::GetStr() const {

        string str;

        //文字列作成.
        str.resize(num.size(), '0');
        //負の値なら.
        if (isMinus) {
            str.insert(str.end(), '-'); //数字の前に「-」を入れる.
        }
        //桁数分ループ.
        for (int i = 0; i < num.size(); i++) {
            //Error: 負の値を見つけたら.
            if (num[i] < 0) { return "[Invalid num] 表示できない値です。"; }
            str[i] += num[i]; //1桁を1字に変換.
        }
        //順序入れ替え.
        reverse(str.begin(), str.end());

        return str;
    }
    //get(絶対値で取得)
    BigInt BigInt::GetAbs() const {

        BigInt tmp = *this;

        tmp.isMinus = false;                          //符号はなし.
        for (auto& i : tmp.num) { i = abs(_int(i)); } //全ての桁を絶対値に.

        return tmp;
    }
    //set(string型で保存)
    void BigInt::SetStr(string _num) {

        isMinus = (_num[0] == '-');               //符号.
        if (_num[0] == '-') { _num.erase(0, 1); } //「-」があったら消して詰める.

        //strの桁数に合わせる.
        num.resize(_num.size());
        //順序入れ替え.
        reverse(_num.begin(), _num.end());
        //strの桁数分ループ.
        for (int i = 0; i < _num.size(); i++) {
            num[i] = _num[i] - '0'; //1桁ずつ保存.
        }
    }
    //set(int型で保存)
    void BigInt::SetNum(int _num) {
        
        isMinus = (_num < 0);         //符号.
        if (_num < 0) { _num *= -1; } //負の数なら符号反転.

        const int digit = _int(log10(_num))+1; //桁数を求める.
        num.resize(digit);                     //サイズを合わせる.
        
        //桁ごとに分割して保存.
        int tmp = _num;
        for (int i = digit-1; i >= 0; i--) {
            num[i] = _int(tmp/pow(10, i)); //割って切り捨てた値.
            tmp %= _int(pow(10, i));       //残った余り.
        }
    }

    //符号反転.
    void BigInt::ReverseSign() {
        isMinus = !isMinus;              //「-」→「+」/「+」→「-」.
        for (auto& i : num) { i *= -1; } //各桁を符号反転する.
    }
    //×10^n.
    void BigInt::AddDigit(unsigned int n) {

        const int befDigit = _int(num.size()); //元の桁数.
        num.resize(befDigit + n);              //n桁増やす.

        //元の桁数ループ.
        for (int i = befDigit-1; i >= 0; i--) {
            num[i+n] = num[i]; //値をシフト.
            num[i]   = 0;      //シフト済の桁は0に.
        }
    }
    //×0.1^n.
    void BigInt::RemDigit(unsigned int n) {

        const int aftDigit = _int(num.size()) - n; //何桁に減るか.
        if (aftDigit < 0) { return; }              //0桁未満になるなら中断.

        //元の桁数ループ.
        for (int i = n; i < num.size(); i++) {
            num[i]   = num[i+n]; //値をシフト.
            num[i+n] = 0;        //シフト済の桁は0に.
        }
        num.resize(aftDigit); //n桁減らす.
    }
    //桁の計算.
    void BigInt::Digit() {

        //符号が入れ替わったか判定.
        for (int i = _int(num.size())-1; i >= 0; i--) {
            //初めに着いた桁が+か-かで判定.
            if (num[i] < 0) {
                ReverseSign(); //符号反転.
                break;         //終了.
            }
            else if (num[i] > 0) {
                break; //何もしないで終了.
            }
        }

        int maxDigit = 0; //桁数(繰り下げ用)

        //繰り上げ & 繰り下げ処理.
        for (int i = 0; i < num.size(); i++) {

            //[繰り上げ]
            if (num[i] >= 10) {
                //桁数が足りなければ1桁増やす.
                if (i == num.size()-1) { num.push_back(0); }
                num[i+1] += num[i]/10; //繰り上げる数.
                num[i]   %= 10;        //残りの数.
            }
            //[繰り下げ]
            if (num[i] < 0) {
                //Error: 1番上の桁で繰り下げが発生.
                if (i+1 == num.size()) {
                    std::cout << "[Error] 最上位の桁で繰り下げが発生しました。" << std::endl;
                    break;
                }
                num[i+1] -=  (num[i]/-10)+1;     //繰り下げる数.
                num[i]   += ((num[i]/-10)+1)*10; //繰り下げ分を加算.
            }
            //[桁数計測] 今の数字が0でなければ.
            if (num[i] != 0) {
                maxDigit = i+1; //暫定の最高桁.
            }
        }
        //不要な0の桁は詰める.
        if (maxDigit < num.size()) {
            num.resize(__max(maxDigit, 1)); //桁を減らす(最低1桁は残す)
        }
    }

    //割り算.
    void BigInt::Division(const BigInt& other, BigInt* syou, BigInt* amari) const {
       
        //0割対策.
        if (other.GetStr() == "0") { return; }

        BigInt tmp1("0"); //余りを求める用.
        BigInt tmp2("0"); //商を求める用.

        const BigInt right = other.GetAbs(); //右辺(割る数)

        //全桁を上からループ.
        for (int i = _int(num.size())-1; i >= 0; i--) {
            tmp1.AddDigit(1);     //×10(桁をずらす)
            tmp2.AddDigit(1);     //×10(桁をずらす)
            tmp1.num[0] = num[i]; //1桁ずつ入れる.
            //引けるだけ引く.
            while (tmp1 >= right) {
                tmp1 -= right;    //1回引く.
                tmp2.num[0] += 1; //商に+1
            }
            //桁の計算(不要な0を消す)
            tmp1.Digit();
            tmp2.Digit();
        }
        //余りに符号を適用.
        tmp1.isMinus = isMinus;
        //符号が異なるなら.
        if (isMinus != other.isMinus) {
            tmp2.isMinus = true; //商を負の値にする.
        }

        //計算結果を返す.
        *syou  = tmp2;
        *amari = tmp1;
    }
    //比較.
    bool BigInt::Comparison(const BigInt& other, bool bigger, bool smaller, bool equal) const {

        //①符号比較.
        if (isMinus != other.isMinus) {
            return (!isMinus) ? bigger : smaller; // + > - / - < +
        }
        //②桁数比較.
        if (num.size() != other.num.size()) {
            if (!isMinus) {
                return (num.size() > other.num.size()) ? bigger : smaller; //例:+100 > +10
            }
            else {
                return (num.size() > other.num.size()) ? smaller : bigger; //例:-100 < -10
            }
        }
        //③桁を上から比較.
        for (int i = _int(num.size())-1; i >= 0; i--) {
            if (num[i] == other.num[i]) { continue; } //一致していたら次へ.
            if (!isMinus) { return (num[i] > other.num[i]) ? bigger : smaller; } //例:+103 > +102
            else          { return (num[i] > other.num[i]) ? smaller : bigger; } //例:-103 < -102
        }

        return equal; //等しい値.
    }

    //足し算.
    BigInt BigInt::operator+(const BigInt& other) const {

        //符号が異なるなら引き算にする.
        if (isMinus != other.isMinus) {
            BigInt tmp = other;
            tmp.isMinus = !tmp.isMinus;
            return *this - tmp;
        }

        BigInt left = *this;         //左辺.
        const BigInt& right = other; //右辺.

        //右辺の桁が大きいなら合わせる.
        if (left.num.size() < right.num.size()) {
            left.num.resize(right.num.size());
        }
        //各桁を加算.
        for (int i = 0; i < right.num.size(); i++) {
            left.num[i] += right.num[i];
        }

        left.Digit(); //桁の計算.
        return left;  //計算結果を返す.
    }
    //引き算.
    BigInt BigInt::operator-(const BigInt& other) const {

        //符号が異なるなら足し算にする.
        if (isMinus != other.isMinus) {
            BigInt tmp = other;
            tmp.isMinus = !tmp.isMinus;
            return *this + tmp;
        }

        BigInt left = *this;         //左辺.
        const BigInt& right = other; //右辺.

        //右辺の桁が大きいなら合わせる.
        if (left.num.size() < right.num.size()) {
            left.num.resize(right.num.size());
        }
        //各桁を減算.
        for (int i = 0; i < right.num.size(); i++) {
            left.num[i] -= right.num[i];
        }

        left.Digit(); //桁の計算.
        return left;  //計算結果を返す.
    }
    //掛け算.
    BigInt BigInt::operator*(const BigInt& other) const {

        BigInt ret;         //計算結果用.
        BigInt tmp = *this; //掛け算で加算する数.

        //otherの全桁ループ.
        for (int i = 0; i < other.num.size(); i++) {
            //掛ける数分ループ.
            for (int j = 0; j < other.num[i]; j++) {
                ret = ret + tmp; //1回足す.
            }
            tmp.AddDigit(1); //1桁増やす.
        }
        //符号が異なるなら.
        if (isMinus != other.isMinus) {
            ret.isMinus = true; //retを負の値にする.
        }
        ret.Digit(); //桁の計算.
        return ret;  //計算結果を返す.
    }
    //割り算.
    BigInt BigInt::operator/(const BigInt& other) const {
        BigInt syou("0"), amari("0");
        Division(other, &syou, &amari); //割り算する.
        return syou;                    //計算結果(商)を返す.
    }
    //余り算.
    BigInt BigInt::operator%(const BigInt& other) const {
        BigInt syou("0"), amari("0");
        Division(other, &syou, &amari); //割り算する.
        return amari;                   //計算結果(余り)を返す.
    }
    //複合代入演算子.
    BigInt& BigInt::operator+=(const BigInt& other) {
        *this = *this + other;
        return *this;
    }
    BigInt& BigInt::operator-=(const BigInt& other) {
        *this = *this - other;
        return *this;
    }
    BigInt& BigInt::operator*=(const BigInt& other) {
        *this = *this * other;
        return *this;
    }
    BigInt& BigInt::operator/=(const BigInt& other) {
        *this = *this / other;
        return *this;
    }
    BigInt& BigInt::operator%=(const BigInt& other) {
        *this = *this % other;
        return *this;
    }

    //比較: n > n
    bool BigInt::operator> (const BigInt& other) const {
        return Comparison(other, true, false, false);
    }
    //比較: n < n
    bool BigInt::operator< (const BigInt& other) const {
        return Comparison(other, false, true, false);
    }
    //比較: n >= n
    bool BigInt::operator>=(const BigInt& other) const {
        return Comparison(other, true, false, true);
    }
    //比較: n <= n
    bool BigInt::operator<=(const BigInt& other) const {
        return Comparison(other, false, true, true);
    }
    //比較: n == n
    bool BigInt::operator==(const BigInt& other) const {
        return Comparison(other, false, false, true);
    }
}