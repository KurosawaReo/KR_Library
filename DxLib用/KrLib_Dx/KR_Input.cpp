/*
   - KR_Input.cpp - (DxLib)
*/
#include "KR_Input.h"

//[include] cppでのみ使うもの.
#include "KR_App.h"

//KrLib名前空間.
namespace KR
{
	InputMng InputMng::inst; //実体生成.

	//キー入力の判定.
	bool InputMng::IsPushKey(KeyID id) {
		return inst.tmKey[_int(id)] > 0;
	}
	int  InputMng::IsPushKeyTime(KeyID id) {
		return inst.tmKey[_int(id)];
	}
	//マウス入力の判定.
	bool InputMng::IsPushMouse(MouseID id) {
		return inst.tmMouse[_int(id)] > 0;
	}
	int  InputMng::IsPushMouseTime(MouseID id) {
		return inst.tmMouse[_int(id)];
	}
	//コントローラ入力の判定.
	bool InputMng::IsPushPadBtn(PadXboxID id) {
		return inst.tmPadBtn[_int(id)] > 0;
	}
	bool InputMng::IsPushPadBtn(PadSwitchID id) {
		return inst.tmPadBtn[_int(id)] > 0;
	}
	bool InputMng::IsPushPadBtn(PadArcadeID id) {
		return inst.tmPadBtn[_int(id)] > 0;
	}
	int  InputMng::IsPushPadBtnTime(PadXboxID id) {
		return inst.tmPadBtn[_int(id)];
	}
	int  InputMng::IsPushPadBtnTime(PadSwitchID id) {
		return inst.tmPadBtn[_int(id)];
	}
	int  InputMng::IsPushPadBtnTime(PadArcadeID id) {
		return inst.tmPadBtn[_int(id)];
	}
	//アクション判定.
	bool InputMng::IsPushAction(string name) {
		return inst.actions[name].time > 0; //押してる時間があればtrue.
	}
	int  InputMng::IsPushActionTime(string name) {
		return inst.actions[name].time;     //時間を返す.
	}

	//アクション追加.
	void InputMng::AddAction(string name, KeyID id) {
		inst.actions[name].inputs.push_back({ InputType::Key,       _int(id) }); //Key操作で登録.
	}
	void InputMng::AddAction(string name, MouseID id) {
		inst.actions[name].inputs.push_back({ InputType::Mouse,     _int(id) }); //Mouse操作で登録.
	}
	void InputMng::AddAction(string name, PadXboxID id) {
		inst.actions[name].inputs.push_back({ InputType::PadXbox,   _int(id) }); //Pad操作(xbox)で登録.
	}
	void InputMng::AddAction(string name, PadSwitchID id) {
		inst.actions[name].inputs.push_back({ InputType::PadSwitch, _int(id) }); //Pad操作(switch)で登録.
	}
	void InputMng::AddAction(string name, PadArcadeID id) {
		inst.actions[name].inputs.push_back({ InputType::PadArcade, _int(id) }); //Pad操作(arcade)で登録.
	}

