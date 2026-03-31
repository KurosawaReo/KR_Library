/*
   - KR_Timer.cpp - (DxLib)
   ver.2026/01/28
*/
#include "KR_Timer.h"

//KrLib名前空間.
namespace KR
{

	//一定時間ごとにtrueを返す(CountDown専用)
	bool TimerBase::IntervalTime() {

		//CountDownじゃない場合はfalseを返し続ける.
		if (mode != TimerMode::CountDown) {
			return false;
		}

		//まだ動いてなかったら.
		if (state == TimerState::Stop) {
			Start(); //タイマー開始.
		}
		//タイマーが0になったら.
		if (IsCountDownEnd()) {
			Reset();
			Start();
			return true; //trueを返す.
		}
		return false; //falseを返す.
	}

// ▼*--=<[ Timer ]>=--*▼ //

	//開始.
	void Timer::Start() {
		tmStart = clock();          //開始時刻の取得.
		state = TimerState::Active; //タイマー稼働.
	}
	//リセット.
	void Timer::Reset() {
		tmStart = 0;
		tmSavePass = tmInit; //初期時刻.
		state = TimerState::Stop;
	}

	//タイマー停止.
	bool Timer::TimerStop() {

		//計測中なら.
		if (state == TimerState::Active) {

			float elapsed = _flt(clock() - tmStart)/1000; //時間差.

			//経過時間を更新.
			if (mode == TimerMode::CountUp) {
				tmSavePass += elapsed;           //タイマー増加.
			}
			else {
				tmSavePass -= elapsed;           //タイマー減少.
				tmSavePass = max(tmSavePass, 0); //下限は0秒.
			}

			return true; //停止した.
		}
		return false; //停止してない.
	}
	//経過時間取得.
	float Timer::GetPassTime() const {

		//計測中なら.
		if (state == TimerState::Active) {

			float elapsed = _flt(clock() - tmStart)/1000; //時間差.
			float pass = 0; //経過時間.

			if (mode == TimerMode::CountUp) {
				pass = tmSavePass + elapsed; //タイマー増加.
			}
			else {
				pass = tmSavePass - elapsed; //タイマー減少.
				pass = max(pass, 0);         //下限は0秒.
			}
			return pass; //仮で計算した時間差を返す.
		}
		else {
			return tmSavePass; //保存時間を返す.
		}	
	}
	//カウントダウンが終了したか.
	bool Timer::IsCountDownEnd() const {
		return GetPassTime() <= 0;
	}

// ▼*--=<[ TimerMicro ]>=--*▼ //

	//開始.
	void TimerMicro::Start() {
		QueryPerformanceCounter(&tmStart); //開始時刻の取得.
		state = TimerState::Active;
	}
	//リセット.
	void TimerMicro::Reset() {
		tmStart.QuadPart = 0;
		tmSavePass = tmInit; //初期時刻.
		state = TimerState::Stop;
	}

	//タイマー停止.
	bool TimerMicro::TimerStop() {

		//計測中なら.
		if (state == TimerState::Active) {
		
			LARGE_INTEGER tmEnd;
			QueryPerformanceCounter(&tmEnd); //終了時刻の取得.

			//時間差(freqでカウントをマイクロ秒に変換する)
			LONGLONG elapsed = (tmEnd.QuadPart - tmStart.QuadPart) * 1000000/freq.QuadPart;

			//経過時間を更新.
			if (mode == TimerMode::CountUp) {
				tmSavePass += elapsed;           //タイマー増加.
			}
			else {
				tmSavePass -= elapsed;           //タイマー減少.
				tmSavePass = max(tmSavePass, 0); //下限は0秒.
			}
			return true; //停止した.
		}
		return false; //停止してない.
	}
	//経過時間取得(マイクロ秒)
	LONGLONG TimerMicro::GetPassTime() const {

		//計測中なら.
		if (state == TimerState::Active) {

			LARGE_INTEGER tmEnd;
			QueryPerformanceCounter(&tmEnd); //終了時刻の取得.

			//時間差(freqでカウントをマイクロ秒に変換する)
			LONGLONG elapsed = (tmEnd.QuadPart - tmStart.QuadPart) * 1000000/freq.QuadPart;
			//経過時間.
			LONGLONG pass;

			if (mode == TimerMode::CountUp) {
				pass = tmSavePass + elapsed; //タイマー増加.
			}
			else{
				pass = tmSavePass - elapsed; //タイマー減少.
				pass = max(pass, 0);         //下限は0カウント.
			}
			return pass; //仮で計算した時間差を返す.
		}
		else {
			return tmSavePass; //保存時間を返す.
		}
	}
	//fps取得.
	double TimerMicro::GetFps() {

		//計測中なら.
		if (state == TimerState::Active) {

			LARGE_INTEGER tmEnd;
			QueryPerformanceCounter(&tmEnd); //終了時刻の取得.

			//時間差(freqでカウントをマイクロ秒に変換する)
			LONGLONG elapsed = (tmEnd.QuadPart - tmStart.QuadPart) * 1000000/freq.QuadPart;
			//fpsの計算.
			const double fps = _flt(1000000)/elapsed;

			Start(); //時間リセット.

			return fps; //fpsを返す.
		}
		else {
			return 0; //計測中じゃない時はfps0
		}
	}
	//カウントダウンが終了したか.
	bool TimerMicro::IsCountDownEnd() const {
		return GetPassTime() <= 0;
	}
}