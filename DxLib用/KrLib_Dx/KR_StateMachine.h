/*
   - KR_StateMachine.h - (DxLib)
   ver.2026/02/04

   ステート遷移機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
    /*
       状態クラス[継承想定]
       AIの行動、シーン、stepなど、様々な切り替えに使える.
    */
    class IState
    {
    public:
        virtual void Enter()  = 0; //状態に入った瞬間.
        virtual void Exit()   = 0; //状態を抜けた瞬間.
        virtual void Update() = 0; //更新.
        virtual void Draw()   = 0; //描画.
    };

    /*
       状態管理クラス.

       RequestChangeで次のstateを設定すると
       次にUpdateが動いた時に変更される.
       (App::Reset中にstate変更した時、即ExitとEnterを実行されないように)
    */
    class StateMachine final
    {
    //▼ ===== 変数 ===== ▼.
    private:
        IState* current = nullptr; //現在のstate.
        IState* next    = nullptr; //次に変更するstate(予約)

    //▼ ===== 関数 ===== ▼.
    private:
        void ChangeState(); //state変更.

    public:
        //get.
        IState* GetCurrent() const { return current; }

        void RequestChange(IState* state); //state遷移予約.
        void Update();                     //更新.
        void Draw();                       //描画.
    };
}