	//キーボード操作取得(十字キー)
	DBL_XY InputMng::GetKey4Dir() {

		INT_XY input{}; //入力.

		//キー入力に応じて移動力を与える.
		if (IsPushKey(KeyID::Up)   ||IsPushKey(KeyID::W)) {
			input.y += -1;
		}
		if (IsPushKey(KeyID::Down) ||IsPushKey(KeyID::S)) {
			input.y += +1;
		}
		if (IsPushKey(KeyID::Left) ||IsPushKey(KeyID::A)) {
			input.x += -1;
		}
		if (IsPushKey(KeyID::Right)||IsPushKey(KeyID::D)) {
			input.x += +1;
		}
		//入力ベクトル(-1.0～1.0)を返す.
		return inst.GetVector4Dir(input);
	}
	//コントローラ操作取得(十字キー)
	DBL_XY InputMng::GetPad4Dir() {

		INT_XY input{}; //入力.

		//キー入力に応じて移動力を与える.
		if (IsPushPadBtn(PadXboxID::Up)) {
			input.y += -1;
		}
		if (IsPushPadBtn(PadXboxID::Down)) {
			input.y += +1;
		}
		if (IsPushPadBtn(PadXboxID::Left)) {
			input.x += -1;
		}
		if (IsPushPadBtn(PadXboxID::Right)) {
			input.x += +1;
		}
		//入力ベクトル(-1.0～1.0)を返す.
		return inst.GetVector4Dir(input);
	}
	//コントローラ操作取得(スティック)
	DBL_XY InputMng::GetPadStick() {
		//範囲-1000～1000を-1.0～1.0に変換.
		return inst.stickVec.ToDbl() / 1000;
	}
	//マウス座標取得.
	DBL_XY InputMng::GetMousePos() {
		return App::ToCameraPos(inst.mPos.ToDbl()); //カメラ座標にして返す.
	}
	//移動4方向処理(斜め計算)
	DBL_XY InputMng::GetVector4Dir(INT_XY input) {

		DBL_XY vec{}; //ベクトル.

		//移動力があれば.
		if (input.x != 0 || input.y != 0) {
			//角度にする.
			double theta = atan2(input.y, input.x);
			//移動量を求める.
			vec = { cos(theta), sin(theta) };
			//ほぼ0の値なら0と見なす(計算上誤差があるため)
			if (fabs(vec.x) < 0.0001) { vec.x = 0; }
			if (fabs(vec.y) < 0.0001) { vec.y = 0; }
		}
		return vec;
	}

	//更新:ボタン.
	void InputMng::UpdateKey() {
	
		char key[KEY_MAX];
		GetHitKeyStateAll(key); //押しているキー情報を取得.
	
		for (int i = 0; i < KEY_MAX; i++) {
			//押されているなら.
			if (key[i] != 0) {
				inst.tmKey[i]++;   //カウント.
			}
			else {
				inst.tmKey[i] = 0; //0秒にリセット.
			}
		}
	}
	//更新:マウス.
	void InputMng::UpdateMouse() {

		//マウス座標取得.
		GetMousePoint(&inst.mPos.x, &inst.mPos.y);

		for (int i = 0; i < MOUSE_MAX; i++) {
			//押されているなら.
			if ((GetMouseInput() & (1 << i)) != 0) { //And演算で抽出.
				inst.tmMouse[i]++;   //カウント.
			}
			else {
				inst.tmMouse[i] = 0; //0秒にリセット.
			}
		}
	}
	//更新:コントローラ.
	void InputMng::UpdatePad() {

		//スティック入力取得.
		GetJoypadAnalogInput(&inst.stickVec.x, &inst.stickVec.y, DX_INPUT_PAD1);

		for (int i = 0; i < PAD_BTN_MAX; i++) {
			//押されているなら.
			if (GetJoypadInputState(DX_INPUT_PAD1) & (1 << i)) { //And演算で抽出.
				inst.tmPadBtn[i]++;   //カウント.
			}
			else {
				inst.tmPadBtn[i] = 0; //0秒にリセット.
			}
		}
	}
	//更新:アクション.
	void InputMng::UpdateAction() {

		//ActionDataを全ループ.
		for (auto& i : inst.actions) {

			bool isPush = false; //何かを押しているかどうか.

			//登録されたInputDataを全ループ.
			for (auto& j : i.second.inputs) {

				switch (j.type)
				{
					case InputType::Key:       isPush = IsPushKey   (static_cast<KeyID>      (j.id)); break;
					case InputType::Mouse:     isPush = IsPushMouse (static_cast<MouseID>    (j.id)); break;
					case InputType::PadXbox:   isPush = IsPushPadBtn(static_cast<PadXboxID>  (j.id)); break;
					case InputType::PadSwitch: isPush = IsPushPadBtn(static_cast<PadSwitchID>(j.id)); break;
					case InputType::PadArcade: isPush = IsPushPadBtn(static_cast<PadArcadeID>(j.id)); break;

					default: assert(FALSE); break;
				}
				//押しているなら.
				if (isPush) {
					break; //終了.
				}
			}

			//何か1つでも押していたなら.
			if (isPush) {
				i.second.time++;   //足す.
			}
			else {
				i.second.time = 0; //リセット.
			}
		}
	}

	//更新(自動実行)
	void InputMng::Update() {
		UpdateKey();
		UpdateMouse();
		UpdatePad();
		UpdateAction();
	}
}