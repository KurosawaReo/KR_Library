/*
   - KR_StateMachine.cpp - (DxLib)
   ver.2026/01/31
*/
#pragma once
#include "KR_StateMachine.h"

//KrLib–¼‘O‹óŠÔ.
namespace KR
{
    //state‘JˆÚ—\–ñ.
    void StateMachine::RequestChange(IState* state) {
        if (current == state) return; //“¯‚¶state‚É‚Í‘JˆÚ‚µ‚È‚¢.
        next = state;                 //ŽŸ‚Ìstate‚É‚·‚é.
    }
    //state•ÏX.
    void StateMachine::ChangeState() {
        //ŽŸ‚É‘JˆÚ‚·‚éstate‚ª‚ ‚ê‚Î.
        if (next) {
            if (current) { current->Exit(); }   //Œ³‚Ìstate‚©‚çExit.
            current = next;                     //state‚ð•ÏX.
            next = nullptr;                     //next‚Í‹ó‚É.
            if (current) { current->Enter(); }  //V‚½‚Èstate‚ÉEnter.
        }
    }
    //XV.
    void StateMachine::Update() {
        ChangeState();
        if (current) { current->Update(); } //Œ»state‚ÌXV.
    }
    //•`‰æ.
    void StateMachine::Draw() {
        if (current) { current->Draw(); }   //Œ»state‚Ì•`‰æ.
    }
}